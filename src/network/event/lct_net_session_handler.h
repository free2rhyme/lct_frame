/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_session_handler.h
 * @version     1.0
 * @date        Aug 21, 2017 9:07:39 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_SESSION_HANDLER_H_
#define SRC_NETWORK_LCT_NET_SESSION_HANDLER_H_

#include "lct_net_event_handler.h"
#include "lct_queue.h"

class CLctNetSessionHandler: public CLctNetEventHandler
{
public:
    explicit CLctNetSessionHandler();
    explicit CLctNetSessionHandler(const CLctSocketShp& socketShp);
    explicit CLctNetSessionHandler(const std::string& adapterAddr); //"0.0.0.0:80"
    virtual ~CLctNetSessionHandler();

public:
    virtual LCT_ERR_CODE onStart() override;
    virtual LCT_ERR_CODE onStop() override;

    virtual LCT_ERR_CODE processRecvBuffData(CLctSocketBufferShp& messageShp) override;

    virtual LCT_ERR_CODE doReadCompletionHandle() override;
    virtual LCT_ERR_CODE doWriteCompletionHandle() override;

    LCT_ERR_CODE postReadEventHandle(CLctNetAsyncReadEventShp& eventShp);
    LCT_ERR_CODE postWriteEvent(const CLctNetAsyncWriteEventShp& asyncWriteEvent);

private:
    LCT_ERR_CODE processWriteEvent();

public:
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
    CLctNetAsyncReadEventShp      m_asyncReadEvent;
    CLctNetAsyncWriteEventShp     m_asyncWriteEvent;

    typedef CLctQueue<CLctNetAsyncWriteEventShp> LctAsyncWriteEventContainer;

    LctAsyncWriteEventContainer   m_asyncWriteEvents;

    typedef std::mutex            CWriteEventMutex;
    mutable CWriteEventMutex      m_writeEventMutex;
};

#include "lct_net_session_handler.hpp"

#endif /* SRC_NETWORK_LCT_NET_SESSION_HANDLER_H_ */
