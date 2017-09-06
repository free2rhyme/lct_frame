/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_socket_manager.cpp
 * @version     1.0
 * @date        Aug 15, 2017 8:34:30 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_socket_manager.h"
#include "lct_base_socket.h"
#include "lct_socket_util.h"
#include "lct_log.h"
#include <sys/epoll.h>
#include <errno.h>

CLctSocketManager::CLctSocketManager()
{
}

CLctSocketManager::~CLctSocketManager()
{
    if (nullptr != m_ioSocketThread) {
        m_ioSocketThread->join();
    }
}

LCT_ERR_CODE CLctSocketManager::init()
{
    LOG_DEBUG_MARK();

    if (nullptr == m_ioSocketThread) {
        m_ioSocketThread = std::make_shared<CAsyncThreadType>(&CLctSocketManager::run, this);
    } else {
        LOG_ERROR << "Socket Manager has been initiated and running";
        return LCT_FAIL;
    }

    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctSocketManager::run()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_epollFd = epoll_create1(EPOLL_CLOEXEC);
    if (m_epollFd <= 0) {
        LOG_ERROR << "Failed to create a epoll-fd as result(" << m_epollFd << ") errno(" << errno << ")";
        return LCT_FAIL;
    }

    m_running = true;

    while (m_running) {

        LOG_DEBUG_MARK();

        errCode = select();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to onCompletion select func" << ErrCodeFormat(errCode);
            break;
        }
    }

    return errCode;
}

LCT_ERR_CODE CLctSocketManager::stop()
{
    LOG_DEBUG_MARK();
    m_running = false;
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctSocketManager::select()
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode                            = LCT_SUCCESS;
    epoll_event events[LCT_EPOLL_EVENT_MAX_SIZE]    = { 0 };
    const int32_t eventsMaxSize                     = LCT_EPOLL_EVENT_MAX_SIZE;

    if (LCT_INVALID_SOCKET_FD != m_epollFd) {

        LOG_DEBUG_MARK();

        const int32_t activeEventSize = epoll_wait(m_epollFd, events, eventsMaxSize, -1);

        if (activeEventSize < 0) {
            LOG_WARNG << "There is events(" << activeEventSize << ") returned from epoll with errno(" << errno << ")";
        } else if (activeEventSize == 0) {
            LOG_WARNG << "There is NONE events returned from epoll with errno(" << errno << ")";
        } else {
            for(int32_t i = 0; i < activeEventSize; ++i) {
                errCode = processSocketEvent(events[i].data.fd, events[i].events);
                if (LCT_SUCCESS != errCode) {
                    LOG_ERROR << "Failed to process socket(" << events[i].data.fd << ")" << ErrCodeFormat(errCode);
                }
            }
        }
    } else {
        LOG_ERROR << "Failed to epoll wait service due to invalid epoll FD." << ErrCodeFormat(LCT_INVALID_SOCKET);
        return LCT_INVALID_SOCKET;
    }
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctSocketManager::addSocket(const CLctSocketShp& socketShp)
{
    LOG_DEBUG_MARK();
    return addSocket(socketShp, EPOLLIN | EPOLLET);
}

LCT_ERR_CODE CLctSocketManager::addSocket(const CLctSocketShp& socketShp, const uint32_t eventType)
{
    LOG_DEBUG_MARK();

    if (!m_running) {
        LOG_ERROR << "Socket Manager is not running yet";
        return LCT_NOT_INITIATED_YET;
    }

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr == socketShp) {
        LOG_ERROR << "Failed to add nullptr socket";
        return LCT_INVALID_SOCKET;
    }

    const int32_t socketFd = socketShp->getSocketFd();

    errCode = saveSocket(socketShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to save socket(" << socketFd << ")";
        return errCode;
    }

    do {
        epoll_event event;
        event.data.fd = socketFd;
        event.events  = eventType;

        errCode = LCTSetSocketNonblocking(socketFd);
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to set socket(" << socketFd << ") to be nonblocking";
            break;
        }

        LOG_DEBUG<< "Adding socketFd(" << socketFd << ") to epoll-ctl epollFd(" << m_epollFd << ")" <<
        " with eventType(" << std::hex << eventType << std::oct << ")";

        int32_t result = epoll_ctl(m_epollFd, EPOLL_CTL_ADD, socketFd, &event);
        if (result < 0) {
            LOG_ERROR << "Failed to add socket(" << socketFd << ") by epoll CTL, result(" << result << ") errno(" << errno << ")";
            errCode = LCT_FAIL;
        }
    } while (0);

    if (LCT_SUCCESS != errCode) {
        if (LCT_SUCCESS == eraseSocket(socketFd)) {
            LOG_DEBUG << "Successful to erase socket(" << socketFd << ") from socket manager";
        } else {
            LOG_DEBUG << "Failed to erase socket(" << socketFd << ") from socket manager";
        }
        return errCode;
    }

    return errCode;
}


LCT_ERR_CODE CLctSocketManager::findSocket(const int32_t socketFd, CLctSocketShp& socketShp) const
{
    LOG_DEBUG_MARK();
    std::lock_guard<CLctSocketMapMutex> lk(m_socketMapMutex);
    auto itRe = m_socketShpMap.find(socketFd);
    if (m_socketShpMap.end() == itRe) {
        LOG_ERROR << "Failed to find socket(" << socketFd << ") in socket manager";
        return LCT_NOT_SUCH_RECORD;
    }
    socketShp = itRe->second;
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctSocketManager::eraseSocket(const int32_t socketFd)
{
    LOG_DEBUG_MARK();
    std::lock_guard<CLctSocketMapMutex> lk(m_socketMapMutex);
    auto it = m_socketShpMap.find(socketFd);
    if (m_socketShpMap.end() == it) {
        LOG_ERROR << "Failed to find socket(" << socketFd << ") in socket manager";
        return LCT_NOT_SUCH_RECORD;
    }
    m_socketShpMap.erase(it);

    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctSocketManager::saveSocket(const CLctSocketShp& socketShp)
{
    LOG_DEBUG_MARK();
    std::lock_guard<CLctSocketMapMutex> lk(m_socketMapMutex);

    const int32_t socketFd = socketShp->getSocketFd();
    auto itRe = m_socketShpMap.find(socketFd);
    if (m_socketShpMap.end() != itRe) {
        LOG_ERROR << "There is already object for socket(" << socketFd << ")";
        return LCT_RECORD_EXISTS;
    }
    m_socketShpMap[socketFd] = socketShp;
    LOG_DEBUG << "Successful to save socket object for socketFd(" << socketFd << ")";
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctSocketManager::processSocketEvent(const int32_t socketFd, const uint32_t epollEvents)
{
    LOG_DEBUG_MARK();
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    CLctSocketShp socketShp;
    errCode = findSocket(socketFd, socketShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to find socket(" << socketFd << ") in socket manager";
        return errCode;
    }

    if ((epollEvents & EPOLLIN) == EPOLLIN) {
        return socketShp->doProcessRead();
    }

    if ((epollEvents & EPOLLOUT) == EPOLLOUT) {
        return socketShp->doProcessWrite();
    }

    LOG_ERROR << "Failed to catch epoll event(" << std::hex << epollEvents << std::oct << ")";
    return LCT_FAIL;
}

