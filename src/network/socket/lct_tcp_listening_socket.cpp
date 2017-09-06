/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_tcp_listening_socket.cpp
 * @version     1.0
 * @date        Aug 16, 2017 1:29:01 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#include "lct_tcp_listening_socket.h"
#include "lct_tcp_server_socket.h"
#include "lct_socket_util.h"
#include "lct_socket_manager.h"
#include "lct_net_service.h"
#include "event/lct_net_event_handler.h"
#include "lct_log.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

CLctTcpListeningSocket::CLctTcpListeningSocket(const int64_t ip, const int16_t port) :
    CLctTcpSocket(ELctSocketType::SOCKET_TYPE_TCP_LISTENING, ip, port)
{
}

CLctTcpListeningSocket::~CLctTcpListeningSocket()
{
}

LCT_ERR_CODE CLctTcpListeningSocket::processReadEvent()
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}
LCT_ERR_CODE CLctTcpListeningSocket::processWriteEvent()
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctTcpListeningSocket::onStart()
{
    LOG_DEBUG_MARK();
    return startListening(getLocalIp(), getLocalPort());
}

LCT_ERR_CODE CLctTcpListeningSocket::onRead()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    CLctSocketShp acceptedSocketShp;
    errCode = accept(acceptedSocketShp);
    if (LCT_SUCCESS != errCode || nullptr == acceptedSocketShp) {
        LOG_ERROR<< "Failed to process accept event for socketFd(" << getSocketFd() << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    if (nullptr != m_eventHandler) {
        errCode = m_eventHandler->doAcceptCompletionHandle(acceptedSocketShp->socketInfo());
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR<< "Failed to process read event for socketFd(" << getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
    } else {
        LOG_ERROR << "Event Handler is null for socketFd(" << getSocketFd() << ")";
    }

    return errCode;
}

LCT_ERR_CODE CLctTcpListeningSocket::onWrite()
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctTcpListeningSocket::onStop()
{
    LOG_DEBUG_MARK();
    return stopListening();
}

LCT_ERR_CODE CLctTcpListeningSocket::startListening(const int64_t adapterIp, const int16_t port)
{
    LOG_DEBUG_MARK();
    int32_t socketFd = getSocketFd();
    if (LCT_INVALID_SOCKET_FD != socketFd) {
        LOG_ERROR << "socket(" << socketFd << ") exists already";
        return LCT_FAIL;
    }

    setLocalPort(port);
    setLocalIp(adapterIp);

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (LCT_INVALID_SOCKET_FD == socketFd) {
        LOG_CRITC<< "Failed to create a new socket(" << errno << ")";
    }

    setSocketFd(socketFd);

    errCode = LCTSetSocketNonblocking(socketFd, m_asyncMode);
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    if (LCT_INADDR_ANY != getLocalPort()) {
        int sflag = 1;
        if (0 != setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (const char *)&sflag, sizeof(sflag))) {
            LOG_ERROR<< "Failed to set socket(" << socketFd << ") reusable due to error(" << errno << ")";
            return LCT_SYSTEM_ERROR;
        }
    }

    errCode = bindSocket(port, adapterIp);
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    if (0 != listen(socketFd, SOMAXCONN)) {
        LOG_ERROR<< "Failed to listen on socket(" << socketFd << ") due to error(" << errno << ")";
        return LCT_LISTEN_SOCKET_FAIL;
    }

    setSocketState(ELctSocketState::SOCKET_STATE_LISTENING);
    if (LCT_INADDR_ANY == getLocalPort()) {
        errCode = retrieveBoundInfo();
        if (LCT_SUCCESS != errCode) {
            return errCode;
        }
    }
    return errCode;
}

LCT_ERR_CODE CLctTcpListeningSocket::stopListening()
{
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctTcpListeningSocket::accept(CLctSocketShp& acceptedSocketShp)
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

//    CLctSocketShp acceptedSocketShp;

    const int32_t socketFd = getSocketFd();

    if (nullptr == acceptedSocketShp) {
        acceptedSocketShp = std::make_shared<CLctTcpServerSocket>();
        if (nullptr == acceptedSocketShp) {
            LOG_ERROR<< "Failed to create a new accepted socket for listing socketFd(" << socketFd << ")" << ErrCodeFormat(LCT_CREATE_SOCKET_FAIL);
            return LCT_CREATE_SOCKET_FAIL;
        }
    }

    struct sockaddr_in peerAddress;
    socklen_t addressSize = sizeof(peerAddress);
    const int32_t fd = ::accept(socketFd, (struct sockaddr *)&peerAddress, &addressSize);
    if (LCT_INVALID_SOCKET_FD == fd) {
        setSocketState(ELctSocketState::SOCKET_STATE_CLOSED);
        LOG_ERROR<< "Failed to accept service for socketFd(" << socketFd << ")" << ErrCodeFormat(LCT_ACCEPT_SOCKET_FAIL);
        return LCT_ACCEPT_SOCKET_FAIL;
    }

    errCode = LCTSetSocketNonblocking(fd);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to set non blocking for socketFd(" << socketFd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    acceptedSocketShp->setSocketFd(fd);
    acceptedSocketShp->setPeerPort(ntohs(peerAddress.sin_port));
    acceptedSocketShp->setPeerIp(ntohl(peerAddress.sin_addr.s_addr));

    if (nullptr != m_eventHandler) {
        CLctNetSessionHandlerShp eventHandlerShp = std::make_shared<CLctNetSessionHandler>(acceptedSocketShp);

        acceptedSocketShp->setEventHandler(eventHandlerShp.get());

        errCode = m_eventHandler->onAccept(eventHandlerShp);
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR<< "Failed to process on-accept event for socketFd(" << getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }

        errCode = m_eventHandler->doReadCompletionHandle();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR<< "Failed to process read event for socketFd(" << getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
    } else {
        LOG_ERROR << "Event Handler is null for socketFd(" << getSocketFd() << ")";
    }

    errCode = acceptedSocketShp->startService();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to start service for socketFd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = LCT_SOCKET_MGR->addSocket(acceptedSocketShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to add socketFd(" << fd << ") into socket manager" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

