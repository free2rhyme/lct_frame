/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_event.cpp
 * @version     1.0
 * @date        Aug 15, 2017 8:46:26 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_log.h"
#include "lct_net_event_handler.h"
#include "socket/lct_socket_manager.h"

CLctNetEventHandler::CLctNetEventHandler()
{
    LOG_DEBUG_MARK();
}

CLctNetEventHandler::CLctNetEventHandler(const CLctSocketShp& socketShp) :
        m_socketShp(socketShp)
{
    LOG_DEBUG_MARK();
}

CLctNetEventHandler::CLctNetEventHandler(const std::string& adapterAddr) :
        m_svcAdapterAddr(adapterAddr)
{
    LOG_DEBUG_MARK();
}

CLctNetEventHandler::~CLctNetEventHandler()
{
    LOG_DEBUG_MARK();
}

CLctNetService* CLctNetEventHandler::netService() const
{
    LOG_DEBUG_MARK();
    return m_networkPtr;
}

void CLctNetEventHandler::setNetService(CLctNetService* ptr)
{
    LOG_DEBUG_MARK();
    m_networkPtr = ptr;
}

LCT_ERR_CODE CLctNetEventHandler::initializeSocket(const CLctSocketShp& socketShp)
{
    LOG_DEBUG_MARK();
    m_socketShp = socketShp;
    return LCT_SUCCESS;
}

bool CLctNetEventHandler::isInitiated() const
{
    return (m_socketShp != nullptr);
}

std::size_t CLctNetEventHandler::socketRecvBuffSize() const
{
    return m_socketRecvBuffSize;
}

void CLctNetEventHandler::setSocketRecvBuffSize(const std::size_t size)
{
    m_socketRecvBuffSize = size;
}

LCT_ERR_CODE CLctNetEventHandler::resetSocketRecvBuffSize()
{
    if (!isInitiated()) {
        return LCT_FAIL;
    }
    return m_socketShp->resetSocketRecvBuffSize(m_socketRecvBuffSize);
}

bool CLctNetEventHandler::isSocketConnected() const
{
    if (!isInitiated()) {
        return false;
    }
    return m_socketShp->getSocketState() == ELctSocketState::SOCKET_STATE_CONNECTED;
}

int32_t CLctNetEventHandler::socketFd() const
{
    return nullptr == m_socketShp ? LCT_INVALID_SOCKET_FD : m_socketShp->getSocketFd();
}

LCT_ERR_CODE CLctNetEventHandler::init()
{
    LOG_DEBUG_MARK();
    return onStart();
}

LCT_ERR_CODE CLctNetEventHandler::stop()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr != m_socketShp) {
        const int32_t socketFd = m_socketShp->getSocketFd();

        errCode = onStop();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to process onStop event for socket(" << socketFd << ")" << ErrCodeFormat(errCode);
            return errCode;
        }

        errCode = m_socketShp->stopService();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to shut down socket(" << socketFd << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
        m_socketShp.reset();
    }

    return errCode;
}

LCT_ERR_CODE CLctNetEventHandler::onStop()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    return errCode;
}

LCT_ERR_CODE CLctNetEventHandler::onAccept(const CLctNetSessionHandlerShp& sessionHandler)
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctNetEventHandler::processRecvBuffData(CLctSocketBufferShp& messageShp)
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctNetEventHandler::doAcceptCompletionHandle(const CLctSocket& socket)
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctNetEventHandler::doReadCompletionHandle()
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctNetEventHandler::doWriteCompletionHandle()
{
    LOG_DEBUG_MARK();
    return LCT_SUCCESS;
}
