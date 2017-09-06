/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_tcp_socket.cpp
 * @version     1.0
 * @date        Aug 16, 2017 1:46:36 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_TCP_SOCKET_CPP_
#define SRC_NETWORK_LCT_TCP_SOCKET_CPP_

#include "lct_tcp_socket.h"
#include "lct_socket_util.h"
#include "lct_log.h"
#include <cstdlib>
#include <sys/socket.h>
#include <errno.h>

CLctTcpSocket::CLctTcpSocket(const ELctSocketType type):
    CLctBaseSocket(type)
{
}

CLctTcpSocket::CLctTcpSocket(const ELctSocketType type, const int64_t ip, const int16_t port) :
    CLctBaseSocket(type, ip, port)
{
}

CLctTcpSocket::~CLctTcpSocket()
{
}

LCT_ERR_CODE CLctTcpSocket::send(const char* pData, std::size_t size)
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (!m_messageQueue.empty()) {
        LOG_DEBUG << "m_messageQueue is empty, so put one into the queue";

        CLctSocketBufferShp socketBufferShp = std::make_shared<CLctSocketBuffer>(pData, size);
        m_messageQueue.enqueue(socketBufferShp);
        return LCT_SUCCESS;
    }

    std::size_t offset = 0;
    errCode = sendData(pData, size, offset);
    if (errCode == LCT_SUCCESS) {
        LOG_DEBUG << "the whole message(" << std::string(pData, size) << " is sent";
        return errCode;
    } else if (errCode == LCT_TCP_PARTIAL_MSG_SENT) {

        LOG_DEBUG << "part of  message(" << std::string(pData, size - offset) << " is sent";

        CLctSocketBufferShp socketBufferShp = std::make_shared<CLctSocketBuffer>(pData + offset, size - offset);
        m_messageQueue.enqueue(socketBufferShp);
        return errCode;
    } else {
        LOG_ERROR << "Failed to sent message and have drop this message errno(" << errno << ")";
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctTcpSocket::sendData(CLctSocketBufferShp& socketBufferShp)
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    int64_t dataSize     = socketBufferShp->size();
    int64_t seqeunceSize = socketBufferShp->space();
    std::size_t offset   = 0;

    if (dataSize > seqeunceSize) {
        while (dataSize > seqeunceSize) {

            errCode = sendData(socketBufferShp->first(), seqeunceSize, offset);

            LCT_ERR_CODE err2Code = m_readBuffer->expand(offset);
            if (LCT_SUCCESS != err2Code) {
                LOG_ERROR<< "Failed to adjust circular buffer for socket";
                stopService();
                return err2Code;
            }

            if (LCT_SUCCESS != errCode) {
                break;
            }
        }
    } else {
        errCode = sendData(socketBufferShp->first(), dataSize, offset);

        LCT_ERR_CODE err2Code = m_readBuffer->expand(offset);
        if (LCT_SUCCESS != err2Code) {
            LOG_ERROR<< "Failed to adjust circular buffer for socket";
            stopService();
            return err2Code;
        }
    }

    if (errCode == LCT_SUCCESS) {
        LOG_DEBUG << "the whole message is sent";
        return errCode;
    } else if (errCode == LCT_TCP_PARTIAL_MSG_SENT) {
        LOG_DEBUG << "part of  message is sent";

        m_messageQueue.enqueue(socketBufferShp);
        return errCode;
    } else {
        LOG_ERROR << "Failed to sent message and have drop this message errno(" << errno << ")";
        return errCode;
    }


    return errCode;
}

