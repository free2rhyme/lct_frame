/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_acceptor.cpp
 * @version     1.0
 * @date        Aug 15, 2017 8:47:14 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_net_acceptor.h"
#include "lct_net_service.h"
#include "socket/lct_tcp_listening_socket.h"
#include "socket/lct_socket_manager.h"
#include "detail/lct_network_util.h"
#include "detail/lct_net_engine.h"
#include "lct_log.h"

CLctNetAcceptor::CLctNetAcceptor(const std::string& ip, const int16_t port) :
        CLctNetEventHandler(),
        m_svcPort(port),
        m_svcIp(IpAddr2long(ip))
{
    LOG_DEBUG_MARK();
}

CLctNetAcceptor::CLctNetAcceptor(const std::string& adapterAddr) :
        CLctNetEventHandler(adapterAddr)
{
    LOG_DEBUG_MARK();
}

CLctNetAcceptor::~CLctNetAcceptor()
{
    LOG_DEBUG_MARK();
}

LCT_ERR_CODE CLctNetAcceptor::doAcceptCompletionHandle(const CLctSocket& socket)
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr == m_socketShp) {
        LOG_ERROR << "Invalid socket object: nullptr" << ErrCodeFormat(LCT_INVALID_POINTER);
        return LCT_INVALID_POINTER;
    }

    if (nullptr != m_asyncAcceptEvent) {
        m_asyncAcceptEvent->setSocket(socket);
        errCode = LCT_NET_ENGINE->postCompletionEvent(m_asyncAcceptEvent);
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to process accept-handle for listening socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
    }  else {
        LOG_WARNG << "asyncAcceptEventHandle is nullptr";
    }

    return errCode;
}

LCT_ERR_CODE CLctNetAcceptor::onStart()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_socketShp = std::make_shared<CLctTcpListeningSocket>(m_svcIp, m_svcPort);
    if (nullptr == m_socketShp) {
        LOG_ERROR << "Failed to create a new listening socket" << ErrCodeFormat(LCT_CREATE_SOCKET_FAIL);
        return LCT_CREATE_SOCKET_FAIL;
    }

    errCode = m_socketShp->startService();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to start service on listening socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = LCT_SOCKET_MGR->addSocket(m_socketShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to add a listening socket into socket manager " << ErrCodeFormat(errCode);
        return errCode;
    }

    m_socketShp->setEventHandler(this);

    return errCode;
}

LCT_ERR_CODE CLctNetAcceptor::onAccept(const CLctNetSessionHandlerShp& sessionHandler)
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr == sessionHandler) {
        LOG_ERROR << "sessionHandler is nullptr";
        return LCT_INVALID_PARAM;
    }

    if (nullptr != m_networkPtr) {
        sessionHandler->setNetService(m_networkPtr);
        errCode = m_networkPtr->registerSessionHandler(sessionHandler->socketFd(), sessionHandler);
    } else {
        LOG_ERROR << "net service ptr is nullptr";
    }
    return errCode;
}


LCT_ERR_CODE CLctNetAcceptor::asyncAccept(LctPlainHandle&& handler)
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_asyncAcceptEvent = std::make_shared<CLctNetAsyncAcceptEvent<LctPlainHandle>>(std::move(handler));

    return errCode;
}

LCT_ERR_CODE CLctNetAcceptor::asyncAccept(LctHandleWithSocket&& handler)
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_asyncAcceptEvent = std::make_shared<CLctNetAsyncAcceptEvent<LctHandleWithSocket, CLctSocket>>(std::move(handler));

    return errCode;
}
