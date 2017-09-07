/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_connector.cpp
 * @version     1.0
 * @date        Aug 15, 2017 8:47:06 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_net_connector.h"
#include "socket/lct_tcp_client_socket.h"
#include "socket/lct_socket_manager.h"
#include "detail/lct_net_engine.h"
#include "detail/lct_network_util.h"
#include "lct_log.h"
#include <sys/epoll.h>

CLctNetConnector::CLctNetConnector(const std::string& ip, const int16_t port) :
        CLctNetSessionHandler(), m_remoteSvcPort(port), m_remoteSvcIp(IpAddr2long(ip))
{
    LOG_DEBUG_MARK();
}

CLctNetConnector::CLctNetConnector(const std::string& adapterAddr) :
        CLctNetSessionHandler(adapterAddr)
{
    LOG_DEBUG_MARK();
}

CLctNetConnector::~CLctNetConnector()
{
    LOG_DEBUG_MARK();
}

LCT_ERR_CODE CLctNetConnector::onStart()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = startConnecting();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to start connecting socket" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctNetConnector::startConnecting()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_socketShp = std::make_shared<CLctTcpClientSocket>();
    if (nullptr == m_socketShp) {
        LOG_ERROR<< "Failed to create a new client socket" << ErrCodeFormat(LCT_CREATE_SOCKET_FAIL);
        return LCT_CREATE_SOCKET_FAIL;
    }

    m_socketShp->setPeerIp(m_remoteSvcIp);
    m_socketShp->setPeerPort(m_remoteSvcPort);
    m_socketShp->setEventHandler(this);

    errCode = m_socketShp->startService();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to start service on a client socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = LCT_SOCKET_MGR->addSocket(m_socketShp, EPOLLIN | EPOLLOUT | EPOLLET | EPOLLHUP | EPOLLRDHUP | EPOLLERR);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to add a client socket into socket manager " << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctNetConnector::doReadCompletionHandle()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    return errCode;
}

LCT_ERR_CODE CLctNetConnector::doWriteCompletionHandle()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr == m_socketShp) {
        LOG_ERROR<< "Invalid socket object: nullptr" << ErrCodeFormat(LCT_INVALID_POINTER);
        return LCT_INVALID_POINTER;
    }

    if (m_socketShp->getSocketState() == ELctSocketState::SOCKET_STATE_CONNECTING) {

        m_socketShp->setSocketState(ELctSocketState::SOCKET_STATE_CONNECTED);

        if (nullptr != m_asyncConnectEvent) {
            errCode = LCT_NET_ENGINE->postCompletionEvent(m_asyncConnectEvent);
            if (LCT_SUCCESS != errCode) {
                LOG_ERROR<< "Failed to process connect-handle for client socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
                return errCode;
            }
        } else {
            LOG_WARNG << "asyncConnectEventHandle is nullptr";
        }
    } else {
        errCode = CLctNetSessionHandler::doWriteCompletionHandle();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR<< "Failed to process write-handle for client socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
    }

    return errCode;
}


LCT_ERR_CODE CLctNetConnector::asyncConnect(LctPlainHandle&& handler)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (!isInitiated()) {
        LOG_WARNG<< "Connector has NOT been initiated yet";
        errCode = init();
        if(LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to init Connector";
            return errCode;
        }
    }

    m_asyncConnectEvent = std::make_shared<CLctNetAsyncConnectEvent<LctPlainHandle>>(std::move(handler));

    return errCode;
}

LCT_ERR_CODE CLctNetConnector::asyncConnect(LctHandleWithSocket&& handler)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (!isInitiated()) {
        LOG_WARNG<< "Connector has NOT been initiated yet";
        errCode = init();
        if(LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to init Connector";
            return errCode;
        }
    }

    m_asyncConnectEvent = std::make_shared<CLctNetAsyncConnectEvent<LctHandleWithSocket, CLctSocket>>(std::move(handler), m_socketShp->socketInfo());
    return errCode;
}

