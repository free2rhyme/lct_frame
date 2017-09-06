/**********************************************************************
 * @copyright    Copyright (C), 2017, Sensetime Tech. Co., Ltd.
 * @file         lct_task_processor.cpp
 * @version      1.0
 * @date         May 24, 2017 11:25:34 AM
 * @author       liuyang@sensetime.com
 * @brief        TODO
 *********************************************************************/

#include "lct_task_processor.h"
#include "lct_log.h"

CLctTaskProcessor::CLctTaskProcessor():
    m_workerGroup(new CLctThreadpool())
{
}

CLctTaskProcessor::~CLctTaskProcessor()
{
    shutdown();
}

LCT_ERR_CODE CLctTaskProcessor::init(const int64_t workerThreadCount)
{

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = m_workerGroup->init(workerThreadCount);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << "Failed to init worker thread pool! " << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctTaskProcessor::start()
{

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = m_workerGroup->start();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << "Failed to start worker thread pool! " << ErrCodeFormat(errCode);
        return errCode;
    }

    m_isRunning = true;

    m_asyncMigrateThread.reset(new CAsyncThreadType(&CLctTaskProcessor::run, this));
    m_asyncMigrateThread->detach();

    return errCode;
}


LCT_ERR_CODE CLctTaskProcessor::shutdown()
{

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if(m_workerGroup != nullptr){
        errCode = m_workerGroup->shutdown();
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << "Failed to shutdown worker thread pool! " << ErrCodeFormat(errCode);
            return errCode;
        }

        m_workerGroup.reset();
    }

    return errCode;
}

LCT_ERR_CODE CLctTaskProcessor::run()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    while(m_isRunning){
        CPendingTaskBlock taskBlock;
        {
            std::unique_lock<CTaskMutex> lk(m_taskMutex);
            while(m_taskContainer.empty()){
                m_taskCond.wait(lk);
            }

            taskBlock = m_taskContainer.top();
            m_taskContainer.pop();
        }

        if(taskBlock.TaskFunc != nullptr){
            CLctTaskFunc taskRef = taskBlock.TaskFunc;
            bool isDelete = taskRef();
            if(!isDelete){
                std::lock_guard<CTaskMutex> lk(m_taskMutex);
                m_taskContainer.push(taskBlock);
            }
        } else {
            LOG_ERROR << "TaskBlock.TaskFunc is nullptr!!!";
        }
    }
    return LCT_SUCCESS;
}

CLctTaskProcessor::CPendingTaskBlock::CPendingTaskBlock() noexcept(true):
    TaskStatus(ELctTaskStatus::LCT_TASK_STATUS_UNKNOWN),
    TaskPriority(0),
    TaskFunc(nullptr)
{
}

CLctTaskProcessor::CPendingTaskBlock::CPendingTaskBlock(const ELctTaskStatus stus, const LCTProcessPriorityType pri, const CLctTaskFunc& fn) noexcept(true):
    TaskStatus(stus),
    TaskPriority(pri),
    TaskFunc(fn)
{
}

CLctTaskProcessor::CPendingTaskBlock::CPendingTaskBlock(const CLctTaskProcessor::CPendingTaskBlock& that) noexcept(true):
    TaskStatus(that.TaskStatus),
    TaskPriority(that.TaskPriority),
    TaskFunc(that.TaskFunc)
{
}

CLctTaskProcessor::CPendingTaskBlock::CPendingTaskBlock(CLctTaskProcessor::CPendingTaskBlock&& that) noexcept(true):
    TaskStatus(that.TaskStatus),
    TaskPriority(that.TaskPriority),
    TaskFunc(std::move(that.TaskFunc))
{
}

CLctTaskProcessor::CPendingTaskBlock& CLctTaskProcessor::CPendingTaskBlock::operator=(const CLctTaskProcessor::CPendingTaskBlock& that) noexcept(true)
{
    if(this == &that){
        return *this;
    }
    TaskStatus    = that.TaskStatus;
    TaskPriority  = that.TaskPriority;
    TaskFunc      = that.TaskFunc;

    return *this;
}

CLctTaskProcessor::CPendingTaskBlock& CLctTaskProcessor::CPendingTaskBlock::operator=(CLctTaskProcessor::CPendingTaskBlock&& that) noexcept(true)
{
    if(this == &that){
        return *this;
    }
    TaskStatus    = that.TaskStatus;
    TaskPriority  = that.TaskPriority;
    TaskFunc      = std::move(that.TaskFunc);

    return *this;
}

bool CLctTaskProcessor::CPendingTaskBlock::operator < (const CLctTaskProcessor::CPendingTaskBlock& that) const
{
    if((TaskStatus == ELctTaskStatus::LCT_TASK_STATUS_RUNNING) && (that.TaskStatus != ELctTaskStatus::LCT_TASK_STATUS_RUNNING)){
        return false;
    }

    if((TaskStatus != ELctTaskStatus::LCT_TASK_STATUS_RUNNING) && (that.TaskStatus == ELctTaskStatus::LCT_TASK_STATUS_RUNNING)){
        return true;
    }

    if(TaskPriority < that.TaskPriority)
    {
        return false;
    }
    return true;
}

