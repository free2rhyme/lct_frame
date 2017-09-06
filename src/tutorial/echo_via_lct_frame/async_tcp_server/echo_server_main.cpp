/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        echo_server_main.cpp
 * @version     1.0
 * @date        Aug 18, 2017 11:09:16 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include "lct_net_service.h"
#include "lct_socket.h"
#include "lct_log.h"

class CMessage;

class CLctNetAcceptorImpl
{
public:
    LCT_ERR_CODE doAccept()
    {
        LOG_DEBUG_MARK();
        return LCT_SUCCESS;
    }
};

class CLctNetServiceImpl
{
public:
    LCT_ERR_CODE doRead()
    {
        LOG_DEBUG_MARK();
        return LCT_SUCCESS;
    }

    LCT_ERR_CODE doWrite()
    {
        LOG_DEBUG_MARK();
        return LCT_SUCCESS;
    }
};

int main()
{
    LCT_LOG->init("/tmp/log");

    CLctNetService service("127.0.0.1", 9090);

    LCT_ERR_CODE errCode = LCT_SUCCESS;
    {
        errCode = service.asyncAccept([&errCode, &service](const CLctSocket& socket) {
            LOG_DEBUG_MARK();
            LOG_DEBUG << "asyncAccept is activated and socketFd is " << socket.socketFd();

            const std::string message("Typhoon is coming sensetime");
            errCode = service.asyncWrite(socket.socketFd(), message, []() {
                LOG_DEBUG_MARK();
            });
        });
    }

    std::string dataBuff;
    printf("Stream(%p), begin(%p)\n", &dataBuff, dataBuff.begin());

//    errCode = service.asyncReadUntil(dataBuff, 6, [&dataBuff]() {
//        LOG_DEBUG_MARK();
//
//        LOG_DEBUG << "Received dataBuff is " << dataBuff.c_str();
//    });

    errCode = service.asyncReadUntil(dataBuff, 'G', [&dataBuff]() {
        LOG_DEBUG_MARK();
        LOG_DEBUG << "Message(" << dataBuff.c_str() << ") is received";
    });


//   struct MatchCondition;
//   errCode = service.asyncRead(MatchCondition(), [](const CMessage& msg){
//       LOG_DEBUG << msg.data();
//       LOG_DEBUG_MARK();
//   }, [](){
//      LOG_DEBUG << "timeout";
//   });

    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to post a write request to net service" << ErrCodeFormat(errCode);
    }

    LOG_DEBUG_MARK();

    std::this_thread::sleep_for(std::chrono::seconds(5));

    {
        const std::string message("Hello sensetime");
        errCode = service.asyncWrite(message, []() {
            LOG_DEBUG_MARK();
        });
    }

    LOG_DEBUG_MARK();

    std::this_thread::sleep_for(std::chrono::seconds(130));


    LOG_DEBUG_MARK();

    return 0;
}
