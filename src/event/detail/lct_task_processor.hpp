/**********************************************************************
 * @copyright    Copyright (C), 2017, Sensetime Tech. Co., Ltd.
 * @file         lct_task_processor.hpp
 * @version      1.0
 * @date         May 24, 2017 6:52:33 PM
 * @author       liuyang@sensetime.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_EVENT_CLCTTASKPROCESSOR_HPP_
#define SRC_EVENT_CLCTTASKPROCESSOR_HPP_

#include "lct_log.h"

template <typename CTaskType>
LCT_ERR_CODE CLctTaskProcessor::process(const std::shared_ptr<CTaskType>& eventShp)
{
    LOG_DEBUG << "eventId(" << eventShp->EventId() << ") taskId(" << eventShp->taskId() << ")";

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = m_workerGroup->emplaceTask(&CTaskType::process, eventShp, eventShp);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
    }

    return errCode;
}

template <typename CTaskType>
LCT_ERR_CODE CLctTaskProcessor::preprocess(const std::shared_ptr<CTaskType>& eventShp)
{
    LOG_DEBUG << "eventId(" << eventShp->EventId() << ") taskId("
            << eventShp->taskId() << ") status(" << (int32_t)eventShp->status() << ") ProcessType("
            << int16_t(eventShp->processType()) << ")";

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    CLctTaskFunc fn = [eventShp, this](){
        bool deleteEventAfterProcess = true;
        const ELctTaskStatus status = eventShp->status();
        switch(status){
        case ELctTaskStatus::LCT_TASK_STATUS_RUNNING:
            this->process(eventShp);
            break;
        case ELctTaskStatus::LCT_TASK_STATUS_CANCELLED:
            deleteEventAfterProcess = true;
            break;
        case ELctTaskStatus::LCT_TASK_STATUS_WAITING:
        case ELctTaskStatus::LCT_TASK_STATUS_SUSPENDED:
            deleteEventAfterProcess = false;
            break;
        case ELctTaskStatus::LCT_TASK_STATUS_SUCCESS:
            deleteEventAfterProcess = true;
            break;
        default:
            break;
        }

        if(eventShp->processType() == ELctProcessType::LCT_PROCESS_TYPE_EVENT){
            deleteEventAfterProcess = (status != ELctTaskStatus::LCT_TASK_STATUS_INIT);
        } else if(eventShp->processType() != ELctProcessType::LCT_PROCESS_TYPE_JOB){
            deleteEventAfterProcess = true;
        }

        return deleteEventAfterProcess;
    };

    {
        CPendingTaskBlock taskBlock(eventShp->status(), eventShp->CurrentPriority(), fn);
        std::lock_guard<CTaskMutex> lk(m_taskMutex);
        m_taskContainer.push(taskBlock);
        m_taskCond.notify_all();
    }

    return errCode;
}


#endif /* SRC_EVENT_CLCTTASKPROCESSOR_HPP_ */
