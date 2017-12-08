/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_service.cpp
 * @version     1.0
 * @date        Jun 30, 2017 7:22:42 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "id_service.h"

#include "id_manager.h"

GRpcStatus CSidServiceImpl::LoadSeed(GRpcServerContext* context, const CSidDummyReq* request, CSidDummyResp* response)
{
   if(nullptr == context){
      return GRpcStatus::CANCELLED;
   }
   if(nullptr == request){
      return GRpcStatus::CANCELLED;
   }
   if(nullptr == response){
      return GRpcStatus::CANCELLED;
   }

   LCT_ERR_CODE errCode = LCT_SUCCESS;

//    errCode = SID_ID_MGR->loadSeed();

   if(LCT_SUCCESS != errCode){
      LOG_ERROR << ErrCodeFormat(errCode);
   }

   CSidResult* result = new CSidResult();
   result->set_result_code(errCode);
   response->set_allocated_result(result);

   return GRpcStatus::OK;
}

GRpcStatus CSidServiceImpl::FetchId(GRpcServerContext* context, const LctCSidRequest* request, LctCSidResponse* response)
{
   if(nullptr == context){
      return GRpcStatus::CANCELLED;
   }
   if(nullptr == request){
      return GRpcStatus::CANCELLED;
   }
   if(nullptr == response){
      return GRpcStatus::CANCELLED;
   }

   LCT_ERR_CODE errCode = LCT_SUCCESS;

   errCode = SID_ID_MGR->fetchId(request->seedid(), *response);

   if(LCT_SUCCESS != errCode){
      LOG_ERROR << ErrCodeFormat(errCode);
   }

   CSidResult* result = new CSidResult();
   result->set_result_code(errCode);
   response->set_allocated_result(result);

   return GRpcStatus::OK;
}


CSidService::CSidService(const int32_t port, const std::string& ip):CLctGRpcService<CSidServiceImpl>(port, ip)
{
   LOG_DEBUG_MARK();
}

CSidService::~CSidService()
{
   LOG_DEBUG_MARK();
}
