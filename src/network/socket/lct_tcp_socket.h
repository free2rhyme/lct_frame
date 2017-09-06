/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_tcp_socket.h
 * @version     1.0
 * @date        Aug 16, 2017 1:46:23 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_TCP_SOCKET_H_
#define SRC_NETWORK_LCT_TCP_SOCKET_H_

#include "lct_base_socket.h"

class CLctTcpSocket: public CLctBaseSocket
{
public:
    explicit CLctTcpSocket(const ELctSocketType type);
    explicit CLctTcpSocket(const ELctSocketType type, const int64_t ip, const int16_t port);
    virtual ~CLctTcpSocket();

public:
    virtual LCT_ERR_CODE send(const char* data, std::size_t size) override;

public:
    virtual LCT_ERR_CODE processReadEvent();
    virtual LCT_ERR_CODE processWriteEvent();

    virtual LCT_ERR_CODE onStart();
    virtual LCT_ERR_CODE onRead();
    virtual LCT_ERR_CODE onWrite();
    virtual LCT_ERR_CODE onStop();

private:
    LCT_ERR_CODE sendData(CLctSocketBufferShp& socketBufferShp);
    LCT_ERR_CODE sendData(const char* data, std::size_t size, std::size_t& offset);

private:
    int64_t m_msgOffset = 0;
};

#endif /* SRC_NETWORK_LCT_TCP_SOCKET_H_ */
