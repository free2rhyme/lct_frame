/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_tcp_server_socket.h
 * @version     1.0
 * @date        Aug 16, 2017 1:48:05 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_TCP_SERVER_SOCKET_H_
#define SRC_NETWORK_LCT_TCP_SERVER_SOCKET_H_

#include "lct_tcp_socket.h"

class CLctTcpServerSocket final:public CLctTcpSocket
{
public:
    explicit CLctTcpServerSocket();
    virtual ~CLctTcpServerSocket();

public:
    LCT_ERR_CODE onStart();
    LCT_ERR_CODE onRead();
    LCT_ERR_CODE onWrite();
    LCT_ERR_CODE onStop();

//   LCT_ERR_CODE accept(const int32_t listenerFd);
    LCT_ERR_CODE onAccept();
};

#endif /* SRC_NETWORK_LCT_TCP_SERVER_SOCKET_H_ */
