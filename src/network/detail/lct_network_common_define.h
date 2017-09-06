/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_network_common_define.h
 * @version     1.0
 * @date        Aug 16, 2017 3:00:08 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NETWORK_COMMON_DEFINE_H_
#define SRC_NETWORK_LCT_NETWORK_COMMON_DEFINE_H_

#include "lct_common_define.h"
#include "socket/lct_socket_circular_buffer.h"
#include "lct_queue.h"
#include <functional>

static constexpr int32_t LCT_INVALID_SOCKET_FD = -1;
static constexpr int64_t LCT_INVALID_IP        = -1;
static constexpr int16_t LCT_INVALID_PORT      = -1;
static constexpr int64_t LCT_INADDR_ANY        = 0;

static constexpr int32_t LCT_EPOLL_EVENT_MAX_SIZE = 1024;

static constexpr int32_t DEFAULT_FIRST_PACKET_TIMEOUT   = 5;
static constexpr int32_t DEFAULT_MAX_SOCKET_QUEUE_DEPTH = 10000;
static constexpr int32_t DEFAULT_RECV_BUFFER_SIZE       = 1024;

enum class ELctSocketType: int16_t
{
    SOCKET_TYPE_UNKNOWN,
    SOCKET_TYPE_TCP_LISTENING,
    SOCKET_TYPE_TCP_CLIENT,
    SOCKET_TYPE_TCP_SERVER,
    SOCKET_TYPE_UDP
};

enum class ELctSocketState: int16_t
{
    SOCKET_STATE_UNKNOWN,
    SOCKET_STATE_CLOSED,
    SOCKET_STATE_LISTENING,
    SOCKET_STATE_CONNECTING,
    SOCKET_STATE_CONNECTED,
    SOCKET_STATE_UDP_READY
};

class CLctBaseSocket;
typedef std::shared_ptr<CLctBaseSocket> CLctSocketShp;

class CLctNetSessionHandler;
typedef std::shared_ptr<CLctNetSessionHandler> CLctNetSessionHandlerShp;

typedef CLctSocketCircularBuffer CLctSocketBuffer;
typedef std::shared_ptr<CLctSocketBuffer> CLctSocketBufferShp;

class CLctSocket;

typedef std::function<void()>                   LctPlainHandle;
typedef std::function<void(const CLctSocket&)>  LctHandleWithSocket;

#endif /* SRC_NETWORK_LCT_NETWORK_COMMON_DEFINE_H_ */
