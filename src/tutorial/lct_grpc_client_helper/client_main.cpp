/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         LctGRpcClientSample.cpp
 * @version      1.0
 * @date         Jul 2, 2017 5:15:24 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/


#include <iostream>
#include <memory>
#include <string>

#include "lct_grpc_client_helper.h"
#include "hello_world.pb.h"
#include "hello_world.grpc.pb.h"

typedef lowcost::vip::helloworld::HelloRequest LctHelloRequest;
typedef lowcost::vip::helloworld::HelloReply   LctHelloReply;

typedef lowcost::vip::helloworld::Greeter      LctGreeter;

int main(int argc, char** argv) {
   uint16_t port = 50051;
   std::string ip = "localhost";

   if(argc == 2){
      port = std::stoi(argv[1]);
   }else if(argc == 3){
      ip = std::string(argv[1]);
      port = std::stoi(argv[2]);
   }

   std::string user("world");
   LctHelloRequest request;
   request.set_name(user);

   LctHelloReply reply;

   CLctGRpcClientHelper::CallService<LctGreeter>(port, ip, &LctGreeter::Stub::SayHello, request, reply);

   std::cout << "Greeter received: " << reply.message() << std::endl;

   return 0;
}


