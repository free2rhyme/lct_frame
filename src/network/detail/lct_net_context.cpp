/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_context.cpp
 * @version     1.0
 * @date        Aug 25, 2017 10:31:12 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_net_context.h"
#include "detail/lct_net_engine.h"
#include "socket/lct_socket_manager.h"
#include "lct_log.h"

LCT_ERR_CODE CLctNetContext::init()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (!isInitiated()) {

        std::lock_guard<CNetContextMutex> lk(m_mutex);

        if (isInitiated()) {
            return errCode;
        }

        errCode = LCT_SOCKET_MGR->init();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to init the socket manager" << ErrCodeFormat(errCode);
            return errCode;
        }

        do {
            errCode = LCT_NET_ENGINE->startService();
            if (LCT_SUCCESS != errCode) {
                LOG_ERROR << "Failed to start LCT NET Engine" << ErrCodeFormat(errCode);
                break;
            }
        } while (0);

        if (LCT_SUCCESS != errCode) {
            LCT_ERR_CODE err2Code = LCT_SOCKET_MGR->stop();
            if (LCT_SUCCESS != err2Code) {
                LOG_ERROR << "Failed to stop the socket manager" << ErrCodeFormat(err2Code);
            }
            return errCode;
        }

        m_isInitiated.store(true);
    }

    return errCode;
}

LCT_ERR_CODE CLctNetContext::stop()
{
    LOG_DEBUG_MARK();

    std::lock_guard<CNetContextMutex> lk(m_mutex);

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = LCT_NET_ENGINE->stopService();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to stop the LCT NET ENGINE" << ErrCodeFormat(errCode);
    }

    errCode = LCT_SOCKET_MGR->stop();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to stop the socket manager" << ErrCodeFormat(errCode);
    }

    m_isInitiated.store(false);

    return errCode;
}

bool CLctNetContext::isInitiated() const
{
    return m_isInitiated.load();
}

CLctNetContext::CLctNetContext()
{
}

CLctNetContext::~CLctNetContext()
{
    stop();
}


