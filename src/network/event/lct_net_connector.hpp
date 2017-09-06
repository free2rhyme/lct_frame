/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_connector.hpp
 * @version     1.0
 * @date        Aug 18, 2017 2:08:27 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_CONNECTOR_HPP_
#define SRC_NETWORK_LCT_NET_CONNECTOR_HPP_

#include "lct_log.h"

template <typename ConnectHandle>
LCT_ERR_CODE CLctNetConnector::asyncConnect(ConnectHandle&& handle)
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

    m_asyncConnectEvent = std::make_shared<CLctNetAsyncConnectEvent<ConnectHandle, CLctSocket>>(std::move(handler));

    return errCode;
}

#endif /* SRC_NETWORK_LCT_NET_CONNECTOR_HPP_ */
