/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_socket_util.h
 * @version     1.0
 * @date        Aug 16, 2017 9:25:08 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_SOCKET_UTIL_H_
#define SRC_NETWORK_LCT_SOCKET_UTIL_H_

#include "detail/lct_network_common_define.h"

LCT_ERR_CODE LCTSetSocketNonblocking(const int32_t fd, const bool isNonblocking = true);

LCT_ERR_CODE LctSetSocketSndbuf(const int32_t fd, const int32_t buffer_size_bytes);

LCT_ERR_CODE LctSetSocketRcvbuf(const int32_t fd, const int32_t buffer_size_bytes);

LCT_ERR_CODE LctSetSocketCloexec(const int32_t fd, const bool closeOnExec);

LCT_ERR_CODE LctSetSocketReuseAddr(const int32_t fd, const bool reuse);

LCT_ERR_CODE LctSetSocketReusePort(const int32_t fd, const bool reuse);

LCT_ERR_CODE LctSetSocketLowLatency(const int32_t fd, const bool setLowLatency);

bool IsFatalSocketError(const int32_t errorNumber);
bool IsSocketDisconnError(const int32_t errorNumber);

#endif /* SRC_NETWORK_LCT_SOCKET_UTIL_H_ */

