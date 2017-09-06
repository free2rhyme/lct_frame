/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_error_code.h
 * @version      1.0
 * @date         May 17, 2017 11:32:52 AM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_ERROR_CODE_H_
#define SRC_COMMON_LCT_ERROR_CODE_H_

#include <cstdint>
#include <string>

typedef int32_t        LCT_ERR_CODE;

constexpr LCT_ERR_CODE LCT_SUCCESS                = 0x00000000;    // Operation successful
constexpr LCT_ERR_CODE LCT_FAIL                   = 0x00000001;    //
constexpr LCT_ERR_CODE LCT_INVALID_PARAM          = 0x00000002;    //
constexpr LCT_ERR_CODE LCT_INVALID_POINTER        = 0x00000003;    //
constexpr LCT_ERR_CODE LCT_DUPLICATE_REQUEST      = 0x00000004;    //
constexpr LCT_ERR_CODE LCT_OVERFLOW               = 0x00000005;    //
constexpr LCT_ERR_CODE LCT_IMPROPER_STATUS        = 0x00000006;    //
constexpr LCT_ERR_CODE LCT_OUT_OF_BOUNDARY        = 0x00000006;    //
constexpr LCT_ERR_CODE LCT_INVALID_FD             = 0x00000007;    //
constexpr LCT_ERR_CODE LCT_TIMEOUT                = 0x00000008;    //
constexpr LCT_ERR_CODE LCT_SERVICE_DOWN           = 0x00000009;    //
constexpr LCT_ERR_CODE LCT_EMPTY_RECORD           = 0x8000F001;    // Empty record
constexpr LCT_ERR_CODE LCT_RECORD_EXISTS          = 0x8000F002;    // Record exists
constexpr LCT_ERR_CODE LCT_NOT_SUCH_RECORD        = 0x8000F003;    // Not such record
constexpr LCT_ERR_CODE LCT_UNIMPLEMENTED          = 0x8000F004;    // Unimplemented
constexpr LCT_ERR_CODE LCT_INVALID_BIZ_TYPE       = 0x8000F005;    // Invalid business type
constexpr LCT_ERR_CODE LCT_NOT_INITIATED_YET      = 0x8000F006;    // Uninitiated yet

constexpr LCT_ERR_CODE LCT_SYSTEM_ERROR           = 0x8000F101;    // System API error
constexpr LCT_ERR_CODE LCT_INVALID_SOCKET         = 0x8000F106;    // Invalid socket.
constexpr LCT_ERR_CODE LCT_CREATE_SOCKET_FAIL     = 0x8000F107;    // Failed to create a new socket.
constexpr LCT_ERR_CODE LCT_BIND_SOCKET_FAIL       = 0x8000F108;    // Failed to bind socket.
constexpr LCT_ERR_CODE LCT_LISTEN_SOCKET_FAIL     = 0x8000F109;    // Failed to listen on socket.
constexpr LCT_ERR_CODE LCT_CONN_SOCKET_FAIL       = 0x8000F110;    // Failed to connect the server.
constexpr LCT_ERR_CODE LCT_ACCEPT_SOCKET_FAIL     = 0x8000F111;    // Failed to accept the conn socket.
constexpr LCT_ERR_CODE LCT_GET_SOCK_NAME_FAIL     = 0x8000F116;    // Failed to get socket name.
constexpr LCT_ERR_CODE LCT_GET_SOCK_INFO_FAIL     = 0x8000F117;    // Failed to get socket info.
constexpr LCT_ERR_CODE LCT_SET_SOCK_ALIVE_FAIL    = 0x8000F118;    // Failed to set socket keepalive.

constexpr LCT_ERR_CODE LCT_TCP_PARTIAL_MSG_SENT   = 0x8000F121;    // Part of TCP message is sent.
constexpr LCT_ERR_CODE LCT_TCP_SEND_MSG_FAIL      = 0x8000F122;    // Failed to send TCP message.

constexpr LCT_ERR_CODE LCT_EXTRACT_TCP_MSG_FAIL   = 0x8000F126;    // Failed to extract TCP message.
constexpr LCT_ERR_CODE LCT_TCP_MSG_NOT_READY      = 0x8000F127;    // TCP message is not ready to extract.
constexpr LCT_ERR_CODE LCT_INSUFFICIENT_BUFF_SIZE = 0x8000F128;    // Insufficient recv buff size.
constexpr LCT_ERR_CODE LCT_PEER_CLOSE_SOCKET      = 0x8000F129;    // Peer close this socket.
constexpr LCT_ERR_CODE LCT_FATAL_ERROR_READING    = 0x8000F130;    // Fatal error happens during reading.
constexpr LCT_ERR_CODE LCT_TRY_AGAIN_READING      = 0x8000F131;    // Try again to read.

constexpr LCT_ERR_CODE LCT_READ_EVENT_UNSET       = 0x8000F130;    // read-event is unset.

constexpr LCT_ERR_CODE LCT_UNEXPECTED             = 0x8000FFFF;    // Unexpected failure

//#define ErrCodeFormat(errCode) std::hex << "errorCode(0x" << errCode << std::dec << ")"

std::string ErrCodeFormat(const LCT_ERR_CODE errCode);

#endif /* SRC_COMMON_LCT_ERROR_CODE_H_ */




