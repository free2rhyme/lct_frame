/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_base_socket.h
 * @version     1.0
 * @date        Aug 16, 2017 1:28:08 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_BASE_SOCKET_H_
#define SRC_NETWORK_LCT_BASE_SOCKET_H_

#include "detail/lct_network_common_define.h"
#include "lct_socket.h"
#include "lct_socket_circular_buffer.h"

class CLctNetEventHandler;

class CLctBaseSocket
{
public:
    explicit CLctBaseSocket(const ELctSocketType type);
    explicit CLctBaseSocket(const ELctSocketType type, const int64_t localIp, const int16_t localPort);
    virtual ~CLctBaseSocket();

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctBaseSocket);

public:
    int32_t getSocketFd() const;
    int64_t getLocalIp()  const;
    int16_t getLocalPort()const;
    int64_t getPeerIp()   const;
    int16_t getPeerPort() const;

    std::size_t getRecvBuffSize() const;

    ELctSocketType getSocketType()   const;
    ELctSocketState getSocketState() const;

    CLctSocket socketInfo() const;
    CLctNetEventHandler* eventHandler() const;

    void setSocketFd(const int32_t fd);
    void setLocalIp(const int64_t ip);
    void setLocalPort(const int16_t port);
    void setPeerIp(const int64_t ip);
    void setPeerPort(const int64_t port);
    void setSocketState(const ELctSocketState state);

    void setEventHandler(CLctNetEventHandler* eventShp);

    LCT_ERR_CODE initRecvBuffer(const std::size_t newBufferSize);
    LCT_ERR_CODE resetSocketRecvBuffSize(const std::size_t newBufferSize);
    LCT_ERR_CODE bindSocket(const int16_t localPort, const int64_t adapterAddr = LCT_INADDR_ANY);
    LCT_ERR_CODE retrieveBoundInfo();

    LCT_ERR_CODE startService(const bool isAsyncMode = true);
    LCT_ERR_CODE stopService();

    LCT_ERR_CODE doProcessRead();
    LCT_ERR_CODE doProcessWrite();

    virtual LCT_ERR_CODE processReadEvent()  = 0;
    virtual LCT_ERR_CODE processWriteEvent() = 0;

    virtual LCT_ERR_CODE send(const char* data, std::size_t size) = 0;

    virtual LCT_ERR_CODE processRecvBuffData();

protected:
    virtual LCT_ERR_CODE onStart() = 0;
    virtual LCT_ERR_CODE onRead()  = 0;
    virtual LCT_ERR_CODE onWrite() = 0;
    virtual LCT_ERR_CODE onStop()  = 0;

protected:
    bool    m_asyncMode          = true;
    int64_t m_recvBufferSize     = 0;
    int64_t m_bytesInRecvBuffer  = 0;
    int64_t m_firstPacketTime    = 0;
    int64_t m_firstPacketTimeout = 0;

    CLctSocket           m_socketInfo;
    CLctSocketBufferShp  m_readBuffer;

    CLctNetEventHandler* m_eventHandler    = nullptr;

    typedef CLctQueue<CLctSocketBufferShp> CLctSocketMessageQueue;
    CLctSocketMessageQueue m_messageQueue {0, 0};
};

#endif /* SRC_NETWORK_LCT_BASE_SOCKET_H_ */
