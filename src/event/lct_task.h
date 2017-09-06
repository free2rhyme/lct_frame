/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_task.h
 * @version      1.0
 * @date         May 24, 2017 12:05:50 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_EVENT_CLCTTASK_H_
#define SRC_EVENT_CLCTTASK_H_

#include <memory>
#include <map>
#include <tuple>
#include <atomic>
#include "lct_common_define.h"
#include "lct_task_id_mgr.h"
#include "lct_rw_lock.h"
#include "iva_log.h"

template <typename CTaskType, typename... CJobType>
class CLctTask
{
public:
    void Process(const std::shared_ptr<CLctTask>&);

public:
    LCT_ERR_CODE executeTask();
    LCT_ERR_CODE scheduleTask();
    LCT_ERR_CODE startTask();
    LCT_ERR_CODE cancelTask();
    LCT_ERR_CODE suspendTask();
    LCT_ERR_CODE resumeTask();
    LCT_ERR_CODE finishTask();
    LCT_ERR_CODE cleanupTask();
    LCT_ERR_CODE persistTask();

protected:
    LCT_ERR_CODE execute();
    LCT_ERR_CODE schedule();
    LCT_ERR_CODE start();
    LCT_ERR_CODE cancel();
    LCT_ERR_CODE suspend();
    LCT_ERR_CODE resume();
    LCT_ERR_CODE finish();
    LCT_ERR_CODE cleanup();
    LCT_ERR_CODE persist();

    LCT_ERR_CODE promote();
    LCT_ERR_CODE Demote();
    LCT_ERR_CODE Normalize();

    LCT_ERR_CODE executeSubTasks();
    LCT_ERR_CODE startSubTasks();
    LCT_ERR_CODE cancelSubTasks();
    LCT_ERR_CODE suspendSubTasks();
    LCT_ERR_CODE resumeSubTasks();
    LCT_ERR_CODE finishSubTasks();
    LCT_ERR_CODE removeSubTasks();

public:
    LctTaskIdType          taskId()          const;
    LctEventIdType         eventId()         const;
    ELctProcessType        processType()     const;
    ELctTaskStatus         status()          const;
    LCTProcessPriorityType currentPriority() const;

    bool isRepeatedTask()      const;

    int64_t creationTime() const;
    int64_t beginTime()    const;
    int64_t finishTime()   const;

    std::string creationTimeString() const;
    std::string beginTimeString()    const;
    std::string finishTimeString()   const;

protected:
    template <typename CDetailTask>
    LCT_ERR_CODE appendSubTask(const CDetailTask& jobShp);

    template <typename CDetailTask>
    LCT_ERR_CODE retrieveSubTask(const LctTaskIdType id, const CDetailTask& jobShp);

private:
    template<size_t I = 0, class ...CTasksType>
    static auto executeSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto executeSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto startSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto startSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto cancelSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto cancelSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto suspendSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto suspendSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto resumeSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto resumeSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto finishSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto finishSubTasks(const std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto removeSubTasks(std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I == sizeof...(CTasksType), void>::type;

    template<size_t I = 0, class ...CTasksType>
    static auto removeSubTasks(std::tuple<CTasksType...>& taskTuple)->typename std::enable_if<I < sizeof...(CTasksType), void>::type;

public:
    explicit CLctTask(const LctEventIdType& eventId, const LCTProcessPriorityType priority, bool repeated);
    explicit CLctTask(const LctEventIdType& eventId, const LCTProcessPriorityType priority, bool repeated,
        const int64_t& creationTime, const int64_t& beginExecTime, const int64_t& finishedExecTime);

    virtual ~CLctTask();

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctTask);

private:
    template<typename...CWhateverEventType>
    friend class CLctEventManager;

    template<typename...CWhateverEventType>
    friend class CLctTask<CTaskType, CJobType...>;

    friend CTaskType;

private:
    typedef CTaskType*        CEventPointerType;
    typedef typename std::tuple<std::map<LctTaskIdType, std::shared_ptr<CJobType>>...> CLctTupleContainer;

    CLctTupleContainer        m_subTaskTuple;

    typedef CLctRwlockGuard   CEventLockGuard;
    typedef lct_rw_lock        CEventLock;
    CEventLock                m_eventLock;

    std::atomic<LCTProcessPriorityType>  m_currentPriority;
    const LCTProcessPriorityType         m_standardPriority;
    ELctTaskStatus                       m_status;
    LctEventIdType                       m_eventId;
    LctTaskIdType                        m_taskId;
    std::atomic<bool>                    m_repeated;

    static constexpr const char* const DEFAULT_TIME_VALUE = "00:00:00";
    
    const int64_t         m_creationTime;
    int64_t               m_beginTime            = 0;
    int64_t               m_finishTime           = 0;
};

#include "detail/lct_task.hpp"

#endif /* SRC_EVENT_CLCTTASK_H_ */
