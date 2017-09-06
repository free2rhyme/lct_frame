/**********************************************************************
 * @copyright    Copyright (C), 2017, Sensetime Tech. Co., Ltd.
 * @file         lct_task_id_mgr.cpp
 * @version      1.0
 * @date         Jul 1, 2017 10:45:08 PM
 * @author       liuyang@sensetime.com
 * @brief        TODO
 *********************************************************************/

#include "ServiceId.pb.h"
#include "ServiceId.grpc.pb.h"
#include "lct_task_id_mgr.h"
#include "lct_grpc_client_helper.hpp"
#include "lct_id_generator.h"
#include "lct_log.h"

#define UNIQUE_ID_DIGITS   (1000000)
#define BIZ_ID_DIGITS      (100)


LCT_ERR_CODE CLctTaskIdMgr::init(const int32_t idSvcPort, const std::string& idSvcIp)
{
    m_idSvcPort    = idSvcPort;
    m_idSvcIp      = idSvcIp;;
    m_isInitiated  = true;;

    return LCT_SUCCESS;
}

LctIdType CLctTaskIdMgr::fetchId(const CLctBizType& bizType) const
{
    LctIdType id = 0;
    if(LCT_SUCCESS == fetchId(bizType, id)){
        return id;
    }
    LOG_WARNG << "Failed to get id from remote servce, then get one from local memory!";
    return LCT_ID_GEN->getId(bizType);
}

LCT_ERR_CODE CLctTaskIdMgr::fetchId(const CLctBizType& bizType, LctIdType& id) const
{
    // LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if(!m_isInitiated){
        LOG_ERROR << "Task Id Manager is uninitiated!";
        return LCT_FAIL;
    }

    lct::CSidRequest  request;
    lct::CSidResponse reply;

    if(LCT_BIZ_EVENT_TYPE == bizType){
        request.set_seedid(LCT_EVENT_ENGINE_SEED_ID);
    } else if (LCT_BIZ_TASKS_TYPE == bizType){
        request.set_seedid(LCT_TASKS_ENGINE_SEED_ID);
    } else {
        return LCT_INVALID_PARAM;
    }

    errCode = CLctGRpcClientHelper::CallService<lct::ServiceLctUniqueId>(m_idSvcPort, m_idSvcIp, 
        &lct::ServiceLctUniqueId::Stub::FetchId, request, reply);
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    int64_t uniqueIdVal = reply.uniqueid();
    if(uniqueIdVal >= UNIQUE_ID_DIGITS ){
        LOG_ERROR << ErrCodeFormat(LCT_OUT_OF_BOUNDARY) << " uniqueIdVal(" << uniqueIdVal << ")";
        return LCT_OUT_OF_BOUNDARY;
    }

    try{
        errCode = reply.result().resultcode();
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << "Failed to fetch Id from remote service " << ErrCodeFormat(LCT_UNEXPECTED);
            return errCode;
        }

        int64_t dateVal = std::stol(reply.date());
        int64_t idValue = dateVal * BIZ_ID_DIGITS + (int64_t)bizType;
        id = idValue * UNIQUE_ID_DIGITS + uniqueIdVal;
    } catch (const std::exception& e) {
        LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
        return LCT_UNEXPECTED;
    } catch (...) {
        LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
        return LCT_UNEXPECTED;
    }

    return errCode;
}

CLctTaskIdMgr::CLctTaskIdMgr()
{
}

CLctTaskIdMgr::~CLctTaskIdMgr()
{
}
