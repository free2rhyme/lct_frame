/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_session_handler.hpp
 * @version     1.0
 * @date        Aug 21, 2017 9:08:04 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_SESSION_HANDLER_HPP_
#define SRC_NETWORK_LCT_NET_SESSION_HANDLER_HPP_

#include "socket/lct_base_socket.h"
#include "detail/lct_net_engine.h"
#include "lct_net_async_read_event.h"
#include "lct_log.h"

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetSessionHandler::asyncReadUntil(CReadStream& buff, const std::size_t maxLength, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    if (!isInitiated()) {
        LOG_WARNG<< "Net service has NOT been initiated yet";
        errCode = init();
        if(LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to start service";
            return errCode;
        }
    }

    m_asyncReadEvent = std::make_shared<CLctNetAsyncReadEvent<ReadHandle, CReadStream, std::size_t>>(std::move(handle),
            buff, maxLength);

    return errCode;
}

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetSessionHandler::asyncReadUntil(CReadStream& buff, const char delim, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    if (!isInitiated()) {
        LOG_WARNG<< "Net service has NOT been initiated yet";
        errCode = init();
        if(LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to start service";
            return errCode;
        }
    }

    m_asyncReadEvent = std::make_shared<CLctNetAsyncReadEvent<ReadHandle, CReadStream, char>>(std::move(handle), buff,
            delim);

    return errCode;
}

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetSessionHandler::asyncReadUntil(CReadStream& buff, const std::string& str, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    if (!isInitiated()) {
        LOG_WARNG<< "Net service has NOT been initiated yet";
        errCode = init();
        if(LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to start service";
            return errCode;
        }
    }

    return LCT_SUCCESS;
}

template <typename CReadStream, typename MatchCondition, typename ReadHandle>
LCT_ERR_CODE CLctNetSessionHandler::asyncReadUntil(CReadStream& buff, MatchCondition&& matchCond, ReadHandle&& handle)
{
    return LCT_SUCCESS;
}

template <typename CWriteStream, typename WriteHandle>
LCT_ERR_CODE CLctNetSessionHandler::asyncWrite(const CWriteStream& data, WriteHandle&& handle)
{
    LOG_DEBUG_MARK();

    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (!isInitiated()) {
        LOG_WARNG<< "Event Handler has NOT been initiated yet";
        errCode = init();
        if(LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to init session handle";
            return errCode;
        }
    }

    CLctNetAsyncWriteEventShp asyncWriteEvent = std::make_shared<CLctNetAsyncWriteEvent<WriteHandle, CWriteStream>>(std::move(handle), data);

    errCode = postWriteEvent(asyncWriteEvent);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR<< "Failed to store write-handle for socket(" << m_socketShp->getSocketFd() << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

#endif /* SRC_NETWORK_LCT_NET_SESSION_HANDLER_HPP_ */
