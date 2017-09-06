/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_session_handler.cpp
 * @version     1.0
 * @date        Aug 21, 2017 9:07:53 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_net_session_handler.h"
#include "lct_net_service.h"
#include "socket/lct_socket_manager.h"

CLctNetSessionHandler::CLctNetSessionHandler() :
        CLctNetEventHandler()
{
    LOG_DEBUG_MARK();
}

CLctNetSessionHandler::CLctNetSessionHandler(const CLctSocketShp& socketShp) :
        CLctNetEventHandler(socketShp)
{
    LOG_DEBUG_MARK();
}

CLctNetSessionHandler::CLctNetSessionHandler(const std::string& adapterAddr) :
        CLctNetEventHandler(adapterAddr)
{
    LOG_DEBUG_MARK();
}

CLctNetSessionHandler::~CLctNetSessionHandler()
{
    LOG_DEBUG_MARK();
}

LCT_ERR_CODE CLctNetSessionHandler::onStart()
{
    LOG_DEBUG_MARK();

    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctNetSessionHandler::onStop()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr != m_socketShp) {
        const int32_t socketFd = m_socketShp->getSocketFd();

        if (nullptr != m_networkPtr) {
            errCode = m_networkPtr->eraseSessionHandler(socketFd);
            if (LCT_SUCCESS == errCode) {
                LOG_DEBUG << "Successful to erase session handler for socket(" << m_socketShp->getSocketFd() << ")";
            } else {
                LOG_ERROR << "Failed to erase session handler for socket(" << m_socketShp->getSocketFd() << ")";
            }
        } else {
            LOG_ERROR << "net service ptr is nullptr for socket(" << m_socketShp->getSocketFd() << ")";
        }
    }

    return CLctNetEventHandler::onStop();
}

LCT_ERR_CODE CLctNetSessionHandler::processRecvBuffData(CLctSocketBufferShp& messageShp)
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr == m_asyncReadEvent) {
        LOG_WARNG << "Async Read Event Handler is nullptr for socket(" << m_socketShp->getSocketFd() << ")";
        return LCT_READ_EVENT_UNSET;
    }

    errCode = m_asyncReadEvent->extractMessage(messageShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to extract message " << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctNetSessionHandler::postReadEventHandle(CLctNetAsyncReadEventShp& asyncReadEvent)
{
    if (nullptr == asyncReadEvent) {
        LOG_ERROR << "Invalid event object is disallowed";
        return LCT_INVALID_PARAM;
    }

    m_asyncReadEvent = asyncReadEvent;
    return LCT_SUCCESS;
}


LCT_ERR_CODE CLctNetSessionHandler::postWriteEvent(const CLctNetAsyncWriteEventShp& asyncWriteEvent)
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr == asyncWriteEvent) {
        LOG_ERROR << "Invalid event object is disallowed";
        return LCT_INVALID_PARAM;
    }

    std::lock_guard<CWriteEventMutex> lk(m_writeEventMutex);
    if (nullptr == m_asyncWriteEvent) {
        m_asyncWriteEvent = asyncWriteEvent;
        errCode = LCT_NET_ENGINE->postWriteEvent(m_asyncWriteEvent, m_socketShp);
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to post write-event to engine for socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
        return errCode;
    } else {
        errCode = m_asyncWriteEvents.enqueue(asyncWriteEvent);
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR<< "Failed to store write-handle for socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
    }

    return errCode;
}

LCT_ERR_CODE CLctNetSessionHandler::doReadCompletionHandle()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr != m_asyncReadEvent) {
        errCode = LCT_NET_ENGINE->postCompletionEvent(m_asyncReadEvent);
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to post read-event to engine for socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
    } else {
        LOG_WARNG << "asyncReadEventHandle is nullptr for socket(" << m_socketShp->getSocketFd() << ")";
    }

    return errCode;
}


LCT_ERR_CODE CLctNetSessionHandler::doWriteCompletionHandle()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr != m_asyncWriteEvent) {
        errCode = LCT_NET_ENGINE->postCompletionEvent(m_asyncWriteEvent);
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to post read-event to engine for socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }
    } else {
        LOG_WARNG << "asyncReadEventHandle is nullptr for socket(" << m_socketShp->getSocketFd() << ")";
    }

    return processWriteEvent();
}

LCT_ERR_CODE CLctNetSessionHandler::processWriteEvent()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    std::lock_guard<CWriteEventMutex> lk(m_writeEventMutex);
    if (!m_asyncWriteEvents.empty()) {
        errCode = m_asyncWriteEvents.dequeue(m_asyncWriteEvent);
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to post write-event to engine for socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
            return errCode;
        }

        if (nullptr != m_asyncWriteEvent) {
            errCode = LCT_NET_ENGINE->postWriteEvent(m_asyncWriteEvent, m_socketShp);
            if (LCT_SUCCESS != errCode) {
                LOG_ERROR << "Failed to post write-event to engine for socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
                return errCode;
            }
        } else {
            LOG_WARNG << "asyncWriteEventHandle is nullptr for socket(" << m_socketShp->getSocketFd() << ")";
        }
    } else {
        LOG_DEBUG << "There is not an asyncWriteEventHandle in the pending queue for socket(" << m_socketShp->getSocketFd() << ")";
        m_asyncWriteEvent.reset();
    }

    return errCode;
}
