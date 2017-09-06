/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_tcp_listening_socket.h
 * @version     1.0
 * @date        Aug 16, 2017 1:28:47 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_TCP_LISTENING_SOCKET_H_
#define SRC_NETWORK_LCT_TCP_LISTENING_SOCKET_H_

#include "lct_tcp_socket.h"

class CLctTcpListeningSocket final:public CLctTcpSocket
{
public:
    explicit CLctTcpListeningSocket(const int64_t ip, const int16_t port);
    virtual ~CLctTcpListeningSocket();

public:
    LCT_ERR_CODE processReadEvent();
    LCT_ERR_CODE processWriteEvent();

    LCT_ERR_CODE onStart();
    LCT_ERR_CODE onRead();
    LCT_ERR_CODE onWrite();
    LCT_ERR_CODE onStop();

    LCT_ERR_CODE startListening(const int64_t adapterIp, const int16_t port);

private:
    LCT_ERR_CODE accept(CLctSocketShp& acceptedSocketShp);
    LCT_ERR_CODE stopListening();
};

#endif /* SRC_NETWORK_LCT_TCP_LISTENING_SOCKET_H_ */
