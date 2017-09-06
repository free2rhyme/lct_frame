/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_event_manager.h
 * @version     1.0
 * @date        Jun 13, 2017 10:59:06 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_EVENT_CLCTEVENTMANAGER_H_
#define SRC_EVENT_CLCTEVENTMANAGER_H_

#include "lct_singleton.h"
#include <mutex>
#include <tuple>
#include <chrono>
#include <vector>
#include <map>

template <typename... CEventType>
class CLctEventManager final: public CLctSingleton<CLctEventManager<CEventType...>>
{
    static constexpr int64_t DEFAULT_WORKER_THREAD_COUNT = -1;
    
public:
    LCT_ERR_CODE init(const int64_t workerThreadCount = DEFAULT_WORKER_THREAD_COUNT);

    LCT_ERR_CODE start();

    LCT_ERR_CODE shutdown();

public:
    LCT_ERR_CODE processEvent();

    template <typename CDetailEvent>
    LCT_ERR_CODE preprocess(const CDetailEvent& eventShp);

    LCT_ERR_CODE scheduleEvent(const LctEventIdType& eventId);

    LCT_ERR_CODE startEvent(const LctEventIdType& eventId);

    LCT_ERR_CODE finishEvent(const LctEventIdType& eventId);

    LCT_ERR_CODE cancelEvent(const LctEventIdType& eventId);

    LCT_ERR_CODE deleteEvent(const LctEventIdType& eventId);

    LCT_ERR_CODE suspendEvent(const LctEventIdType& eventId);

    LCT_ERR_CODE resumeEvent(const LctEventIdType& eventId);

    template <typename CDetailEvent>
    LCT_ERR_CODE retrieveEvent(const LctEventIdType id, CDetailEvent& eventShp);

    template <typename Func, typename CDetailEvent>
    LCT_ERR_CODE iterateEvents(Func fn) const;

private:
    LCT_ERR_CODE processEventImpl();

    template <typename CDetailEvent>
    LCT_ERR_CODE preprocessImpl(const CDetailEvent& eventShp);

    LCT_ERR_CODE scheduleEventImpl(const LctEventIdType& eventId);

    LCT_ERR_CODE startEventImpl(const LctEventIdType& eventId);

    LCT_ERR_CODE finishEventImpl(const LctEventIdType& eventId);

    LCT_ERR_CODE cancelEventImpl(const LctEventIdType& eventId);

    LCT_ERR_CODE deleteEventImpl(const LctEventIdType& eventId);

    LCT_ERR_CODE suspendEventImpl(const LctEventIdType& eventId);

    LCT_ERR_CODE resumeEventImpl(const LctEventIdType& eventId);

private:
    template <typename CDetailEvent>
    LCT_ERR_CODE processEvent(const CDetailEvent& eventShp);

    template <typename CDetailEvent>
    LCT_ERR_CODE scheduleEvent(const CDetailEvent& eventShp);

    template <typename CDetailEvent>
    LCT_ERR_CODE startEvent(const CDetailEvent& eventShp);

    template <typename CDetailEvent>
    LCT_ERR_CODE finishEvent(const CDetailEvent& eventShp);

    template <typename CDetailEvent>
    LCT_ERR_CODE cancelEvent(const CDetailEvent& eventShp);

    template <typename CDetailEvent>
    LCT_ERR_CODE suspendEvent(const CDetailEvent& eventShp);

    template <typename CDetailEvent>
    LCT_ERR_CODE resumeEvent(const CDetailEvent& eventShp);

    template <typename CDetailEvent>
    LCT_ERR_CODE PushWaitingEvent(const CDetailEvent& eventShp);

    LCT_ERR_CODE PopWaitingEvent(LctEventIdType& eventId);

    LCT_ERR_CODE RemoveWaitingEvent(const LctEventIdType& eventId);

    template <typename CDetailEvent>
    LCT_ERR_CODE RemoveWaitingEvent(const CDetailEvent& eventShp);

    LCT_ERR_CODE ProcessEvent(const LctEventIdType eventId);

private:

    template<size_t I = 0, class ...CEventsType>
    auto processEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto processEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto scheduleEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto scheduleEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto startEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto startEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto finishEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto finishEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto cancelEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto cancelEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type;
  
    template<size_t I = 0, class ...CEventsType>
    auto deleteEvent(const LctEventIdType eventId, std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto deleteEvent(const LctEventIdType eventId, std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto suspendEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto suspendEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto resumeEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto resumeEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto removeWaitingEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I == sizeof...(CEventsType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class ...CEventsType>
    auto removeWaitingEvent(const LctEventIdType eventId, const std::tuple<CEventsType...>& eventTuple)->typename std::enable_if<I < sizeof...(CEventsType), LCT_ERR_CODE>::type;

private:
    friend class CLctSingleton<CLctEventManager<CEventType...>>;
    CLctEventManager()
    {
    }

    ~CLctEventManager()
    {
    }

private:
    typedef std::chrono::system_clock                    CLctEventTimeClock;
    typedef std::chrono::time_point<CLctEventTimeClock>  CLctEventTimeStamp;

    struct CWaitingEventBlock
    {
        LctEventIdType                        EventId;
        CLctEventTimeStamp                    EventTime;

        explicit CWaitingEventBlock(const LctEventIdType& eventId) noexcept(true);
        CWaitingEventBlock(const CWaitingEventBlock& that) noexcept(true);
        CWaitingEventBlock(CWaitingEventBlock&& that) noexcept(true);

        CWaitingEventBlock& operator=(const CWaitingEventBlock& that) noexcept(true);
        CWaitingEventBlock& operator=(CWaitingEventBlock&& that) noexcept(true);
        bool operator==(const CWaitingEventBlock& that) const noexcept(true);
        // bool operator < (const CWaitingEventBlock& that) const;
    };

    typedef std::vector<CWaitingEventBlock>   CWaitingTaskContainer;
    CWaitingTaskContainer                     m_waitingTaskSet;

    LctEventIdType                            m_currentEvent = INVALID_EVENT_ID;
  
    typedef std::map<LctEventIdType, CWaitingEventBlock> CWaitingEventBlockMap;
    CWaitingEventBlockMap                     m_waitingEventBlockMap;


    typedef typename std::tuple<std::map<LctEventIdType, std::shared_ptr<CEventType>>...> CLctTupleContainer;
    CLctTupleContainer          m_eventsTuple;

    typedef std::mutex                        CEventLock;
    typedef std::lock_guard<CEventLock>       CEventLockGuard;
    mutable CEventLock                        m_eventLock;
};

#include "detail/lct_event_manager.hpp"

#endif /* SRC_EVENT_CLCTEVENTMANAGER_H_ */
