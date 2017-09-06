/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_socket.cpp
 * @version     1.0
 * @date        Aug 24, 2017 7:52:32 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_socket.h"

CLctSocket::CLctSocket(const ELctSocketType type) noexcept(true):
    m_type(type)
{
}

CLctSocket::CLctSocket(const ELctSocketType type, const int64_t localIp, const int16_t localPort) noexcept(true):
    m_type(type),
    m_localPort(localPort),
    m_localIp(localIp)
{
}

CLctSocket::CLctSocket(const CLctSocket& that) noexcept(true):
    m_type(that.m_type),
    m_state(that.m_state),
    m_localPort(that.m_localPort),
    m_peerPort(that.m_peerPort),
    m_socketFd(that.m_socketFd),
    m_localIp(that.m_localIp),
    m_peerIp(that.m_peerIp)
{
}

CLctSocket::CLctSocket(CLctSocket&& that) noexcept(true):
    m_type(that.m_type),
    m_state(that.m_state),
    m_localPort(that.m_localPort),
    m_peerPort(that.m_peerPort),
    m_socketFd(that.m_socketFd),
    m_localIp(that.m_localIp),
    m_peerIp(that.m_peerIp)
{
}

CLctSocket& CLctSocket::operator=(const CLctSocket& that) noexcept(true)
{
    if (this == &that) {
        return *this;
    }
    m_type      = that.m_type;
    m_state     = that.m_state;
    m_localPort = that.m_localPort;
    m_peerPort  = that.m_peerPort;
    m_socketFd  = that.m_socketFd;
    m_localIp   = that.m_localIp;
    m_peerIp    = that.m_peerIp;

    return *this;
}

CLctSocket& CLctSocket::operator=(CLctSocket&& that) noexcept(true)
{
    if (this == &that) {
        return *this;
    }
    m_type      = that.m_type;
    m_state     = that.m_state;
    m_localPort = that.m_localPort;
    m_peerPort  = that.m_peerPort;
    m_socketFd  = that.m_socketFd;
    m_localIp   = that.m_localIp;
    m_peerIp    = that.m_peerIp;

    return *this;
}

CLctSocket::~CLctSocket()
{
}

int32_t CLctSocket::socketFd() const
{
    return m_socketFd;
}

int64_t CLctSocket::localIp() const
{
    return m_localIp;
}

int16_t CLctSocket::localPort() const
{
    return m_localPort;
}

int64_t CLctSocket::peerIp() const
{
    return m_peerIp;
}

int16_t CLctSocket::peerPort() const
{
    return m_peerPort;
}

ELctSocketType  CLctSocket::socketType() const
{
    return m_type;
}

ELctSocketState CLctSocket::socketState() const
{
    return m_state;
}

void CLctSocket::setSocketFd(const int32_t fd)
{
    m_socketFd = fd;
}

void CLctSocket::setLocalIp(const int64_t ip)
{
    m_localIp = ip;
}

void CLctSocket::setLocalPort(const int16_t port)
{
    m_localPort = port;
}

void CLctSocket::setPeerIp(const int64_t ip)
{
    m_peerIp = ip;
}

void CLctSocket::setPeerPort(const int64_t port)
{
    m_peerPort = port;
}

void CLctSocket::setSocketState(const ELctSocketState state)
{
    m_state = state;
}


