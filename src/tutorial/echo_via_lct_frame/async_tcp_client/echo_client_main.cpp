/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        echo_client_main.cpp
 * @version     1.0
 * @date        Aug 18, 2017 11:09:08 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_log.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include "lct_net_connect.h"

int main()
{
    LCT_LOG->init("/tmp/log");
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    CLctNetConnect client("127.0.0.1", 9090);

    const std::string message1("winterGisGcomingG");

    errCode = client.asyncWrite(message1, [message1]() {
        LOG_DEBUG << "message1(" << message1 << ") is sent";
    });

    const std::string message2("Games.of.ThroneG");
    errCode = client.asyncWrite(message2, [message2]() {
        LOG_DEBUG << "message2(" << message2 << ") is sent";
     });

    const std::string message3("YoU know Gnothing");
    errCode = client.asyncWrite(message3, [message3]() {
        LOG_DEBUG << "message3(" << message3 << ") is sent";
     });

    {
        std::string dataBuff;
        errCode = client.asyncReadUntil(dataBuff, 2000, [&dataBuff]() {
            LOG_DEBUG_MARK();
            LOG_DEBUG << dataBuff;
        });
    }

    //std::shared_ptr<std::string> dataBuff;
    std::string dataBuff;
    client.setSocketRecvBuffSize(1024);
    errCode = client.asyncReadUntil(dataBuff, '\r', [&dataBuff]() {
        LOG_DEBUG_MARK();
        LOG_DEBUG << dataBuff;
    });

    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to post a write request to net service" << ErrCodeFormat(errCode);
    }

    LOG_DEBUG_MARK();

    std::this_thread::sleep_for(std::chrono::seconds(30));

    LOG_DEBUG_MARK();

    return 0;
}

