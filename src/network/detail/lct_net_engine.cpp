/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_engine.cpp
 * @version     1.0
 * @date        Aug 19, 2017 10:45:30 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_net_engine.h"
#include "socket/lct_base_socket.h"
#include "lct_log.h"

CLctNetEngine::CLctNetEngine()
{
    LOG_DEBUG_MARK();
}

CLctNetEngine::~CLctNetEngine()
{
    LOG_DEBUG_MARK();
}

LCT_ERR_CODE CLctNetEngine::startService()
{
    LOG_DEBUG_MARK();

    if (m_isServing || (nullptr != m_workerGroup)) {
        LOG_ERROR << "Net processor has been initiated already ";
        return LCT_FAIL;
    }

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_workerGroup = std::make_shared<CLctThreadpool>();

    errCode = m_workerGroup->init();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to init worker thread pool " << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = m_workerGroup->start();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to start worker thread pool " << ErrCodeFormat(errCode);
        return errCode;
    }

    m_isServing = true;

    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctNetEngine::stopService()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (nullptr != m_workerGroup) {
        errCode = m_workerGroup->shutdown();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to shut down worker thread pool " << ErrCodeFormat(errCode);
            return errCode;
        }
        m_workerGroup.reset();
    }

    m_isServing = false;

    return LCT_SUCCESS;
}
//
//LCT_ERR_CODE CLctNetEngine::appendReadEventHandler(const CLctSocketShp& socketShp,
//        const CLctNetAsyncReadEventShp& asyncEvent)
//{
//    LOG_DEBUG_MARK();
//
//    const int32_t socketFd = socketShp->getSocketFd();
//    CLctSocketMap::const_iterator itRe = m_socketMap.find(socketFd);
//    if (m_socketMap.end() == itRe) {
//        m_socketMap[socketFd] = socketShp;
//    }
//
//    CLctNetAsyncReadEventMap::iterator it = m_readAsyncHandlerMap.find(socketFd);
//    if (m_readAsyncHandlerMap.end() != it) {
//        LOG_DEBUG<< "socketFd(" << socketFd << ") exists in map";
////      m_asyncEventMap.erase(it);
//    }
//    m_readAsyncHandlerMap[socketFd] = asyncEvent;
//
//    return LCT_SUCCESS;
//}
//
//LCT_ERR_CODE CLctNetEngine::appendWriteEventHandler(const CLctSocketShp& socketShp,
//        const CLctNetAsyncReadEventShp& asyncEvent)
//{
//    LOG_DEBUG_MARK();
//
//    const int32_t socketFd = socketShp->getSocketFd();
//    CLctSocketMap::const_iterator itRe = m_socketMap.find(socketFd);
//    if (m_socketMap.end() == itRe) {
//        m_socketMap[socketFd] = socketShp;
//    }
//
//    CLctNetAsyncReadEventMap::iterator it = m_writeAsyncHandlerMap.find(socketFd);
//    if (m_writeAsyncHandlerMap.end() != it) {
//        LOG_DEBUG<< "socketFd(" << socketFd << ") exists in map";
////      m_asyncEventMap.erase(it);
//    }
//    m_writeAsyncHandlerMap[socketFd] = asyncEvent;
//
//    return LCT_SUCCESS;
//}

