/**********************************************************************
 * @copyright   Copyright (C), 2017, Sensetime Tech. Co., Ltd.
 * @file        lct_event_manager.hpp
 * @version     1.0
 * @date        Jun 13, 2017 5:43:27 PM
 * @author      liuyang@sensetime.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_EVENT_CLCTEVENTMANAGER_HPP_
#define SRC_EVENT_CLCTEVENTMANAGER_HPP_

#include "CPlusPlus14.h"
#include "LctLog.h"

/*******************************************************************************************
 *
 * Life Cycle
 *
 *******************************************************************************************/
template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::init(const int64_t workerThreadCount /* = -1*/)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if(workerThreadCount == DEFAULT_WORKER_THREAD_COUNT){
        errCode = LCT_TASK_PROCESSOR->init(std::thread::hardware_concurrency());
    } else {
        errCode = LCT_TASK_PROCESSOR->init(workerThreadCount);
    }

    if(LCT_SUCCESS != errCode){
        LOG_ERROR << "Failed to init Lct Task Processor! " << ErrCodeFormat(errCode);
        return errCode;
    } else {
        LOG_INFOR << "Successful to init LCT Event Processor";
    }

    return errCode;
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::start()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = LCT_TASK_PROCESSOR->start();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::shutdown()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = LCT_TASK_PROCESSOR->shutdown();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
    }

    return errCode;
}

/*******************************************************************************************
 *
 * Public Api
 *
 *******************************************************************************************/
