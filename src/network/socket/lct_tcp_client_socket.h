/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_tcp_client_socket.h
 * @version     1.0
 * @date        Aug 16, 2017 1:47:44 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_TCP_CLIENT_SOCKET_H_
#define SRC_NETWORK_LCT_TCP_CLIENT_SOCKET_H_

#include "lct_tcp_socket.h"

class CLctTcpClientSocket final:public CLctTcpSocket
{
public:
    explicit CLctTcpClientSocket();
    virtual ~CLctTcpClientSocket();

public:
    LCT_ERR_CODE onStart();
    LCT_ERR_CODE onRead();
    LCT_ERR_CODE onWrite();
    LCT_ERR_CODE onStop();

public:
    LCT_ERR_CODE init(const int64_t localIp, const int16_t localPort);

    LCT_ERR_CODE connect();

    LCT_ERR_CODE processWriteEvent();

private:
    LCT_ERR_CODE processConnectEvent();
};

#endif /* SRC_NETWORK_LCT_TCP_CLIENT_SOCKET_H_ */
