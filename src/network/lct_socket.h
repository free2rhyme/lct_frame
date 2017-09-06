/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_socket.h
 * @version     1.0
 * @date        Aug 24, 2017 7:51:50 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_SOCKET_H_
#define SRC_NETWORK_LCT_SOCKET_H_

#include "detail/lct_network_common_define.h"

class CLctSocket final
{
public:
    explicit CLctSocket(const ELctSocketType) noexcept(true);
    explicit CLctSocket(const ELctSocketType, const int64_t localIp, const int16_t localPort) noexcept(true);

    CLctSocket(const CLctSocket&) noexcept(true);
    CLctSocket(CLctSocket&&) noexcept(true);

    CLctSocket& operator=(const CLctSocket&) noexcept(true);
    CLctSocket& operator=(CLctSocket&&) noexcept(true);

    ~CLctSocket();

public:
    int32_t socketFd() const;

    int64_t localIp()  const;
    int16_t localPort()const;

    int64_t peerIp()   const;
    int16_t peerPort() const;

    ELctSocketType  socketType()  const;
    ELctSocketState socketState() const;

    void setSocketFd(const int32_t fd);

    void setLocalIp(const int64_t ip);
    void setLocalPort(const int16_t port);

    void setPeerIp(const int64_t ip);
    void setPeerPort(const int64_t port);

    void setSocketState(const ELctSocketState state);

private:
    ELctSocketType  m_type     = ELctSocketType::SOCKET_TYPE_UNKNOWN;
    ELctSocketState m_state    = ELctSocketState::SOCKET_STATE_UNKNOWN;
    int16_t m_localPort        = LCT_INVALID_PORT;
    int16_t m_peerPort         = LCT_INVALID_PORT;
    int32_t m_socketFd         = LCT_INVALID_SOCKET_FD;
    int64_t m_localIp          = LCT_INVALID_IP;
    int64_t m_peerIp           = LCT_INVALID_IP;
};

#endif /* SRC_NETWORK_LCT_SOCKET_H_ */
