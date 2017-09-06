/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_base_socket.cpp
 * @version     1.0
 * @date        Aug 16, 2017 1:28:23 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_base_socket.h"
#include "lct_log.h"
#include "event/lct_net_event_handler.h"
#include <algorithm>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>


CLctBaseSocket::CLctBaseSocket(const ELctSocketType type):
    m_socketInfo(type)
{
}

CLctBaseSocket::CLctBaseSocket(const ELctSocketType type, const int64_t localIp, const int16_t localPort):
    m_socketInfo(type, localIp, localPort)
{
}

CLctBaseSocket::~CLctBaseSocket()
{
}

int32_t CLctBaseSocket::getSocketFd() const
{
    return m_socketInfo.socketFd();
}

int64_t CLctBaseSocket::getLocalIp() const
{
    return m_socketInfo.localIp();
}

int16_t CLctBaseSocket::getLocalPort() const
{
    return m_socketInfo.localPort();
}

int64_t CLctBaseSocket::getPeerIp() const
{
    return m_socketInfo.peerIp();
}

int16_t CLctBaseSocket::getPeerPort() const
{
    return m_socketInfo.peerPort();
}

ELctSocketType CLctBaseSocket::getSocketType() const
{
    return m_socketInfo.socketType();
}

ELctSocketState CLctBaseSocket::getSocketState() const
{
    return m_socketInfo.socketState();
}

std::size_t CLctBaseSocket::getRecvBuffSize() const
{
    return nullptr == m_readBuffer ? 0 : m_readBuffer->size();
}

CLctSocket CLctBaseSocket::socketInfo() const
{
    return m_socketInfo;
}

CLctNetEventHandler* CLctBaseSocket::eventHandler() const
{
    return m_eventHandler;
}

void CLctBaseSocket::setSocketFd(const int32_t fd)
{
    m_socketInfo.setSocketFd(fd);
}

void CLctBaseSocket::setLocalIp(const int64_t ip)
{
    m_socketInfo.setLocalIp(ip);
}

void CLctBaseSocket::setLocalPort(const int16_t port)
{
    m_socketInfo.setLocalPort(port);
}

void CLctBaseSocket::setPeerIp(const int64_t ip)
{
    m_socketInfo.setPeerIp(ip);
}

void CLctBaseSocket::setPeerPort(const int64_t port)
{
    m_socketInfo.setPeerPort(port);
}

void CLctBaseSocket::setSocketState(const ELctSocketState state)
{
    m_socketInfo.setSocketState(state);
}

void CLctBaseSocket::setEventHandler(CLctNetEventHandler* ptr)
{
    m_eventHandler = ptr;
}

LCT_ERR_CODE CLctBaseSocket::initRecvBuffer(const std::size_t newBufferSize)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_readBuffer = std::make_shared<CLctSocketBuffer>(newBufferSize);

    return errCode;
}

LCT_ERR_CODE CLctBaseSocket::resetSocketRecvBuffSize(const std::size_t newBufferSize)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_readBuffer = std::make_shared<CLctSocketBuffer>(newBufferSize);

    return errCode;
}

LCT_ERR_CODE CLctBaseSocket::bindSocket(const int16_t localPort, const int64_t adapterAddr)
{
    struct sockaddr_in localAddress;

    const int32_t socketFd = getSocketFd();
    if (LCT_INVALID_SOCKET_FD == socketFd) {
        return LCT_INVALID_SOCKET;
    }

    std::fill_n((char*)&localAddress, sizeof(localAddress), 0);

    localAddress.sin_family      = AF_INET;
    localAddress.sin_port        = htons(localPort);
    localAddress.sin_addr.s_addr = htonl(adapterAddr);

    if (0 != bind(socketFd, (struct sockaddr *)&localAddress, sizeof(localAddress))) {
        LOG_ERROR<< "Failed to bind socket(" << socketFd << ") due to error(" << errno << ")";
        return LCT_BIND_SOCKET_FAIL;
    }

    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctBaseSocket::retrieveBoundInfo()
{
    struct sockaddr_in localAddress;
    socklen_t localAddressSize = sizeof(localAddress);

    const int32_t socketFd = getSocketFd();
    if (0 != getsockname(socketFd, (sockaddr *)&localAddress, &localAddressSize)) {
        LOG_ERROR<< "Failed to get info of socket(" << socketFd << ") due to error(" << errno << ")";
        return LCT_GET_SOCK_NAME_FAIL;
    }

    setLocalPort(ntohs(localAddress.sin_port));
    setLocalIp(htonl(localAddress.sin_addr.s_addr));

    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctBaseSocket::startService(const bool isAsyncMode/* = true*/)
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_asyncMode = isAsyncMode;

    errCode = onStart();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to handle on-start during starting service";
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctBaseSocket::stopService()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = onStop();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to handle on-stop during stopping service";
    }

    const int32_t socketFd = getSocketFd();
    if (LCT_INVALID_SOCKET_FD == socketFd) {
        ::close(socketFd);
        setSocketFd(LCT_INVALID_SOCKET_FD);
    }
    setSocketState(ELctSocketState::SOCKET_STATE_CLOSED);

    if (nullptr != m_eventHandler) {
        errCode = m_eventHandler->onStop();
        if (LCT_SUCCESS != errCode) {
            LOG_WARNG << "Failed to stop in event handler for socketFd(" << socketFd << ")";
            return errCode;
        }
    } else {
        LOG_ERROR << "eventHandler is nullptr for socketFd(" << socketFd << ")";
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctBaseSocket::doProcessRead()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = processReadEvent();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to process read event";
        return errCode;
    }

    errCode = onRead();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to handle on-read";
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctBaseSocket::doProcessWrite()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = processWriteEvent();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to process write event";
        return errCode;
    }

    errCode = onWrite();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to handle on-write";
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctBaseSocket::processRecvBuffData()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr != m_eventHandler) {
        errCode = m_eventHandler->processRecvBuffData(m_readBuffer);
        if (LCT_READ_EVENT_UNSET == errCode) {
            LOG_WARNG << "Failed to process recv buffer data in event handler due to unset event-handle";
            return errCode;
        } else {
            if (LCT_SUCCESS != errCode) {
               LOG_ERROR << "Failed to process recv buffer data in event handler";
               return errCode;
            }
        }
    } else {
        LOG_ERROR << "eventHandler is nullptr";
        return errCode;
    }

    return errCode;
}

