/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_udp_socket.cpp
 * @version     1.0
 * @date        Aug 16, 2017 8:17:57 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#include "lct_udp_socket.h"
#include "lct_log.h"
#include <sys/socket.h>

CLctUdpSocket::CLctUdpSocket():
    CLctBaseSocket(ELctSocketType::SOCKET_TYPE_UDP)
{
    const int32_t socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    setSocketFd(socketFd);
    if (LCT_INVALID_SOCKET_FD == socketFd) {
        LOG_CRITC<< "Failed to create a new socket";
    }
}

CLctUdpSocket::~CLctUdpSocket()
{
    const int32_t socketFd = getSocketFd();
    if (LCT_INVALID_SOCKET_FD != socketFd) {
        if (0 != close(socketFd)) {
            LOG_ERROR<< "Failed to close socket(" << socketFd;
        }
        setSocketFd(LCT_INVALID_SOCKET_FD);
    }
}

