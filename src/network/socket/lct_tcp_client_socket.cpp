/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_tcp_client_socket.cpp
 * @version     1.0
 * @date        Aug 16, 2017 1:48:44 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_tcp_client_socket.h"
#include "lct_socket_manager.h"
#include "lct_socket_util.h"
#include "lct_log.h"
#include "event/lct_net_event_handler.h"
#include "detail/lct_network_util.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>

CLctTcpClientSocket::CLctTcpClientSocket():
    CLctTcpSocket(ELctSocketType::SOCKET_TYPE_TCP_CLIENT)
{
}

CLctTcpClientSocket::~CLctTcpClientSocket()
{
}

LCT_ERR_CODE CLctTcpClientSocket::onStart()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    setSocketState(ELctSocketState::SOCKET_STATE_CONNECTING);

    std::size_t revBuffSize = DEFAULT_RECV_BUFFER_SIZE;
    if (nullptr != m_eventHandler) {
        revBuffSize = m_eventHandler->socketRecvBuffSize();
    }

    errCode = initRecvBuffer(revBuffSize);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to create recv-buffer for socketFd(" << getSocketFd() << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = connect();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to connect remote service(" << IpAddr2String(getPeerIp()) << ":" << getPeerPort();
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctTcpClientSocket::onRead()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    return errCode;
}

LCT_ERR_CODE CLctTcpClientSocket::onWrite()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    if (nullptr != m_eventHandler) {
        errCode = m_eventHandler->doWriteCompletionHandle();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR<< "Failed to process write-event at eventHandler for socketFd(" << getSocketFd() << ")";
            return errCode;
        }
    } else {
        LOG_WARNG << "EventHandler is NULL for socketFd(" << getSocketFd() << ")";
    }

    return errCode;
}

LCT_ERR_CODE CLctTcpClientSocket::onStop()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    return errCode;
}

LCT_ERR_CODE CLctTcpClientSocket::init(const int64_t localIp, const int16_t localPort)
{
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctTcpClientSocket::connect()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    const int32_t socketFd = socket(AF_INET, SOCK_STREAM, 0);

    setSocketFd(socketFd);

    if (LCT_INVALID_SOCKET_FD == socketFd) {
        LOG_ERROR<< "Failed to start listening on a invalid socket";
        return LCT_FAIL;
    }

    int32_t SynCnt = 1;
    if (0 != setsockopt(socketFd, SOL_TCP, TCP_SYNCNT, &SynCnt, sizeof(SynCnt))) {
        LOG_ERROR<< "Failed to start listening on a invalid socket";
        return LCT_FAIL;
    }

    const int64_t localIp   = getLocalIp();
    const int16_t localPort = getLocalPort();
    if ((LCT_INADDR_ANY != localIp) && (LCT_INVALID_IP != localIp)) {
        errCode = bindSocket(localPort, localIp);
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR<< "Failed to bind socket on address(" << IpAddr2String(localIp) << ":" << localPort;
            stopService();
            return errCode;
        }

        LOG_ERROR<< "Successful to bind socket on address(" << IpAddr2String(localIp) << ":" << localPort;
    }

    errCode = LCTSetSocketNonblocking(socketFd);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to set nonblocking socket(" << socketFd << ")";
        stopService();
        return errCode;
    }

    const int64_t peerIp   = getPeerIp();
    const int16_t peerPort = getPeerPort();
    struct sockaddr_in peerAddress;
    peerAddress.sin_family      = AF_INET;
    peerAddress.sin_port        = htons(peerPort);
    peerAddress.sin_addr.s_addr = htonl(peerIp);

    if (0 == ::connect((int)socketFd, (const struct sockaddr*)&peerAddress, sizeof(peerAddress))) {
        setSocketState(ELctSocketState::SOCKET_STATE_CONNECTED);
        LOG_DEBUG << "Successful to connect socket(" << socketFd << ")";
        return errCode;
    }

    LOG_DEBUG<< "Socket(" << socketFd << ") is connecting " << IpAddr2String(peerIp) << ":" << peerPort;

    const int32_t errorNumber = errno;
    if ((EALREADY == errorNumber) || (EINPROGRESS == errorNumber) || (EWOULDBLOCK == errorNumber)) {
        errCode = retrieveBoundInfo();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR<< "Failed to retrieve bound info on socket(" << socketFd << ")";
            return errCode;
        }
    } else {
        LOG_ERROR << "Failed to connect socket(" << socketFd << ") errno (" << errorNumber << ")";
        stopService();
        return LCT_CONN_SOCKET_FAIL;
    }
    return errCode;
}

LCT_ERR_CODE CLctTcpClientSocket::processWriteEvent()
{
    LOG_DEBUG_MARK();
    if (getSocketState() == ELctSocketState::SOCKET_STATE_CONNECTING) {
        return processConnectEvent();
    } else {
        return CLctTcpSocket::processWriteEvent();
    }
}

LCT_ERR_CODE CLctTcpClientSocket::processConnectEvent()
{
    LOG_DEBUG_MARK();
    int32_t soError = 0;
    socklen_t soErrorSize = sizeof(soError);

    const int32_t socketFd = getSocketFd();
    if (0 != getsockopt(socketFd, SOL_SOCKET, SO_ERROR, &soError, &soErrorSize)) {
        LOG_ERROR<< "Failed to connect socket(" << socketFd << ")";
        stopService();
        return LCT_GET_SOCK_INFO_FAIL;
    }
    if (0 != soError) {
        LOG_ERROR << "Failed to connect socket(" << socketFd << ")";
        return LCT_CONN_SOCKET_FAIL;
    }

    setSocketState(ELctSocketState::SOCKET_STATE_CONNECTED);

    LOG_DEBUG<< "Successful to connect to remote service with socket(" << socketFd << ")";

    return LCT_SUCCESS;
}
