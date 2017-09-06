/**********************************************************************
 * @copyright    Copyright (C), 2017, Sensetime Tech. Co., Ltd.
 * @file         lct_task.hpp
 * @version      1.0
 * @date         May 24, 2017 12:08:45 PM
 * @author       liuyang@sensetime.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_EVENT_CLCTTASK_HPP_
#define SRC_EVENT_CLCTTASK_HPP_

#include "lct_cplusplus_14.h"
#include "lct_task_processor.h"
#include "lct_common_util.h"
#include "lct_log.h"

#ifdef RELEASE
    #define LOG_DEBUG_TASK()
#else
    #define  LOG_DEBUG_TASK() do{ LOG_DEBUG << "eventId(" << EventId() << ") taskId(" << TaskId() \
                 << ") m_status(" << (int16_t)m_status << ") m_repeated(" \
                 << m_repeated.load(std::memory_order_relaxed) << ")"; \
             }while(0)
#endif

template <typename CTaskType, typename... CJobType>
void CLctTask<CTaskType, CJobType...>::process(const std::shared_ptr<CLctTask<CTaskType, CJobType...>>& self)
{
    LOG_DEBUG_TASK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;
    switch(m_status){
    case ELctTaskStatus::LCT_TASK_STATUS_RUNNING:
    {
        errCode = Execute();

        if(LCT_SUCCESS == errCode){
            Normalize();
        } else {
            Demote();
        }

//        if(ProcessType() == ELctProcessType::LCT_PROCESS_TYPE_EVENT){
//            return;
//        }
//
//        if(ProcessType() != ELctProcessType::LCT_PROCESS_TYPE_TASK){
//            LOG_ERROR << "Unexpected ProcessType(" << (int16_t)ProcessType() << ")";
//            return;
//        }

        if(m_repeated.load(std::memory_order_seq_cst)){
            LCT_TASK_PROCESSOR->Preprocess(self);
        }
        break;
    }
    case ELctTaskStatus::LCT_TASK_STATUS_WAITING:
        break;
    case ELctTaskStatus::LCT_TASK_STATUS_CANCELLED:
        break;
    case ELctTaskStatus::LCT_TASK_STATUS_SUSPENDED:
    {
        Demote();
        LCT_TASK_PROCESSOR->Preprocess(self);
        break;
    }
    case ELctTaskStatus::LCT_TASK_STATUS_SUCCESS:
        break;
    default:
        break;
    }
}


/***************************************************************
*
* Interfaces
*
****************************************************************/
template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::executeTask()
{
    LOG_DEBUG_TASK();

    return static_cast<CEventPointerType>(this)->Execute();
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::scheduleTask()
{
    LOG_DEBUG_TASK();

    return static_cast<CEventPointerType>(this)->Schedule();
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::startTask()
{
    LOG_DEBUG_TASK();

    return static_cast<CEventPointerType>(this)->Start();
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::cancelTask()
{
    LOG_DEBUG_TASK();

    return static_cast<CEventPointerType>(this)->Cancel();
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::suspendTask()
{
    LOG_DEBUG_TASK();

    return static_cast<CEventPointerType>(this)->Suspend();
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::resumeTask()
{
    LOG_DEBUG_TASK();

    return static_cast<CEventPointerType>(this)->Resume();
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::finishTask()
{
    LOG_DEBUG_TASK();

    return static_cast<CEventPointerType>(this)->Finish();
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::cleanupTask()
{
    LOG_DEBUG_TASK();

    return static_cast<CEventPointerType>(this)->Cleanup();
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::persistTask()
{
    LOG_DEBUG_TASK();

    return static_cast<CEventPointerType>(this)->Persist();
}

/***************************************************************
*
* Base Behaviors
*
****************************************************************/
template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::execute()
{
    return LCT_UNIMPLEMENTED;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::schedule()
{
    LOG_DEBUG_TASK();

    m_status    = ELctTaskStatus::LCT_TASK_STATUS_WAITING;

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::start()
{
    LOG_DEBUG_TASK();

    m_status    = ELctTaskStatus::LCT_TASK_STATUS_RUNNING;

    m_beginTime = (m_beginTime != 0) ? m_beginTime:LctCurrentDateTimeValue();

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::cancel()
{
    LOG_DEBUG_TASK();

    m_status = ELctTaskStatus::LCT_TASK_STATUS_CANCELLED;

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::suspend()
{
    LOG_DEBUG_TASK();

    Demote();

    m_status = ELctTaskStatus::LCT_TASK_STATUS_SUSPENDED;

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::resume()
{
    LOG_DEBUG_TASK();

    Normalize();

    m_status = ELctTaskStatus::LCT_TASK_STATUS_WAITING;

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::finish()
{
    LOG_DEBUG_TASK();

    m_repeated.store(false, std::memory_order_seq_cst);

    m_status     = ELctTaskStatus::LCT_TASK_STATUS_SUCCESS;

    m_finishTime = (m_finishTime != 0) ? m_finishTime:LctCurrentDateTimeValue();
  
    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::cleanup()
{
    LOG_DEBUG_TASK();
    return RemoveSubTasks();
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::persist()
{
    LOG_DEBUG_TASK();
    return LCT_UNIMPLEMENTED;
}

/***************************************************************
*
* Priority Management
*
****************************************************************/
template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::promote()
{
    if(m_currentPriority.fetch_sub(1) <= LCT_PROCESS_PRIORITY_HIGH){
        return LCT_SUCCESS;
    } else {
        m_currentPriority.fetch_add(1);
        return LCT_FAIL;
    }
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::demote()
{
    if(m_currentPriority.fetch_add(1) >= LCT_PROCESS_PRIORITY_LOW){
        return LCT_SUCCESS;
    } else {
        m_currentPriority.fetch_sub(1);
        return LCT_FAIL;
    }
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::normalize()
{
    if(m_currentPriority.load(std::memory_order_seq_cst) != m_standardPriority){
        m_currentPriority.store(m_standardPriority, std::memory_order_seq_cst);
    }
    return LCT_SUCCESS;
}


/***************************************************************
*
* Sub Tasks Management
*
****************************************************************/
template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::executeSubTasks()
{
    ExecuteSubTasks(m_subTaskTuple);

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::startSubTasks()
{
    StartSubTasks(m_subTaskTuple);

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::cancelSubTasks()
{
    CancelSubTasks(m_subTaskTuple);

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::suspendSubTasks()
{
    SuspendSubTasks(m_subTaskTuple);

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::resumeSubTasks()
{
    ResumeSubTasks(m_subTaskTuple);

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::finishSubTasks()
{
    FinishSubTasks(m_subTaskTuple);

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::removeSubTasks()
{
    RemoveSubTasks(m_subTaskTuple);

    return LCT_SUCCESS;
}

/***************************************************************
*
* Task information
*
****************************************************************/
template <typename CTaskType, typename... CJobType>
LctTaskIdType CLctTask<CTaskType, CJobType...>::TaskId() const
{
    return m_taskId;
}

template <typename CTaskType, typename... CJobType>
LctEventIdType CLctTask<CTaskType, CJobType...>::eventId() const{
    return m_eventId;
}

template <typename CTaskType, typename... CJobType>
ELctProcessType CLctTask<CTaskType, CJobType...>::processType() const
{
    return static_cast<const CTaskType*>(this)->ProcessType();;
}

template <typename CTaskType, typename... CJobType>
ELctTaskStatus CLctTask<CTaskType, CJobType...>::status() const
{
    return m_status;
}

template <typename CTaskType, typename... CJobType>
LCTProcessPriorityType CLctTask<CTaskType, CJobType...>::currentPriority() const
{
    return m_currentPriority.load(std::memory_order_seq_cst);
}

template <typename CTaskType, typename... CJobType>
bool CLctTask<CTaskType, CJobType...>::isRepeatedTask() const
{
    return m_repeated.load(std::memory_order_seq_cst);
}

template <typename CTaskType, typename... CJobType>
int64_t CLctTask<CTaskType, CJobType...>::creationTime() const
{
    return m_creationTime;
}

template <typename CTaskType, typename... CJobType>
int64_t CLctTask<CTaskType, CJobType...>::beginTime() const
{
    return m_beginTime;
}

template <typename CTaskType, typename... CJobType>
int64_t CLctTask<CTaskType, CJobType...>::finishTime() const
{
    return m_finishTime;
}

template <typename CTaskType, typename... CJobType>
std::string CLctTask<CTaskType, CJobType...>::creationTimeString() const
{
    if(m_creationTime == 0){
        return DEFAULT_TIME_VALUE;
    }
    return LctFormatTime(m_creationTime);
}

template <typename CTaskType, typename... CJobType>
std::string CLctTask<CTaskType, CJobType...>::beginTimeString()    const
{
    if(m_beginTime == 0){
        return DEFAULT_TIME_VALUE;
    }
    return LctFormatTime(m_beginTime);
}

template <typename CTaskType, typename... CJobType>
std::string CLctTask<CTaskType, CJobType...>::finishTimeString()   const
{
    if(m_finishTime == 0){
        return DEFAULT_TIME_VALUE;
    }
    return LctFormatTime(m_finishTime);
}

/*****************************************************************************
*
* Through static polymorphism, implementation for sub tasks management
*
*****************************************************************************/
template <typename CTaskType, typename... CJobType>
template <typename CDetailTask>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::appendSubTask(const CDetailTask& jobShp)
{
    LOG_DEBUG << "eventId(" << jobShp->EventId() << ") taskId(" << jobShp->TaskId() << ") " << ClassName(jobShp);

    auto& mmap = cplusplus14::get<std::map<LctTaskIdType, CDetailTask>>(m_subTaskTuple);

    CEventLockGuard lk(m_eventLock, true);
    mmap[jobShp->TaskId()] = jobShp;

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
template <typename CDetailTask>
LCT_ERR_CODE CLctTask<CTaskType, CJobType...>::retrieveSubTask(const LctTaskIdType id, const CDetailTask& jobShp)
{
    LOG_DEBUG_MARK();

    auto& mmap = cplusplus14::get<std::map<LctTaskIdType, CDetailTask>>(m_subTaskTuple);

    CEventLockGuard lk(m_eventLock);
    typename std::map<LctTaskIdType, CDetailTask>::const_iterator itRe = mmap.find(id);
    if(itRe == mmap.end()){
        LOG_ERROR << "Couldn't fine the right Task for the taskId(" << id << ") " << ErrCodeFormat(LCT_INVALID_PARAM);
        return LCT_INVALID_PARAM;
    }
    jobShp = itRe->second;

    return LCT_SUCCESS;
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::executeSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type
{
    return;
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::executeSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type
{
    auto& mmap = std::get<I>(taskTuple);

    for(auto itRe = mmap.begin(); itRe != mmap.end(); ++itRe){
        const auto& ref = itRe->second;
        if(ref != nullptr){
            LCT_TASK_PROCESSOR->Process(ref);
        }
    }
    ExecuteSubTasks<I+1>(taskTuple);
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::startSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type
{
    return;
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::startSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type
{
    auto& mmap = std::get<I>(taskTuple);

    for(auto itRe = mmap.begin(); itRe != mmap.end(); ++itRe){
        const auto& ref = itRe->second;
        if(ref != nullptr){
              ref->Start();
        }
    }
    StartSubTasks<I+1>(taskTuple);
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::cancelSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type
{
    return;
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::cancelSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type
{
    auto& mmap = std::get<I>(taskTuple);

    for(auto itRe = mmap.begin(); itRe != mmap.end(); ++itRe){
            const auto& ref = itRe->second;
            if(ref != nullptr){
                ref->Cancel();
        }
    }
    CancelSubTasks<I+1>(taskTuple);
}

template <typename CTaskType, typename... CJobType>
template <size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::suspendSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type
{
    return;
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::suspendSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type
{
    auto& mmap = std::get<I>(taskTuple);

    for(auto itRe = mmap.begin(); itRe != mmap.end(); ++itRe){
        const auto& ref = itRe->second;
        if(ref != nullptr){
            ref->Suspend();
        }
    }
    SuspendSubTasks<I+1>(taskTuple);
}


template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::resumeSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type
{
    return;
}


template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::resumeSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type
{
    auto& mmap = std::get<I>(taskTuple);

    for(auto itRe = mmap.begin(); itRe != mmap.end(); ++itRe){
        const auto& ref = itRe->second;
           if(ref != nullptr){
            ref->ResumeTask();
        }
    }

    ResumeSubTasks<I+1>(taskTuple);
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::finishSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type
{
    return;
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::finishSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type
{
    auto& mmap = std::get<I>(taskTuple);

    for(auto itRe = mmap.begin(); itRe != mmap.end(); ++itRe){
        const auto& ref = itRe->second;
           if(ref != nullptr){
            ref->FinishTask();
        }
    }

    FinishSubTasks<I+1>(taskTuple);
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::removeSubTasks(std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type
{
    return;
}

template <typename CTaskType, typename... CJobType>
template<size_t I, class ...CTasksType>
auto CLctTask<CTaskType, CJobType...>::removeSubTasks(std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type
{
    auto& mmap = std::get<I>(taskTuple);

    for(auto itRe = mmap.begin(); itRe != mmap.end(); ++itRe){
        const auto& ref = itRe->second;
           if(ref != nullptr){
            ref->Cleanup();
        }
    }

    mmap.erase(mmap.begin(), mmap.end());

    RemoveSubTasks<I+1>(taskTuple);
}

template <typename CTaskType, typename... CJobType>
CLctTask<CTaskType, CJobType...>::CLctTask(const LctEventIdType& eventId, const LCTProcessPriorityType priority, bool repeated):
    m_currentPriority(priority),
    m_standardPriority(priority),
    m_repeated(repeated),
    m_status(ELctTaskStatus::LCT_TASK_STATUS_INIT),
    m_eventId(eventId),
    m_taskId(LCT_TASK_ID_MGR->FetchId(LCT_BIZ_TASKS_TYPE)),
    m_creationTime(LctCurrentDateTimeValue())
{
}

template <typename CTaskType, typename... CJobType>
CLctTask<CTaskType, CJobType...>::CLctTask(const LctEventIdType& eventId, const LCTProcessPriorityType priority, bool repeated,
    const int64_t& creationTime, const int64_t& beginExecTime, const int64_t& finishedExecTime):
    m_currentPriority(priority),
    m_standardPriority(priority),
    m_repeated(repeated),
    m_status(ELctTaskStatus::LCT_TASK_STATUS_INIT),
    m_eventId(eventId),
    m_taskId(LCT_TASK_ID_MGR->FetchId(LCT_BIZ_TASKS_TYPE)),
    m_creationTime(creationTime),
    m_beginTime(beginExecTime),
    m_finishTime(finishedExecTime)
{
}

template <typename CTaskType, typename... CJobType>
CLctTask<CTaskType, CJobType...>::~CLctTask()
{
    LOG_DEBUG_TASK();
}


#endif /* SRC_EVENT_CLCTTASK_HPP_ */