template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::processEvent()
{
    LOG_DEBUG_MARK();

    CEventLockGuard lk(m_eventLock);
    return processEventImpl();
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::preprocess(const CDetailEvent& eventShp)
{
    LOG_DEBUG << "eventId(" << eventShp->EventId() << ")";

    CEventLockGuard lk(m_eventLock);
    return preprocessImpl(eventShp);
}


template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::scheduleEvent(const LctEventIdType& eventId)
{
    LOG_DEBUG << "eventId(" << eventId << ")";

    CEventLockGuard lk(m_eventLock);
    return ScheduleEventImpl(eventId);
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::startEvent(const LctEventIdType& eventId)
{
    LOG_DEBUG << "eventId(" << eventId << ")";

    CEventLockGuard lk(m_eventLock);
    return StartEventImpl(eventId);
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::finishEvent(const LctEventIdType& eventId)
{
    LOG_DEBUG << "eventId(" << eventId << ")";

    CEventLockGuard lk(m_eventLock);
    return FinishEventImpl(eventId);
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::cancelEvent(const LctEventIdType& eventId)
{
    LOG_DEBUG << "eventId(" << eventId << ")";

    CEventLockGuard lk(m_eventLock);
    return CancelEventImpl(eventId);
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::deleteEvent(const LctEventIdType& eventId)
{
    LOG_DEBUG << "eventId(" << eventId << ")";

    CEventLockGuard lk(m_eventLock);
    return DeleteEventImpl(eventId);
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::suspendEvent(const LctEventIdType& eventId)
{
    LOG_DEBUG << "eventId(" << eventId << ")";

    CEventLockGuard lk(m_eventLock);
    return SuspendEventImpl(eventId);
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::resumeEvent(const LctEventIdType& eventId)
{
    LOG_DEBUG << "eventId(" << eventId << ")";

    CEventLockGuard lk(m_eventLock);
    return ResumeEventImpl(eventId);
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::retrieveEvent(const LctEventIdType id, CDetailEvent& eventShp)
{
    auto& mmap = cplusplus14::get<std::map<LctEventIdType, CDetailEvent>>(m_eventsTuple);

    CEventLockGuard lk(m_eventLock);
    typename std::map<LctEventIdType, CDetailEvent>::const_iterator itRe = mmap.find(id);
    if(itRe == mmap.end()){
        LOG_ERROR << "Couldn't fine the right Event for the eventId(" << id << ") " << ErrCodeFormat(LCT_INVALID_PARAM);
        return LCT_INVALID_PARAM;
    }
    eventShp = itRe->second;

    return LCT_SUCCESS;
}

template <typename... CEventType>
template <typename Func, typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::iterateEvents(Func fn) const
{
    const auto& mmap = cplusplus14::get<std::map<LctEventIdType, CDetailEvent>>(m_eventsTuple);

    CEventLockGuard lk(m_eventLock);
    typename std::map<LctEventIdType, CDetailEvent>::const_iterator itRe = mmap.begin();
    while(itRe != mmap.end()){
        const CDetailEvent& ref = itRe->second;
        fn(ref);
        ++itRe;
    }

    return LCT_SUCCESS;
}

/*******************************************************************************************
 *
 * Below member functions are implementation of Public Api
 *
 *******************************************************************************************/

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::processEventImpl()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if(INVALID_EVENT_ID != m_currentEvent){
        LOG_DEBUG << "There is a running event(" << m_currentEvent << ")" << ErrCodeFormat(LCT_FAIL);
        return LCT_FAIL;
    }

    LctEventIdType eventId = INVALID_EVENT_ID;
    errCode = PopWaitingEvent(eventId);
    if(LCT_SUCCESS != errCode){
        if(LCT_EMPTY_RECORD == errCode){
             m_currentEvent = INVALID_EVENT_ID;
             LOG_DEBUG << "There is not any more event to handle";
             return LCT_SUCCESS;
        } else {
            LOG_WARNG << ErrCodeFormat(errCode);
            return errCode;
        }
    }

    m_currentEvent = eventId;
    errCode = ProcessEvent(eventId);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::preprocessImpl(const CDetailEvent& eventShp)
{
    auto& mmap = cplusplus14::get<std::map<LctEventIdType, CDetailEvent>>(m_eventsTuple);
    if(mmap.end() != mmap.find(eventShp->EventId())){
        LOG_ERROR << ErrCodeFormat(LCT_DUPLICATE_REQUEST);
        return LCT_DUPLICATE_REQUEST;
    }
    mmap[eventShp->EventId()] = eventShp;

    return LCT_SUCCESS;
}


template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::scheduleEventImpl(const LctEventIdType& eventId)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = ScheduleEvent(eventId, m_eventsTuple);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    ProcessEventImpl();

    return errCode;
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::startEventImpl(const LctEventIdType& eventId)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if(m_currentEvent != INVALID_EVENT_ID){
        errCode = SuspendEvent(m_currentEvent, m_eventsTuple);
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            return errCode;
        }

        errCode = ScheduleEventImpl(m_currentEvent);
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
        }
    }

    errCode = RemoveWaitingEvent(eventId);
    if(LCT_SUCCESS == errCode){
        LOG_WARNG << ErrCodeFormat(errCode);
        //return errCode;
    }

    m_currentEvent = eventId;
    errCode = StartEvent(eventId, m_eventsTuple);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::finishEventImpl(const LctEventIdType& eventId)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    
    errCode = FinishEvent(eventId, m_eventsTuple);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        // return errCode;
    }

    if(m_currentEvent == eventId){
        m_currentEvent = INVALID_EVENT_ID;
        errCode = ProcessEventImpl();
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            return errCode;
        }
    }

    return errCode;
}


template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::cancelEventImpl(const LctEventIdType& eventId)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = RemoveWaitingEvent(eventId);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CancelEvent(eventId, m_eventsTuple);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    if(m_currentEvent == eventId){
        m_currentEvent = INVALID_EVENT_ID;
        errCode = ProcessEventImpl();
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            return errCode;
        }
    }

    return errCode;
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::deleteEventImpl(const LctEventIdType& eventId)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = RemoveWaitingEvent(eventId);
    if(LCT_SUCCESS != errCode){
        LOG_DEBUG << ErrCodeFormat(errCode);
    }

    errCode = CancelEvent(eventId, m_eventsTuple);
    if(LCT_SUCCESS != errCode){
        LOG_DEBUG << ErrCodeFormat(errCode);
    }

    errCode = DeleteEvent(eventId, m_eventsTuple);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    if(m_currentEvent == eventId){
        m_currentEvent = INVALID_EVENT_ID;
        errCode = ProcessEventImpl();
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            return errCode;
        }
    }

    return errCode;
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::suspendEventImpl(const LctEventIdType& eventId)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = RemoveWaitingEvent(eventId);
    if(LCT_SUCCESS != errCode){
        LOG_WARNG << ErrCodeFormat(errCode);
        //return errCode;
    }

    errCode = SuspendEvent(eventId, m_eventsTuple);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    if(m_currentEvent == eventId){
        m_currentEvent = INVALID_EVENT_ID;
        errCode = ProcessEventImpl();
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            return errCode;
        }
    }

    return errCode;
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::resumeEventImpl(const LctEventIdType& eventId)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = ScheduleEventImpl(eventId);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = ResumeEvent(eventId, m_eventsTuple);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