LCT_ERR_CODE CLctTcpSocket::sendData(const char* pData, std::size_t size, std::size_t& offset)
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    LOG_DEBUG << "Going to send data(" << pData << ")";

    int64_t dataSize = size;

    offset = 0;

    const int32_t socketFd = getSocketFd();
    while (dataSize > 0) {
        int64_t result = ::send(socketFd, pData, dataSize, 0);
        if (result > 0) {
            pData       += result;
            dataSize    -= result;

            offset      += result;

            if (dataSize <= 0) {
                break;
            }
        } else {
            int32_t socketError = 0;
            if (result == -1) {
                socketError = errno;
            }

            if (IsSocketDisconnError(socketError)) {
                errCode = LCT_TCP_SEND_MSG_FAIL;
                break;
            } else if (!IsFatalSocketError(socketError)) {
                errCode = LCT_TCP_PARTIAL_MSG_SENT;
                break;
            } else {
                errCode = LCT_TCP_SEND_MSG_FAIL;
                break;
            }
        }
    }

    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Error happens during sending message errno(" << errno << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctTcpSocket::processReadEvent()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    bool bDone = false;

    const int32_t socketFd = getSocketFd();

    while (!bDone) {
        const int64_t bytesFree = m_readBuffer->space();
        if (bytesFree <= 0) {
            LOG_ERROR << "There is no enough room for rest data";
            stopService();
            bDone = true;
            return LCT_FAIL;
        }

        char* pNextFreeByte = m_readBuffer->last();

        const int64_t result = ::recv(socketFd, pNextFreeByte, bytesFree, 0);
        if (result > 0) {
            errCode = m_readBuffer->expand(result);
            if (LCT_SUCCESS != errCode) {
                LOG_ERROR<< "Failed to adjust circular buffer for socket" << ErrCodeFormat(LCT_INSUFFICIENT_BUFF_SIZE);
                errCode = LCT_INSUFFICIENT_BUFF_SIZE;
                break;
            }

            errCode = processRecvBuffData();
            if (LCT_TCP_MSG_NOT_READY == errCode) {
                LOG_WARNG << "Failed to process recv buffer message due to incomplete message " << ErrCodeFormat(errCode);
                continue;
            } else if (LCT_READ_EVENT_UNSET == errCode) {
                LOG_WARNG << "Failed to process recv buffer message since there is no event to process recv buf data" << ErrCodeFormat(errCode);
                continue;
            } else {
                if (LCT_SUCCESS != errCode) {
                    LOG_ERROR << "Failed to process recv buffer message" << ErrCodeFormat(errCode);
                    break;
                }
            }

        } else if (result == 0) {
            LOG_ERROR << "Error happens during socket-reading errno(" << errno << "), the peer closed the socket" << ErrCodeFormat(LCT_PEER_CLOSE_SOCKET);
            errCode = LCT_PEER_CLOSE_SOCKET;
            break;
        } else if (result == -1) {
            const int32_t socketError = errno;
            if (IsFatalSocketError(socketError)) {
                LOG_ERROR << "Error happens during socket-reading errno(" << errno << ")" << ErrCodeFormat(LCT_FATAL_ERROR_READING);
                errCode = LCT_FATAL_ERROR_READING;
                break;
            } else {
                LOG_WARNG << "Interrupt happens during socket-reading errno(" << errno << ")" << ErrCodeFormat(LCT_TRY_AGAIN_READING);
                errCode = LCT_SUCCESS;
                break;
            }
        } else {
            LOG_ERROR << "Error happens during socket-reading errno(" << errno << ")" << ErrCodeFormat(LCT_UNEXPECTED);
            errCode = LCT_UNEXPECTED;
            break;
        }
    }

    if (LCT_SUCCESS != errCode) {
        if (LCT_SUCCESS != stopService()) {
            LOG_ERROR << "Failed to stop service";
        }
    }

    return errCode;
}

LCT_ERR_CODE CLctTcpSocket::processWriteEvent()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    while (!m_messageQueue.empty()) {

        CLctSocketBufferShp socketBufferShp;

        errCode = m_messageQueue.dequeue(socketBufferShp);
        if ((LCT_SUCCESS != errCode) || (nullptr == socketBufferShp)) {
            LOG_ERROR << "Failed to dequeue a socket message";
            break;
        }

        errCode = sendData(socketBufferShp);
        if (LCT_TCP_PARTIAL_MSG_SENT == errCode) {
            LOG_WARNG << "Just sent part of a socket message and put it back to queue";
            m_messageQueue.enqueue(socketBufferShp);
            break;
        } else {
            if (LCT_SUCCESS != errCode) {
                LOG_ERROR << "Failed to send a socket message";
                break;
            }
        }
    }
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctTcpSocket::onStart()
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctTcpSocket::onRead()
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctTcpSocket::onWrite()
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctTcpSocket::onStop()
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

#endif /* SRC_NETWORK_LCT_TCP_SOCKET_CPP_ */
