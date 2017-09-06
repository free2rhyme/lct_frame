/**********************************************************************
 * @copyright   Copyright (C), 2017, Sensetime Tech. Co., Ltd.
 * @file        lct_job.hpp
 * @version     1.0
 * @date        Jun 16, 2017 4:07:17 PM
 * @author      liuyang@sensetime.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_EVENT_CLCTJOB_HPP_
#define SRC_EVENT_CLCTJOB_HPP_

#include "lct_cplusplus_14.h"
#include "lct_task_processor.h"
#include "lct_common_util.h"
#include "lct_log.h"

#ifdef RELEASE
    #define LOG_DEBUG_JOB()
#else
    #define LOG_DEBUG_JOB() do{ LOG_DEBUG << "eventId(" << CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::eventId() << ") JobId(" \
            << JobId() << ") "; } while(0)
#endif

template <typename CJobType, typename... CSubjobType>
void CLctJob<CJobType, CSubjobType...>::Process(const std::shared_ptr<CLctJob<CJobType, CSubjobType...>>& self)
{
    LOG_DEBUG_JOB();

    LCT_ERR_CODE errCode = LCT_SUCCESS;
    const ELctTaskStatus status = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::status();
    switch(status){
    case ELctTaskStatus::LCT_TASK_STATUS_RUNNING:{
        errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::executeTask();
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::demote();
        } else {
            CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::normalize();
        }

        if(CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::isRepeatedTask()){
            LCT_TASK_PROCESSOR->preprocess(self);
        }
        break;
    }
    case ELctTaskStatus::LCT_TASK_STATUS_WAITING:
    case ELctTaskStatus::LCT_TASK_STATUS_SUSPENDED:{
        break;
    }
    case ELctTaskStatus::LCT_TASK_STATUS_CANCELLED:{
        errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::CancelTask();
        break;
    }
    case ELctTaskStatus::LCT_TASK_STATUS_SUCCESS:{
        errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::finishTask();
        break;
    }
    default:
        break;
    }

    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
    }
}

template <typename CJobType, typename... CSubjobType>
LCT_ERR_CODE CLctJob<CJobType, CSubjobType...>::execute()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG_JOB();

    errCode = static_cast<CEventPointerType>(this)->execute();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    if(ELctTaskStatus::LCT_TASK_STATUS_RUNNING == CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::status()){
        CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::startSubTasks();
        CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::executeSubTasks();
    }

    return errCode;
}

template <typename CJobType, typename... CSubjobType>
LCT_ERR_CODE CLctJob<CJobType, CSubjobType...>::schedule()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG_JOB();

    return errCode;
}

template <typename CJobType, typename... CSubjobType>
LCT_ERR_CODE CLctJob<CJobType, CSubjobType...>::start()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG_JOB();

    errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::start();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CJobType, typename... CSubjobType>
LCT_ERR_CODE CLctJob<CJobType, CSubjobType...>::finish()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG_JOB();

    errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::finish();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CJobType, typename... CSubjobType>
LCT_ERR_CODE CLctJob<CJobType, CSubjobType...>::Cancel()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG_JOB();

    errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::cancel();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::cancelSubTasks();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CJobType, typename... CSubjobType>
LCT_ERR_CODE CLctJob<CJobType, CSubjobType...>::suspend()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG_JOB();

    errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::suspend();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::suspendSubTasks();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CJobType, typename... CSubjobType>
LCT_ERR_CODE CLctJob<CJobType, CSubjobType...>::Resume()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG_JOB();

    errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::Resume();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::suspendSubTasks();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CJobType, typename... CSubjobType>
LCT_ERR_CODE  CLctJob<CJobType, CSubjobType...>::Cleanup()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG_JOB();

    errCode = CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::RemoveSubTasks();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CJobType, typename... CSubjobType>
LctTaskIdType CLctJob<CJobType, CSubjobType...>::JobId() const
{
    return CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::TaskId();
}

template <typename CJobType, typename... CSubjobType>
LctEventIdType CLctJob<CJobType, CSubjobType...>::eventId() const{
    return CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>::eventId();
}

template <typename CJobType, typename... CSubjobType>
ELctProcessType CLctJob<CJobType, CSubjobType...>::ProcessType() const
{
    return ELctProcessType::LCT_PROCESS_TYPE_JOB;
}

template <typename CJobType, typename... CSubjobType>
CLctJob<CJobType, CSubjobType...>::CLctJob(const LctEventIdType& eventId,
        const LCTProcessPriorityType priority, bool repeated):
        CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>(eventId, priority, repeated)
{
}

template <typename CJobType, typename... CSubjobType>
CLctJob<CJobType, CSubjobType...>::~CLctJob()
{
    LOG_DEBUG_JOB();
}



#endif /* SRC_EVENT_CLCTJOB_HPP_ */
