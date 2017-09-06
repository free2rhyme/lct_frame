/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_work.cpp
 * @version     1.0
 * @date        Aug 25, 2017 11:37:14 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "../detail/lct_net_work.h"

#include "../detail/lct_net_context.h"

std::atomic_bool CLctNetWork::m_isContextInited = ATOMIC_VAR_INIT(false);

CLctNetWork::CLctNetWork()
{
}

CLctNetWork::~CLctNetWork()
{
}

std::size_t CLctNetWork::socketRecvBuffSize() const
{
    return m_socketRecvBuffSize;
}

LCT_ERR_CODE CLctNetWork::setSocketRecvBuffSize(const std::size_t size)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_socketRecvBuffSize = size;

    errCode = resetRecvBuffSize();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to reset the size for socket recv-buff" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctNetWork::resetRecvBuffSize()
{
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctNetWork::start()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    if (m_isWorking) {
        LOG_ERROR << "Failed to start the net work due to it is already running" << ErrCodeFormat(errCode);
        return LCT_FAIL;
    }

    errCode = prepareContext();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to prepare the net context" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = startWork();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to prepare for the net work" << ErrCodeFormat(errCode);
        return errCode;
    }

    m_isWorking  = true;

    return errCode;
}

LCT_ERR_CODE CLctNetWork::stop()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    if (!m_isWorking) {
        LOG_ERROR << "Failed to stop the net work due to it is already stopped" << ErrCodeFormat(errCode);
        return LCT_FAIL;
    }

    m_isWorking  = false;

    errCode = stopWork();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to stop the net work" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

bool CLctNetWork::isContextInited()
{
    return m_isContextInited.load();
}

LCT_ERR_CODE CLctNetWork::prepareContext()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    if (!m_isContextInited.load()) {
        errCode = LCT_NET_CONTEXT->init();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to init the net context" << ErrCodeFormat(errCode);
            return errCode;
        }
        m_isContextInited.store(true);
    }
    return errCode;
}

LCT_ERR_CODE CLctNetWork::cleanupContext()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (m_isContextInited.load()) {
        errCode = LCT_NET_CONTEXT->stop();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to clean up the net context" << ErrCodeFormat(errCode);
            return errCode;
        }
        m_isContextInited.store(false);
    }

    return errCode;
}

bool CLctNetWork::isWorking() const
{
    return m_isWorking;
}

LCT_ERR_CODE CLctNetWork::checkService()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = prepareContext();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to prepare the net context" << ErrCodeFormat(errCode);
        return errCode;
    }

    if (!m_isWorking) {
        errCode = checkWork();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to prepare for the net work" << ErrCodeFormat(errCode);
            return errCode;
        }
        m_isWorking = true;
    }

    return errCode;
}


