/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_tcp_server_socket.cpp
 * @version     1.0
 * @date        Aug 16, 2017 1:49:14 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_tcp_server_socket.h"
#include "lct_socket_util.h"
#include "lct_net_service.h"
#include "event/lct_net_event_handler.h"
#include "lct_log.h"

CLctTcpServerSocket::CLctTcpServerSocket():
    CLctTcpSocket(ELctSocketType::SOCKET_TYPE_TCP_SERVER)
{
    LOG_DEBUG_MARK();
}

CLctTcpServerSocket::~CLctTcpServerSocket()
{
    LOG_DEBUG_MARK();
}

LCT_ERR_CODE CLctTcpServerSocket::onStart()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = onAccept();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to handle on-accept for socketFd(" << getSocketFd() << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    std::size_t revBuffSize = DEFAULT_RECV_BUFFER_SIZE;
    if (nullptr != m_eventHandler) {
        revBuffSize = m_eventHandler->socketRecvBuffSize();
    }

    errCode = initRecvBuffer(revBuffSize);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to create recv-buffer for socketFd(" << getSocketFd() << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctTcpServerSocket::onRead()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr != m_eventHandler) {
        errCode = m_eventHandler->doReadCompletionHandle();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to process read event for socketFd(" << getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
    } else {
        LOG_ERROR << "Event Handler is null for socketFd(" << getSocketFd() << ")";
    }

    return errCode;
}

LCT_ERR_CODE CLctTcpServerSocket::onWrite()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr != m_eventHandler) {
        errCode = m_eventHandler->doWriteCompletionHandle();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to process write event for socketFd(" << getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
    } else {
        LOG_ERROR << "Event Handler is null for socketFd(" << getSocketFd() << ")";
    }

    return errCode;
}

LCT_ERR_CODE CLctTcpServerSocket::onStop()
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctTcpServerSocket::onAccept()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = retrieveBoundInfo();
    if (LCT_SUCCESS != errCode) {
        stopService();
        return errCode;
    }

    setSocketState(ELctSocketState::SOCKET_STATE_CONNECTED);

    return LCT_SUCCESS;
}

