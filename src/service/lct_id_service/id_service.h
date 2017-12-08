/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_service.h
 * @version     1.0
 * @date        Jun 30, 2017 7:22:19 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_SERVICE_LCT_ID_SERVICE_ID_SERVICE_H_
#define SRC_SERVICE_LCT_ID_SERVICE_ID_SERVICE_H_

#include <grpc++/grpc++.h>

#include "id_common_define.h"
#include "service_id.pb.h"
#include "service_id.grpc.pb.h"
#include "lct_grpc_service_helper.h"

class CSidServiceImpl final: public LctServiceLctUniqueId::Service
{
   GRpcStatus LoadSeed(GRpcServerContext* context, const CSidDummyReq* request, CSidDummyResp* response) override;
   GRpcStatus FetchId(GRpcServerContext* context, const LctCSidRequest* request, LctCSidResponse* response) override;
};

class CSidService final:public CLctGRpcService<CSidServiceImpl>{
public:
   CSidService(const int32_t port, const std::string& ip);
   ~CSidService();
};

typedef std::shared_ptr<CSidService> CSidServiceShp;

#endif /* SRC_SERVICE_LCT_ID_SERVICE_ID_SERVICE_H_ */
