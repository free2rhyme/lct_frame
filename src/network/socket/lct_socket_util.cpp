/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_socket_util.cpp
 * @version     1.0
 * @date        Aug 16, 2017 10:39:32 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <errno.h>
#include "lct_socket_util.h"
#include "lct_log.h"

LCT_ERR_CODE LCTResetEtOneShot(const int32_t epollfd, const int32_t fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    if (0 != epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event)) {
        LOG_ERROR<< "Failed to reset oneshot event errno(" << errno << ")";
        return LCT_SYSTEM_ERROR;
    }
    return LCT_SUCCESS;
}

LCT_ERR_CODE LCTSetSocketNonblocking(const int32_t fd, const bool isNonblocking)
{
    int32_t flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        return LCT_SYSTEM_ERROR;
    }

    if (isNonblocking) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }

    if (fcntl(fd, F_SETFL, flags) != 0) {
        return LCT_SYSTEM_ERROR;
    }

    return LCT_SUCCESS;
}

LCT_ERR_CODE LctSetSocketKeepalive(const int32_t socketId, const int64_t keepAliveSeconds)
{
    if (keepAliveSeconds > 0) {
        int KeepAlive = 1;
        if (0 != setsockopt(socketId, SOL_SOCKET, SO_KEEPALIVE, &KeepAlive, sizeof(KeepAlive))) {
            return LCT_SET_SOCK_ALIVE_FAIL;
        }
        if (0 != setsockopt(socketId, IPPROTO_TCP, TCP_KEEPIDLE, &keepAliveSeconds, sizeof(keepAliveSeconds))) {
            return LCT_SET_SOCK_ALIVE_FAIL;
        }
    } else {
        int keepAlive = 0;
        if (0 != setsockopt(socketId, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(keepAlive))) {
            return LCT_SET_SOCK_ALIVE_FAIL;
        }
    }
    return LCT_SUCCESS;
}

LCT_ERR_CODE LctSetSocketSndbuf(const int32_t fd, const int32_t buffer_size_bytes)
{
    return 0 == setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffer_size_bytes, sizeof(buffer_size_bytes)) ?
            LCT_SUCCESS : LCT_SYSTEM_ERROR;
}

LCT_ERR_CODE LctSetSocketRcvbuf(const int32_t fd, const int32_t buffer_size_bytes)
{
    return 0 == setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &buffer_size_bytes, sizeof(buffer_size_bytes)) ?
            LCT_SUCCESS : LCT_SYSTEM_ERROR;
}

LCT_ERR_CODE LctSetSocketCloexec(const int32_t fd, const bool closeOnExec)
{
    int32_t flags = fcntl(fd, F_GETFD, 0);
    if (flags < 0) {
        return LCT_SYSTEM_ERROR;
    }

    if (closeOnExec) {
        flags |= FD_CLOEXEC;
    } else {
        flags &= ~FD_CLOEXEC;
    }

    if (fcntl(fd, F_SETFD, flags) != 0) {
        return LCT_SYSTEM_ERROR;
    }
    return LCT_SUCCESS;
}

LCT_ERR_CODE LctSetSocketReuseAddr(const int32_t fd, const bool reuse)
{
    int32_t val = (reuse ? 1 : 0);
    int32_t newval;
    socklen_t intlen = sizeof(newval);
    if (0 != setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val))) {
        return LCT_SYSTEM_ERROR;
    }
    if (0 != getsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &newval, &intlen)) {
        return LCT_SYSTEM_ERROR;
    }
    if ((newval != 0) != reuse) {
        return LCT_SYSTEM_ERROR;
    }
    return LCT_SUCCESS;
}

LCT_ERR_CODE LctSetSocketReusePort(const int32_t fd, const bool reuse)
{
    int32_t val = (reuse ? 1 : 0);
    int32_t newval;
    socklen_t intlen = sizeof(newval);
    if (0 != setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val))) {
        return LCT_SYSTEM_ERROR;
    }
    if (0 != getsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &newval, &intlen)) {
        return LCT_SYSTEM_ERROR;
    }
    if ((newval != 0) != reuse) {
        return LCT_SYSTEM_ERROR;
    }
    return LCT_SUCCESS;
}

LCT_ERR_CODE LctSetSocketLowLatency(const int32_t fd, const bool setLowLatency)
{
    int32_t val = (setLowLatency ? 1 : 0);
    int32_t newval;
    socklen_t intlen = sizeof(newval);
    if (0 != setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val))) {
        return LCT_SYSTEM_ERROR;
    }
    if (0 != getsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &newval, &intlen)) {
        return LCT_SYSTEM_ERROR;
    }
    if ((newval != 0) != val) {
        return LCT_FAIL;
    }
    return LCT_SUCCESS;
}

bool IsFatalSocketError(const int32_t errorNumber)
{
    if (EINPROGRESS == errorNumber) {
        return false;
    }

    if (EAGAIN == errorNumber) {
        return false;
    }

    if (EWOULDBLOCK == errorNumber) {
        return false;
    }

    if (EINTR == errorNumber) {
        return false;
    }
    return true;
}

bool IsSocketDisconnError(const int32_t errorNumber)
{
    if (ENETDOWN == errorNumber) {
        return true;
    }

    if (ENOTCONN == errorNumber) {
        return true;
    }

    if (ESHUTDOWN == errorNumber) {
        return true;
    }

    if (ECONNABORTED == errorNumber) {
        return true;
    }

    if (ECONNRESET == errorNumber) {
        return true;
    }

    if (EPIPE == errorNumber) {
        return true;
    }

    if (EINVAL == errorNumber) {
        return true;
    }

    return false;
}
