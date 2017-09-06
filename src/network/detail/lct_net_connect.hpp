/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_connect.hpp
 * @version     1.0
 * @date        Aug 25, 2017 2:19:18 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_CONNECT_HPP_
#define SRC_NETWORK_LCT_NET_CONNECT_HPP_

template <typename ConnectHandle>
LCT_ERR_CODE CLctNetConnect::asyncConnect(ConnectHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }
    return m_connector.asyncConnect(std::move(handle));
}

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetConnect::asyncReadUntil(CReadStream& buff, const std::size_t maxLength, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }
    return m_connector.asyncReadUntil(buff, maxLength, std::move(handle));
}

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetConnect::asyncReadUntil(CReadStream& buff, const char delim, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }
    return m_connector.asyncReadUntil(buff, delim, std::move(handle));
}

template <typename CReadStream, typename ReadHandle>
LCT_ERR_CODE CLctNetConnect::asyncReadUntil(CReadStream& buff, const std::string& delim, ReadHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }
    return m_connector.asyncReadUntil(buff, delim, std::move(handle));
}

template <typename CReadStream, typename MatchCondition, typename ReadHandle>
LCT_ERR_CODE CLctNetConnect::asyncReadUntil(CReadStream& buff, MatchCondition&& matchCond, ReadHandle&& handle)
{
    return LCT_SUCCESS;
}

template <typename CWriteStream, typename WriteHandle>
LCT_ERR_CODE CLctNetConnect::asyncWrite(const CWriteStream& data, WriteHandle&& handle)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    errCode = checkService();
    if (LCT_SUCCESS != errCode) {
        return errCode;
    }
    return m_connector.asyncWrite(data, std::move(handle));
}


#endif /* SRC_NETWORK_LCT_NET_CONNECT_HPP_ */
