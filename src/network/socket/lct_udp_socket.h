/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_udp_socket.h
 * @version     1.0
 * @date        Aug 16, 2017 8:17:44 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_UDP_SOCKET_H_
#define SRC_NETWORK_LCT_UDP_SOCKET_H_

#include "lct_base_socket.h"

class CLctUdpSocket: public CLctBaseSocket
{
public:
    explicit CLctUdpSocket();
    virtual ~CLctUdpSocket();

public:
    virtual LCT_ERR_CODE processReadEvent() = 0;
    virtual LCT_ERR_CODE processWriteEvent() = 0;

    virtual LCT_ERR_CODE onRead() = 0;
    virtual LCT_ERR_CODE onWrite() = 0;
};

#endif /* SRC_NETWORK_LCT_UDP_SOCKET_H_ */
