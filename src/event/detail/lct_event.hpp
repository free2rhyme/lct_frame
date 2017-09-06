/**********************************************************************
 * @copyright    Copyright (C), 2017, Sensetime Tech. Co., Ltd.
 * @file         lct_event.hpp
 * @version      1.0
 * @date         May 24, 2017 11:24:35 AM
 * @author       liuyang@sensetime.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_EVENT_CLCTEVENT_HPP_
#define SRC_EVENT_CLCTEVENT_HPP_

#include "CLctTaskProcessor.h"
#include "LctLog.h"

template <typename CEventType, typename... CJobType>
void CLctEvent<CEventType, CJobType...>::process(const std::shared_ptr<CLctEvent<CEventType, CJobType...>>& self)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    const ELctTaskStatus status = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::status();
    switch(status){
    case ELctTaskStatus::LCT_TASK_STATUS_RUNNING:{
        errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::executeTask();
        break;
    }
    case ELctTaskStatus::LCT_TASK_STATUS_WAITING:
    case ELctTaskStatus::LCT_TASK_STATUS_SUSPENDED:{
        //errCode = LCT_TASK_PROCESSOR->Preprocess(self);
        break;
    }
    case ELctTaskStatus::LCT_TASK_STATUS_CANCELLED:{
        errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::cancelTask();
        break;
    }
    case ELctTaskStatus::LCT_TASK_STATUS_SUCCESS:{
        errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::FinishTask();
        break;
    }
    default:
        break;
    }

    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
    }
}


template <typename CEventType, typename... CJobType>
LCT_ERR_CODE CLctEvent<CEventType, CJobType...>::execute()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG << "eventId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::eventId()
            << ") taskId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::taskId() << ")" ;

    errCode = static_cast<CEventPointerType>(this)->Execute();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::startSubTasks();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::executeSubTasks();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}


template <typename CEventType, typename... CJobType>
LCT_ERR_CODE CLctEvent<CEventType, CJobType...>::schedule()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG << "eventId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::eventId()
            << ") taskId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::taskId() << ")" ;

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::schedule();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::persistTask();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CEventType, typename... CJobType>
LCT_ERR_CODE CLctEvent<CEventType, CJobType...>::start()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG << "eventId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::eventId()
            << ") taskId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::taskId() << ")" ;

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::start();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }


    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::persistTask();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CEventType, typename... CJobType>
LCT_ERR_CODE CLctEvent<CEventType, CJobType...>::Finish()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG << "eventId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::eventId()
            << ") taskId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::taskId() << ")" ;

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::Finish();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::persistTask();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::cleanup();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CEventType, typename... CJobType>
LCT_ERR_CODE CLctEvent<CEventType, CJobType...>::cancel()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG << "eventId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::eventId()
            << ") taskId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::taskId() << ")" ;

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::cancel();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::cancelSubTasks();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::persistTask();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::cleanup();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}


template <typename CEventType, typename... CJobType>
LCT_ERR_CODE CLctEvent<CEventType, CJobType...>::suspend()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG << "eventId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::eventId()
            << ") taskId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::taskId() << ")" ;

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::suspend();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::suspendSubTasks();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::persistTask();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

//    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::cleanup();
//    if(LCT_SUCCESS != errCode){
//        LOG_ERROR << ErrCodeFormat(errCode);
//        return errCode;
//    }

    return errCode;
}


template <typename CEventType, typename... CJobType>
LCT_ERR_CODE CLctEvent<CEventType, CJobType...>::Resume()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG << "eventId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::eventId()
            << ") taskId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::taskId() << ")" ;

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::schedule();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::ResumeSubTasks();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::persistTask();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }
    
    return errCode;
}

template <typename CEventType, typename... CJobType>
LCT_ERR_CODE CLctEvent<CEventType, CJobType...>::cleanup()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG << "eventId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::eventId()
            << ") taskId(" << CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::taskId() << ")" ;

    errCode = CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>::RemoveSubTasks();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CEventType, typename... CJobType>
LCT_ERR_CODE CLctEvent<CEventType, CJobType...>::persist()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = static_cast<CEventPointerType>(this)->Persistence();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }
    return errCode;
}

template <typename CEventType, typename... CJobType>
LCT_ERR_CODE CLctEvent<CEventType, CJobType...>::persistence() const
{
    return LCT_UNIMPLEMENTED;
}

template <typename CEventType, typename... CJobType>
std::string CLctEvent<CEventType, CJobType...>::eventComment() const
{
    return m_comment;
}

template <typename CEventType, typename... CJobType>
void CLctEvent<CEventType, CJobType...>::eventComment(const std::string& comment)
{
    m_comment = comment;
}

template <typename CEventType, typename... CJobType>
ELctProcessType CLctEvent<CEventType, CJobType...>::processType() const
{
    return ELctProcessType::LCT_PROCESS_TYPE_EVENT;
}

template <typename CEventType, typename... CJobType>
CLctEvent<CEventType, CJobType...>::CLctEvent( const LCTProcessPriorityType priority):
    CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>(LCT_TASK_ID_MGR->FetchId(LCT_BIZ_EVENT_TYPE), priority, false)
{
}

template <typename CEventType, typename... CJobType>
CLctEvent<CEventType, CJobType...>::CLctEvent(const LCTProcessPriorityType priority, const std::string& comment):
    CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>(LCT_TASK_ID_MGR->FetchId(LCT_BIZ_EVENT_TYPE), priority, false),
    m_comment(comment)
{
}

template <typename CEventType, typename... CJobType>
CLctEvent<CEventType, CJobType...>::CLctEvent(const LctEventIdType& eventId, const LCTProcessPriorityType priority, const std::string& comment,
    const int64_t& creationTime, const int64_t& beginexecTime, const int64_t& finishExecTime):
    CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>(eventId, priority, false, creationTime, beginexecTime, finishExecTime), 
    m_comment(comment)
{
}

template <typename CEventType, typename... CJobType>
CLctEvent<CEventType, CJobType...>::~CLctEvent()
{
}

#endif /* SRC_EVENT_CLCTEVENT_HPP_ */
