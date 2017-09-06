/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_service.cpp
 * @version     1.0
 * @date        Aug 18, 2017 10:55:30 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_net_service.h"
#include "lct_net_engine.h"
#include "socket/lct_socket_manager.h"

CLctNetService::CLctNetService(const std::string& ip, const int16_t port) :
        m_acceptor(ip, port)
{
    m_acceptor.setNetService(this);
    start();
}

CLctNetService::~CLctNetService()
{
}

LCT_ERR_CODE CLctNetService::startWork()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = m_acceptor.init();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to prepare acceptor" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctNetService::stopWork()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = m_acceptor.stop();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to shut down the service of acceptor" << ErrCodeFormat(errCode);
    }

    return errCode;
}

LCT_ERR_CODE CLctNetService::checkWork()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = startWork();

    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to start work" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctNetService::resetRecvBuffSize()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    for (auto& kv: m_sessionHandlerMap) {
        CLctNetSessionHandlerShp& handlerRef  = kv.second;
        errCode = handlerRef->resetSocketRecvBuffSize();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to reset socket recv-buff size for fd(" << kv.first << ")" << ErrCodeFormat(errCode);
        }
    }

    return errCode;
}

LCT_ERR_CODE CLctNetService::registerSessionHandler(const int32_t socketFd, const CLctNetSessionHandlerShp& sessionHandlerShp)
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr == sessionHandlerShp) {
        LOG_ERROR << "Invalid session socket" << ErrCodeFormat(LCT_INVALID_POINTER);
        return LCT_INVALID_POINTER;
    }

    errCode = saveSessionHandler(socketFd, sessionHandlerShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to save session socket for socketFd(" << socketFd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    if (nullptr != m_asyncReadEvent) {
        sessionHandlerShp->postReadEventHandle(m_asyncReadEvent);
    }

    if (nullptr != m_asyncWriteEvent) {
        sessionHandlerShp->postWriteEvent(m_asyncWriteEvent);
    }

    return errCode;
}

LCT_ERR_CODE CLctNetService::sessionHandler(const int32_t socketFd, CLctNetSessionHandlerShp& sessionHandlerShp) const
{
    LOG_DEBUG_MARK();

    std::lock_guard<CSessionHandlerMapMutex> lk(m_sessionHandlerMapMutex);

    CLctSessionHandlerMap::const_iterator itRe = m_sessionHandlerMap.find(socketFd);
    if (m_sessionHandlerMap.end() == itRe) {
        LOG_ERROR << "There is no such session socket for fd(" << socketFd << ")" << ErrCodeFormat(LCT_FAIL);
        return LCT_NOT_SUCH_RECORD;
    }
    sessionHandlerShp = itRe->second;

    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctNetService::eraseSessionHandler(const int32_t socketFd)
{
    LOG_DEBUG_MARK();

    std::lock_guard<CSessionHandlerMapMutex> lk(m_sessionHandlerMapMutex);

    auto it = m_sessionHandlerMap.find(socketFd);
    if (m_sessionHandlerMap.end() == it) {
        LOG_ERROR << "Failed to find session handler for socket(" << socketFd << ") in net service";
        return LCT_NOT_SUCH_RECORD;
    }
    m_sessionHandlerMap.erase(it);

    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctNetService::updateReadEvent()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr != m_asyncReadEvent) {
        for (auto& kv: m_sessionHandlerMap) {
            CLctNetSessionHandlerShp& handlerRef  = kv.second;
            handlerRef->postReadEventHandle(m_asyncReadEvent);
        }
    }

    return errCode;
}


LCT_ERR_CODE CLctNetService::updateWriteEvent()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    for (auto& kv: m_sessionHandlerMap) {
        CLctNetSessionHandlerShp& handlerRef  = kv.second;
        handlerRef->postWriteEvent(m_asyncWriteEvent);
    }

    return errCode;
}

LCT_ERR_CODE CLctNetService::saveSessionHandler(const int32_t socketFd, const CLctNetSessionHandlerShp& sessionHandlerShp)
{
    LOG_DEBUG_MARK();

    std::lock_guard<CSessionHandlerMapMutex> lk(m_sessionHandlerMapMutex);

    CLctSessionHandlerMap::const_iterator itRe = m_sessionHandlerMap.find(socketFd);
    if (m_sessionHandlerMap.end() != itRe) {
        LOG_ERROR << "Session socket(" << socketFd << ") exists already" << ErrCodeFormat(LCT_RECORD_EXISTS);
        return LCT_RECORD_EXISTS;
    }

    m_sessionHandlerMap[socketFd] = sessionHandlerShp;
    return LCT_SUCCESS;
}
