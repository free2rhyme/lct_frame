/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_connect.h
 * @version     1.0
 * @date        Aug 25, 2017 11:33:13 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_CONNECT_H_
#define SRC_NETWORK_LCT_NET_CONNECT_H_

#include "detail/lct_net_work.h"
#include "event/lct_net_connector.h"

class CLctNetConnect final: public CLctNetWork
{
public:
    explicit CLctNetConnect(const std::string& ip, const int16_t port);
    explicit CLctNetConnect(const std::string& adapterAddr); //"0.0.0.0:80"
    ~CLctNetConnect();

public:
    bool isConnected() const;

public:
    template <typename ConnectHandle>
    LCT_ERR_CODE asyncConnect(ConnectHandle&& handle);

    template <typename CReadStream, typename ReadHandle>
    LCT_ERR_CODE asyncReadUntil(CReadStream& buff, const std::size_t maxLength, ReadHandle&& handle);

    template <typename CReadStream, typename ReadHandle>
    LCT_ERR_CODE asyncReadUntil(CReadStream& buff, const char delim, ReadHandle&& handle);

    template <typename CReadStream, typename ReadHandle>
    LCT_ERR_CODE asyncReadUntil(CReadStream& buff, const std::string& delim, ReadHandle&& handle);

    template <typename CReadStream, typename MatchCondition, typename ReadHandle>
    LCT_ERR_CODE asyncReadUntil(CReadStream& buff, MatchCondition&& matchCond, ReadHandle&& handle);

    template <typename CWriteStream, typename WriteHandle>
    LCT_ERR_CODE asyncWrite(const CWriteStream& data, WriteHandle&& handle);

private:
    LCT_ERR_CODE startWork() override;
    LCT_ERR_CODE stopWork() override;
    LCT_ERR_CODE checkWork() override;
    LCT_ERR_CODE resetRecvBuffSize() override;

private:
    CLctNetConnector m_connector;
};

#include "detail/lct_net_connect.hpp"

#endif /* SRC_NETWORK_LCT_NET_CONNECT_H_ */
