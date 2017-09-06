/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_grpc_client_helper.hpp
 * @version      1.0
 * @date         Jul 2, 2017 4:48:34 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_WRAPPER_GRPC_LCTGRPCCLIENTHELPER_HPP_
#define SRC_WRAPPER_GRPC_LCTGRPCCLIENTHELPER_HPP_

#include "lct_common_define.h"
#include "lct_log.h"
#include <grpc++/grpc++.h>

class CLctGRpcClientHelper
{
public:
    template <typename ClientType, typename ServiceCallable, class CRequestType, class CReplyType>
    static LCT_ERR_CODE CallService(const int32_t svcPort, const std::string& svcIp, ServiceCallable ServiceFunc, const CRequestType& request, CReplyType& reply){
        const std::string svcAddr = svcIp + ":" + std::to_string(svcPort);
        try{
            std::unique_ptr<typename ClientType::Stub> stub(
                ClientType::NewStub(grpc::CreateChannel(svcAddr, grpc::InsecureChannelCredentials())));

            grpc::ClientContext context;

            std::function<grpc::Status()> remoteCall = std::bind(ServiceFunc, std::ref(stub), &context, std::ref(request), &reply);

            grpc::Status status = remoteCall();

            if(status.ok()){
                LOG_DEBUG << "Successful to call remote service";
                return LCT_SUCCESS;
            }else{
                LOG_ERROR << "Failed to call remote service error(" << status.error_message() << ")";
                return LCT_FAIL;
            }
        } catch (const std::exception& e) {
            LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
            return LCT_UNEXPECTED;
        } catch (...) {
            LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
            return LCT_UNEXPECTED;
        }
        return LCT_SUCCESS;
    }

private:
    DISALLOW_INSTANTIATE(CLctGRpcClientHelper);
    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctGRpcClientHelper);
};

#endif /*SRC_WRAPPER_GRPC_LCTGRPCCLIENTHELPER_HPP_*/