/*******************************************************************************************
 *
 * Below member functions are all private
 *
 *******************************************************************************************/

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::processEvent(const CDetailEvent& eventShp)
{
    LOG_DEBUG << "eventId(" << eventShp->EventId() << ")";
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = eventShp->Start();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = LCT_TASK_PROCESSOR->Preprocess(eventShp);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::scheduleEvent(const CDetailEvent& eventShp)
{
    LOG_DEBUG << "eventId(" << eventShp->EventId() << ")";
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if((ELctTaskStatus::LCT_TASK_STATUS_INIT != eventShp->Status()) &&
            (ELctTaskStatus::LCT_TASK_STATUS_CANCELLED != eventShp->Status()) &&
            (ELctTaskStatus::LCT_TASK_STATUS_FAIL != eventShp->Status()) &&
            (ELctTaskStatus::LCT_TASK_STATUS_SUSPENDED != eventShp->Status())){

        LOG_ERROR << "eventId(" << eventShp->EventId() << ") status(" << (int16_t)eventShp->Status() << ") is not allowed to schedule event";
        return LCT_FAIL;
    }

    errCode = PushWaitingEvent(eventShp);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = eventShp->Schedule();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return LCT_SUCCESS;
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::startEvent(const CDetailEvent& eventShp)
{
    LOG_DEBUG << "eventId(" << eventShp->EventId() << ")";
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if((ELctTaskStatus::LCT_TASK_STATUS_INIT != eventShp->Status()) &&
            (ELctTaskStatus::LCT_TASK_STATUS_WAITING != eventShp->Status()) &&
            (ELctTaskStatus::LCT_TASK_STATUS_CANCELLED != eventShp->Status()) &&
            (ELctTaskStatus::LCT_TASK_STATUS_SUSPENDED != eventShp->Status())){
        LOG_ERROR << "eventId(" << eventShp->EventId() << ") status(" << (int16_t)eventShp->Status() << ") is not allowed to start event";
        return LCT_FAIL;
    }

    errCode = eventShp->Start();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }
    return LCT_TASK_PROCESSOR->Preprocess(eventShp);
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::finishEvent(const CDetailEvent& eventShp)
{
    return eventShp->Finish();
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::CancelEvent(const CDetailEvent& eventShp)
{
    if((ELctTaskStatus::LCT_TASK_STATUS_FAIL == eventShp->Status()) ||
            (ELctTaskStatus::LCT_TASK_STATUS_SUCCESS == eventShp->Status()) ||
            (ELctTaskStatus::LCT_TASK_STATUS_CANCELLED == eventShp->Status())){
        LOG_WARNG << "eventId(" << eventShp->EventId() << ") status(" << (int16_t)eventShp->Status() << ") is not allowed to cancel event";
        return LCT_FAIL;
    }

    return eventShp->Cancel();
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::suspendEvent(const CDetailEvent& eventShp)
{
    if((ELctTaskStatus::LCT_TASK_STATUS_FAIL == eventShp->Status()) ||
            (ELctTaskStatus::LCT_TASK_STATUS_SUCCESS == eventShp->Status()) ||
            (ELctTaskStatus::LCT_TASK_STATUS_CANCELLED == eventShp->Status())){
        LOG_ERROR << "eventId(" << eventShp->EventId() << ") status(" << (int16_t)eventShp->Status() << ") is not allowed to suspend event";
        return LCT_FAIL;
    }

    return eventShp->Suspend();
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::resumeEvent(const CDetailEvent& eventShp)
{
    if((ELctTaskStatus::LCT_TASK_STATUS_SUSPENDED != eventShp->Status())){
        LOG_ERROR << "eventId(" << eventShp->EventId() << ") status(" << (int16_t)eventShp->Status() << ") is not allowed to resume event";
        return LCT_FAIL;
    }

    return eventShp->Resume();
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::pushWaitingEvent(const CDetailEvent& eventShp)
{
    typename CWaitingEventBlockMap::iterator itMap = m_waitingEventBlockMap.find(eventShp->EventId());
    if(itMap != m_waitingEventBlockMap.end()){
        LOG_ERROR << "eventId(" << eventShp->EventId() << ")" << ErrCodeFormat(LCT_DUPLICATE_REQUEST);
        return LCT_DUPLICATE_REQUEST;
    }
    CWaitingEventBlock eventBlock(eventShp->EventId());
    m_waitingEventBlockMap.insert(std::pair<LctEventIdType, CWaitingEventBlock>(eventShp->EventId(), eventBlock));
    m_waitingTaskSet.emplace_back(eventBlock);

    LOG_DEBUG << "eventId(" << eventShp->EventId() << ") taskId(" << eventShp->TaskId() << ") m_waitingEventBlockMap.size("
            << m_waitingEventBlockMap.size() << ")"<< ClassName(eventShp);

    return LCT_SUCCESS;
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::popWaitingEvent(LctEventIdType& eventId)
{
    if(m_waitingTaskSet.empty()){
        LOG_WARNG << ErrCodeFormat(LCT_EMPTY_RECORD);
        return LCT_EMPTY_RECORD;
    }
    typename CWaitingTaskContainer::iterator it = m_waitingTaskSet.begin();
    const CWaitingEventBlock& eventBlock = *it;
    eventId = eventBlock.EventId;
    m_waitingTaskSet.erase(it);

    typename CWaitingEventBlockMap::iterator itMap = m_waitingEventBlockMap.find(eventId);
    if(itMap == m_waitingEventBlockMap.end()){
        LOG_ERROR << "Could't find eventId(" << eventId << ") in waiting list";
        return LCT_EMPTY_RECORD;
    }
    m_waitingEventBlockMap.erase(itMap);

    LOG_DEBUG << "successfu to pop eventId(" << eventId << ")";
    return LCT_SUCCESS;
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::removeWaitingEvent(const LctEventIdType& eventId)
{
    return RemoveWaitingEvent(eventId, m_eventsTuple);
}

template <typename... CEventType>
template <typename CDetailEvent>
LCT_ERR_CODE CLctEventManager<CEventType...>::removeWaitingEvent(const CDetailEvent& eventShp)
{
    LOG_DEBUG << "eventId(" << eventShp->EventId() << ") taskId(" << eventShp->TaskId() << ")";
    typename CWaitingEventBlockMap::iterator itMap = m_waitingEventBlockMap.find(eventShp->EventId());
    if(itMap == m_waitingEventBlockMap.end()){
        LOG_ERROR << ErrCodeFormat(LCT_EMPTY_RECORD);
        return LCT_EMPTY_RECORD;
    }

    const CWaitingEventBlock& eventBlock = itMap->second;
    {
        typename CWaitingTaskContainer::iterator it = std::find(m_waitingTaskSet.begin(), m_waitingTaskSet.end(), eventBlock);
        if(it == m_waitingTaskSet.end()){
            LOG_ERROR << ErrCodeFormat(LCT_EMPTY_RECORD);
            return LCT_EMPTY_RECORD;
        }
        m_waitingTaskSet.erase(it);
    }
    m_waitingEventBlockMap.erase(itMap);

    return LCT_SUCCESS;
}

template <typename... CEventType>
LCT_ERR_CODE CLctEventManager<CEventType...>::processEvent(const LctEventIdType eventId)
{
    return ProcessEvent(eventId, m_eventsTuple);;
}

template <typename... CEventType>
template <size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::processEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    return LCT_EMPTY_RECORD;
}

template <typename... CEventType>
template <size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::processEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    auto& mmap = std::get<I>(eventTuple);

    {
        auto itRe = mmap.find(eventId);
        if(itRe != mmap.end()){
            const auto& eventShp = itRe->second;
            return ProcessEvent(eventShp);
        }
    }

    return ProcessEvent<I+1>(eventId, eventTuple);
}


template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::scheduleEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    return LCT_EMPTY_RECORD;
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::scheduleEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    auto& mmap = std::get<I>(eventTuple);

    auto itRe = mmap.find(eventId);
    if(itRe != mmap.end()){
        const auto& eventShp = itRe->second;
        return ScheduleEvent(eventShp);
    }

    return ScheduleEvent<I+1>(eventId, eventTuple);
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::startEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    return LCT_EMPTY_RECORD;
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::startEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    auto& mmap = std::get<I>(eventTuple);

    {
        auto itRe = mmap.find(eventId);
        if(itRe != mmap.end()){
            const auto& eventShp = itRe->second;
            return StartEvent(eventShp);
        }
    }

    return StartEvent<I+1>(eventId, eventTuple);
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::finishEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    return LCT_EMPTY_RECORD;
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::finishEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    auto& mmap = std::get<I>(eventTuple);

    {
        auto itRe = mmap.find(eventId);
        if(itRe != mmap.end()){
            const auto& eventShp = itRe->second;
            return FinishEvent(eventShp);
        }
    }

    return FinishEvent<I+1>(eventId, eventTuple);
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::cancelEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    return LCT_EMPTY_RECORD;
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::cancelEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    auto& mmap = std::get<I>(eventTuple);

    {
        auto itRe = mmap.find(eventId);
        if(itRe != mmap.end()){
            const auto& eventShp = itRe->second;
            return CancelEvent(eventShp);
        }
    }

    return CancelEvent<I+1>(eventId, eventTuple);
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::deleteEvent(const LctEventIdType eventId, std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    return LCT_EMPTY_RECORD;
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::deleteEvent(const LctEventIdType eventId, std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    auto& mmap = std::get<I>(eventTuple);

    {
        auto itRe = mmap.find(eventId);
        if(itRe != mmap.end()){
            mmap.erase(itRe);
            return LCT_SUCCESS;
        }
    }

    return DeleteEvent<I+1>(eventId, eventTuple);
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::suspendEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    return LCT_EMPTY_RECORD;
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::suspendEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    auto& mmap = std::get<I>(eventTuple);

    {
        auto itRe = mmap.find(eventId);
        if(itRe != mmap.end()){
            const auto& eventShp = itRe->second;
            return SuspendEvent(eventShp);
        }
    }

    return SuspendEvent<I+1>(eventId, eventTuple);
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::resumeEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    return LCT_EMPTY_RECORD;
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::resumeEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    auto& mmap = std::get<I>(eventTuple);

    {
        auto itRe = mmap.find(eventId);
        if(itRe != mmap.end()){
            const auto& eventShp = itRe->second;
            return ResumeEvent(eventShp);
        }
    }

    return ResumeEvent<I+1>(eventId, eventTuple);
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::removeWaitingEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    return LCT_EMPTY_RECORD;
}

template <typename... CEventType>
template<size_t I, class ...CEventsType>
auto CLctEventManager<CEventType...>::removeWaitingEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type
{
    auto& mmap = std::get<I>(eventTuple);

    {
        auto itRe = mmap.find(eventId);
        if(itRe != mmap.end()){
            const auto& eventShp = itRe->second;
            return RemoveWaitingEvent(eventShp);
        }
    }

    return RemoveWaitingEvent<I+1>(eventId, eventTuple);
}

template <typename... CEventType>
CLctEventManager<CEventType...>::CWaitingEventBlock::CWaitingEventBlock(const LctEventIdType& eventId) noexcept(true):
    EventId(eventId),
    EventTime(CLctEventManager<CEventType...>::CLctEventTimeClock::now().time_since_epoch())
{
}

template <typename... CEventType>
CLctEventManager<CEventType...>::CWaitingEventBlock::CWaitingEventBlock(const CLctEventManager<CEventType...>::CWaitingEventBlock& that) noexcept(true):
    EventId(that.EventId),
    EventTime(that.EventTime)
{
}

template <typename... CEventType>
CLctEventManager<CEventType...>::CWaitingEventBlock::CWaitingEventBlock(CLctEventManager<CEventType...>::CWaitingEventBlock&& that) noexcept(true):
    EventId(that.EventId),
    EventTime(that.EventTime)
{
}

template <typename... CEventType>
typename CLctEventManager<CEventType...>::CWaitingEventBlock&
CLctEventManager<CEventType...>::CWaitingEventBlock::operator=(const CLctEventManager<CEventType...>::CWaitingEventBlock& that) noexcept(true)
{
    if(this == &that){
        return *this;
    }
    EventId            = that.EventId;
    EventTime          = that.EventTime;
    return *this;
}

template <typename... CEventType>
typename CLctEventManager<CEventType...>::CWaitingEventBlock&
CLctEventManager<CEventType...>::CWaitingEventBlock::operator=(CLctEventManager<CEventType...>::CWaitingEventBlock&& that) noexcept(true)
{
    if(this == &that){
        return *this;
    }
    EventId            = that.EventId;
    EventTime          = that.EventTime;
    return *this;
}

template <typename... CEventType>
bool CLctEventManager<CEventType...>::CWaitingEventBlock::operator==(const CLctEventManager<CEventType...>::CWaitingEventBlock& that) const noexcept(true)
{
    return (EventId == that.EventId) && (EventTime == that.EventTime);
}

// template <typename... CEventType>
// bool CLctEventManager<CEventType...>::CWaitingEventBlock::operator < (const CLctEventManager<CEventType...>::CWaitingEventBlock& that) const
// {
//    if(EventTime < that.EventTime){
//        return true;
//    }
//    if(EventTime > that.EventTime){
//        return false;
//    }
//    return EventId <= that.EventId;
// }


#endif /* SRC_EVENT_CLCTEVENTMANAGER_HPP_ */
