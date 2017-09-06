/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_acceptor.h
 * @version     1.0
 * @date        Aug 15, 2017 8:46:56 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_ACCEPTOR_H_
#define SRC_NETWORK_LCT_NET_ACCEPTOR_H_

#include "lct_net_event_handler.h"
#include "lct_net_async_accept_event.h"
#include "lct_socket.h"

class CLctNetAcceptor: public CLctNetEventHandler
{
public:
    explicit CLctNetAcceptor(const std::string& ip, const int16_t port);
    explicit CLctNetAcceptor(const std::string& adapterAddr); //"0.0.0.0:80"
    virtual ~CLctNetAcceptor();

public:
    LCT_ERR_CODE doAcceptCompletionHandle(const CLctSocket& socket) override;

public:
    LCT_ERR_CODE asyncAccept(LctPlainHandle&& handler);
    LCT_ERR_CODE asyncAccept(LctHandleWithSocket&& handler);

public:
    LCT_ERR_CODE onStart() override;
    LCT_ERR_CODE onAccept(const CLctNetSessionHandlerShp& sessionHandler) override;


private:
    int16_t       m_svcPort         = LCT_INVALID_PORT;
    int64_t       m_svcIp           = LCT_INVALID_IP;

    CLctSocketShp                m_latestAcceptedSocket;
    CLctNetAsyncAcceptEventShp   m_asyncAcceptEvent;
};

#include "lct_net_acceptor.hpp"

#endif /* SRC_NETWORK_LCT_NET_ACCEPTOR_H_ */
