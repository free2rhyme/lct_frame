/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_service.hpp
 * @version     1.0
 * @date        Aug 18, 2017 4:11:18 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_SERVICE_HPP_
#define SRC_NETWORK_LCT_NET_SERVICE_HPP_

#include "lct_log.h"

template <typename AcceptHandler>
LCT_ERR_CODE CLctNetService::asyncAccept(AcceptHandler&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    return m_acceptor.asyncAccept(handle);
}

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetService::asyncReadUntil(CReadStream& buff, const std::size_t maxLength, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    m_asyncReadEvent = std::make_shared<CLctNetAsyncReadEvent<ReadHandle, CReadStream, std::size_t>>(std::move(handle), buff, maxLength);

    updateReadEvent();

    return errCode;
}

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetService::asyncReadUntil(CReadStream& buff, const char delim, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    m_asyncReadEvent = std::make_shared<CLctNetAsyncReadEvent<ReadHandle, CReadStream, char>>(std::move(handle), buff, delim);

    updateReadEvent();

    return errCode;
}

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetService::asyncReadUntil(CReadStream& buff, const std::string& delim, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    return LCT_SUCCESS;
}

template <typename CReadStream, typename MatchCondition, typename ReadHandle>
LCT_ERR_CODE CLctNetService::asyncReadUntil(CReadStream& buff, MatchCondition&& matchCond, ReadHandle&& handle)
{
    return LCT_SUCCESS;
}

template <typename CWriteStream, typename WriteHandle>
LCT_ERR_CODE CLctNetService::asyncWrite(const CWriteStream& data, WriteHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    m_asyncWriteEvent = std::make_shared<CLctNetAsyncWriteEvent<WriteHandle, CWriteStream>>(std::move(handle), data);

    updateWriteEvent();

    return errCode;
}

/*************************************************************************************
 *  Operation on specified socket
 *************************************************************************************/
template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetService::asyncReadUntil(const int32_t fd, CReadStream& buff, const std::size_t maxLength, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    CLctNetSessionHandlerShp sessionHandlerShp;
    errCode = sessionHandler(fd, sessionHandlerShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to get the right session handler for fd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = sessionHandlerShp->asyncReadUntil(buff, maxLength, std::move(handle));
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to post the event to session handler for fd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetService::asyncReadUntil(const int32_t fd, CReadStream& buff, const char delim, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    CLctNetSessionHandlerShp sessionHandlerShp;
    errCode = sessionHandler(fd, sessionHandlerShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to get the right session handler for fd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = sessionHandlerShp->asyncReadUntil(buff, delim, std::move(handle));
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to post the event to session handler for fd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetService::asyncReadUntil(const int32_t fd, CReadStream& buff, const std::string& delim, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    CLctNetSessionHandlerShp sessionHandlerShp;
    errCode = sessionHandler(fd, sessionHandlerShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to get the right session handler for fd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = sessionHandlerShp->asyncReadUntil(buff, delim, std::move(handle));
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to post the event to session handler for fd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CReadStream, typename MatchCondition, typename ReadHandle>
LCT_ERR_CODE CLctNetService::asyncReadUntil(const int32_t fd, CReadStream& buff, MatchCondition&& matchCond, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    CLctNetSessionHandlerShp sessionHandlerShp;
    errCode = sessionHandler(fd, sessionHandlerShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to get the right session handler for fd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = sessionHandlerShp->asyncReadUntil(buff, matchCond, std::move(handle));
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to post the event to session handler for fd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}

template <typename CWriteStream, typename WriteHandle>
LCT_ERR_CODE CLctNetService::asyncWrite(const int32_t fd, const CWriteStream& data, WriteHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    CLctNetSessionHandlerShp sessionHandlerShp;
    errCode = sessionHandler(fd, sessionHandlerShp);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to get the right session handler for fd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    errCode = sessionHandlerShp->asyncWrite(data, std::move(handle));
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to post the event to session handler for fd(" << fd << ")" << ErrCodeFormat(errCode);
        return errCode;
    }

    return errCode;
}



#endif /* SRC_NETWORK_LCT_NET_SERVICE_HPP_ */
