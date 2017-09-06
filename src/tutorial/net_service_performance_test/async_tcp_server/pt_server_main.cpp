/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        pt_server_main.cpp
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

int main()
{
    LCT_LOG->init("/tmp/log");

    CLctNetService service("127.0.0.1", 9090);

    LCT_ERR_CODE errCode = LCT_SUCCESS;
    {
        errCode = service.asyncAccept([&errCode, &service](const CLctSocket& socket) {
            LOG_DEBUG_MARK();
            LOG_DEBUG << "asyncAccept is activated and socketFd is " << socket.socketFd();
        });
    }

    std::atomic<int64_t> value{0};
    std::string dataBuff;
    errCode = service.asyncReadUntil(dataBuff, 'V', [&dataBuff, &value]() {
        LOG_DEBUG << "Message(" << dataBuff.c_str() << ") is received";
        value += std::stol(dataBuff);

    });

    std::cout << "before sleep, value is " << value << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(60));


    std::cout << "at the end, value is " << value << std::endl;

    LOG_DEBUG_MARK();

    return 0;
}



