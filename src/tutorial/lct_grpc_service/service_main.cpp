/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         LctGRpcServiceSample.cpp
 * @version      1.0
 * @date         May 17, 2017 9:15:24 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/

#include <grpc++/grpc++.h>
#include "lct_common_define.h"
#include "lct_common_util.h"
#include "hello_world.pb.h"
#include "hello_world.grpc.pb.h"
#include "lct_grpc_service_helper.h"

#include <thread>
#include <chrono>
#include <string>
#include "lct_log.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;


typedef lowcost::vip::helloworld::HelloRequest LctHelloRequest;
typedef lowcost::vip::helloworld::HelloReply   LctHelloReply;

typedef lowcost::vip::helloworld::Greeter      LctGreeter;

// Logic and data behind the server's behavior.
class LctGreeterServiceImpl final : public LctGreeter::Service {
    Status SayHello(ServerContext* context, const LctHelloRequest* request, LctHelloReply* reply) override {
        //std::cout << "Service: " << __PRETTY_FUNCTION__ << " is called " << std::endl;
        std::string prefix("Hello ");
        reply->set_message(prefix + request->name());

        LOG_DEBUG << "Server: " << LctCurrentDateTime()<< " " <<  std::this_thread::get_id() << " receive a rpc request and going to take a nap ";
        std::this_thread::sleep_for(std::chrono::seconds(4));
        LOG_DEBUG << "Server: " << LctCurrentDateTime()<< " " << std::this_thread::get_id() << " receive a rpc request and waken up ";


        return Status::OK;
    }
};

class LctGreeterService final:public CLctGRpcService<LctGreeterServiceImpl>{
public:
    LctGreeterService(const uint16_t port, const std::string& ip):CLctGRpcService<LctGreeterServiceImpl>(port, ip){
    }
    virtual ~LctGreeterService(){
    }
};


int main(int argc, char* argv[]){
    std::cout << std::numeric_limits<uint16_t>::max() << std::endl;
    uint16_t servicePort = 50051;
    if(argc > 1){
        servicePort = (uint16_t)std::stoi(argv[1]);
    }
    const std::string serviceIp        = "0.0.0.0";
    LctGreeterService instance(servicePort, serviceIp);
    instance.init();

    std::cout << "Server is going to listen on:" << serviceIp << ":" << servicePort << std::endl;
    instance.serve();

    return 0;
}







