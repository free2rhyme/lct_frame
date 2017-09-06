/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_connector.h
 * @version     1.0
 * @date        Aug 15, 2017 8:46:46 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_CONNECTOR_H_
#define SRC_NETWORK_LCT_NET_CONNECTOR_H_

#include "lct_net_session_handler.h"
#include "lct_net_async_connect_event.h"

class CLctNetConnector: public CLctNetSessionHandler
{
public:
    explicit CLctNetConnector(const std::string& ip, const int16_t port);
    explicit CLctNetConnector(const std::string& adapterAddr); //"0.0.0.0:80"
    virtual ~CLctNetConnector();

public:
    virtual LCT_ERR_CODE doReadCompletionHandle() override;
    virtual LCT_ERR_CODE doWriteCompletionHandle() override;

public:
    LCT_ERR_CODE asyncConnect(LctPlainHandle&& handler);
    LCT_ERR_CODE asyncConnect(LctHandleWithSocket&& handler);

public:
    LCT_ERR_CODE onStart() override;

private:
    LCT_ERR_CODE startConnecting();

private:
    int16_t       m_remoteSvcPort         = LCT_INVALID_PORT;
    int64_t       m_remoteSvcIp           = LCT_INVALID_IP;

    CLctNetAsyncConnectEventShp m_asyncConnectEvent;
};

#include "lct_net_event_handler.hpp"

#endif /* SRC_NETWORK_LCT_NET_CONNECTOR_H_ */
