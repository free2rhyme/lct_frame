/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_event.h
 * @version     1.0
 * @date        Aug 15, 2017 8:46:20 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_EVENT_HANDLER_H_
#define SRC_NETWORK_LCT_NET_EVENT_HANDLER_H_

#include "detail/lct_network_common_define.h"
#include "event/lct_net_async_read_event.h"
#include "event/lct_net_async_write_event.h"

class CLctNetService;
class CLctNetEventHandler
{
public:
    explicit CLctNetEventHandler();
    explicit CLctNetEventHandler(const CLctSocketShp& socketShp);
    explicit CLctNetEventHandler(const std::string& adapterAddr);
    virtual ~CLctNetEventHandler();

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctNetEventHandler);

public:
    CLctNetService* netService() const;
    void setNetService(CLctNetService* ptr);

    std::size_t socketRecvBuffSize() const;
    void setSocketRecvBuffSize(const std::size_t size);
    LCT_ERR_CODE resetSocketRecvBuffSize();

    bool isSocketConnected() const;

    int32_t socketFd() const;
public:
    LCT_ERR_CODE init();
    LCT_ERR_CODE run();
    LCT_ERR_CODE stop();

    virtual LCT_ERR_CODE onStart() = 0;
    virtual LCT_ERR_CODE onStop();
    virtual LCT_ERR_CODE onAccept(const CLctNetSessionHandlerShp& sessionHandler);

public:
    virtual LCT_ERR_CODE processRecvBuffData(CLctSocketBufferShp& messageShp);

    virtual LCT_ERR_CODE doAcceptCompletionHandle(const CLctSocket& socket);
    virtual LCT_ERR_CODE doReadCompletionHandle();
    virtual LCT_ERR_CODE doWriteCompletionHandle();

protected:
    LCT_ERR_CODE initializeSocket(const CLctSocketShp& socketShp);

    bool isInitiated() const;

protected:
    std::size_t         m_socketRecvBuffSize = DEFAULT_RECV_BUFFER_SIZE;
    CLctSocketShp       m_socketShp;
    std::string         m_svcAdapterAddr;
    CLctNetService*     m_networkPtr = nullptr;
};

//#include "lct_net_event_handler.hpp"

#endif /* SRC_NETWORK_LCT_NET_EVENT_HANDLER_H_ */
