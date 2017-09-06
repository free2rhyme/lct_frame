/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_service.h
 * @version     1.0
 * @date        Aug 18, 2017 10:55:20 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_SERVICE_H_
#define SRC_NETWORK_LCT_NET_SERVICE_H_

#include "detail/lct_network_common_define.h"
#include "detail/lct_net_work.h"
#include "event/lct_net_session_handler.h"
#include "event/lct_net_acceptor.h"
#include <unordered_map>
#include <memory>

class CLctNetService final: public CLctNetWork
{
public:
    explicit CLctNetService(const std::string& ip, const int16_t port);
    ~CLctNetService();

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctNetService);

public:
    LCT_ERR_CODE registerSessionHandler(const int32_t socketFd, const CLctNetSessionHandlerShp& sessionHandlerShp);
    LCT_ERR_CODE sessionHandler(const int32_t socketFd, CLctNetSessionHandlerShp& sessionHandlerShp) const;
    LCT_ERR_CODE eraseSessionHandler(const int32_t socketFd);

public:
    template <typename AcceptHandler>
    LCT_ERR_CODE asyncAccept(AcceptHandler&& handle);

    /**************************************
     *  Post and apply to all sockets
     **************************************/
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

    /*************************************************************************************
     *  Post event to the specified socket
     *************************************************************************************/
    template <typename CReadStream, typename ReadHandle>
    LCT_ERR_CODE asyncReadUntil(const int32_t fd, CReadStream& buff, const std::size_t maxLength, ReadHandle&& handle);

    template <typename CReadStream, typename ReadHandle>
    LCT_ERR_CODE asyncReadUntil(const int32_t fd, CReadStream& buff, const char delim, ReadHandle&& handle);

    template <typename CReadStream, typename ReadHandle>
    LCT_ERR_CODE asyncReadUntil(const int32_t fd, CReadStream& buff, const std::string& delim, ReadHandle&& handle);

    template <typename CReadStream, typename MatchCondition, typename ReadHandle>
    LCT_ERR_CODE asyncReadUntil(const int32_t fd, CReadStream& buff, MatchCondition&& matchCond, ReadHandle&& handle);

    template <typename CWriteStream, typename WriteHandle>
    LCT_ERR_CODE asyncWrite(const int32_t fd, const CWriteStream& data, WriteHandle&& handle);

private:
    LCT_ERR_CODE startWork() override;
    LCT_ERR_CODE stopWork()  override;
    LCT_ERR_CODE checkWork() override;
    LCT_ERR_CODE resetRecvBuffSize() override;

    LCT_ERR_CODE updateReadEvent();
    LCT_ERR_CODE updateWriteEvent();

    LCT_ERR_CODE saveSessionHandler(const int32_t socketFd, const CLctNetSessionHandlerShp& sessionHandlerShp);

private:
    CLctNetAcceptor             m_acceptor;

    CLctNetAsyncReadEventShp    m_asyncReadEvent;
    CLctNetAsyncWriteEventShp   m_asyncWriteEvent;

    typedef std::unordered_map<int32_t, CLctNetSessionHandlerShp> CLctSessionHandlerMap;
    typedef std::mutex          CSessionHandlerMapMutex;

    mutable CSessionHandlerMapMutex  m_sessionHandlerMapMutex;
    CLctSessionHandlerMap            m_sessionHandlerMap;
};

#include "detail/lct_net_service.hpp"

#endif /* SRC_NETWORK_LCT_NET_SERVICE_H_ */
