/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_connect.cpp
 * @version     1.0
 * @date        Aug 25, 2017 11:33:24 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_net_connect.h"

CLctNetConnect::CLctNetConnect(const std::string& ip, const int16_t port) :
        m_connector(ip, port)
{
    LOG_DEBUG_MARK();
}

CLctNetConnect::CLctNetConnect(const std::string& adapterAddr) :
        m_connector(adapterAddr)
{
    LOG_DEBUG_MARK();
}

CLctNetConnect::~CLctNetConnect()
{
    LOG_DEBUG_MARK();

    stopWork();
}

LCT_ERR_CODE CLctNetConnect::startWork()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = prepareContext();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to prepare the net context" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = m_connector.init();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to start connector service" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctNetConnect::stopWork()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = m_connector.stop();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to stop connector service" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CLctNetConnect::checkWork()
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = startWork();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to start connector service" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

bool CLctNetConnect::isConnected() const
{
    return m_connector.isSocketConnected();
}

LCT_ERR_CODE CLctNetConnect::resetRecvBuffSize()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = m_connector.resetSocketRecvBuffSize();

    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to reset socket recv-buff" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

