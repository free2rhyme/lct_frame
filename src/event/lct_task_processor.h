/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_task_processor.h
 * @version      1.0
 * @date         May 23, 2017 7:40:58 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_EVENT_CLCTTASKPROCESSOR_H_
#define SRC_EVENT_CLCTTASKPROCESSOR_H_

#include <queue>
#include "lct_common_define.h"
#include "lct_singleton.h"
#include "lct_thread_pool.h"
#include "lct_event_common_define.h"

class CLctTaskProcessor final:public CLctSingleton<CLctTaskProcessor>
{
public:
    template <typename CTaskType>
    LCT_ERR_CODE process(const std::shared_ptr<CTaskType>&);

    template <typename CTaskType>
    LCT_ERR_CODE preprocess(const std::shared_ptr<CTaskType>& );

    LCT_ERR_CODE init(const int64_t workerThreadCount);
    LCT_ERR_CODE start();
    LCT_ERR_CODE shutdown();

    LCT_ERR_CODE run();

    int32_t workerCount() const
    {
        if(m_workerGroup == nullptr){
            return 0;
        }
        return m_workerGroup->size();
    }

private:
    friend class CLctSingleton;

    CLctTaskProcessor();
    ~CLctTaskProcessor();

private:
    typedef std::function<bool()> CLctTaskFunc;

    struct CPendingTaskBlock
    {
        ELctTaskStatus            TaskStatus;
        LCTProcessPriorityType    TaskPriority;
        CLctTaskFunc              TaskFunc;

        CPendingTaskBlock() noexcept(true);
        CPendingTaskBlock(const ELctTaskStatus stus, const LCTProcessPriorityType pri, const CLctTaskFunc& fn) noexcept(true);
        CPendingTaskBlock(const CPendingTaskBlock& that) noexcept(true);
        CPendingTaskBlock(CPendingTaskBlock&& that) noexcept(true);
        CPendingTaskBlock& operator=(const CPendingTaskBlock& that) noexcept(true);
        CPendingTaskBlock& operator=(CPendingTaskBlock&& that) noexcept(true);

        bool operator < (const CPendingTaskBlock& that) const;
    };

    typedef std::priority_queue<CPendingTaskBlock>    CTaskContainer;
    CTaskContainer                                    m_taskContainer;

    typedef std::mutex                                CTaskMutex;
    mutable CTaskMutex                                m_taskMutex;

    typedef std::condition_variable                   CTaskCond;
    mutable CTaskCond                                 m_taskCond;

private:
    bool                                        m_isRunning = false;
    CLctThreadpoolShp                           m_workerGroup;

    typedef std::thread                         CAsyncThreadType;
    typedef std::shared_ptr<CAsyncThreadType>   CAsyncThreadShp;
    CAsyncThreadShp                             m_asyncMigrateThread;
};

#define LCT_TASK_PROCESSOR CLctTaskProcessor::Instance()

#include "detail/lct_task_processor.hpp"

#endif /* SRC_EVENT_CLCTTASKPROCESSOR_H_ */
