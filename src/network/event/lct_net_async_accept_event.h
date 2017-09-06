/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_async_accept_event.h
 * @version     1.0
 * @date        Aug 25, 2017 9:24:56 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_ASYNC_ACCEPT_EVENT_H_
#define SRC_NETWORK_LCT_NET_ASYNC_ACCEPT_EVENT_H_

#include "detail/lct_network_common_define.h"
#include "lct_log.h"
#include "lct_socket.h"

class CLctNetBasicAsyncAcceptEvent
{
public:
    CLctNetBasicAsyncAcceptEvent()
    {
        //LOG_DEBUG_MARK();
    }

    virtual ~CLctNetBasicAsyncAcceptEvent()
    {
        //LOG_DEBUG_MARK();
    }

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctNetBasicAsyncAcceptEvent);

    virtual LCT_ERR_CODE onCompletion() = 0;

    virtual void setSocket(const CLctSocket& socket) {
        ;
    }
};

typedef std::shared_ptr<CLctNetBasicAsyncAcceptEvent> CLctNetAsyncAcceptEventShp;

template <typename HandleType, typename...Args>
class CLctNetAsyncAcceptEvent: public CLctNetBasicAsyncAcceptEvent
{
};


template <typename HandleType>
class CLctNetAsyncAcceptEvent<HandleType>: public CLctNetBasicAsyncAcceptEvent
{
public:
    explicit CLctNetAsyncAcceptEvent(HandleType&& handle):
        m_onCompletion(handle)
    {
    }

    ~CLctNetAsyncAcceptEvent()
    {
        LOG_DEBUG_MARK();
    }

    void setSocket(const CLctSocket& socket) override
    {
        ;
    }

    LCT_ERR_CODE onCompletion() override
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        if (nullptr != m_onCompletion) {
            m_onCompletion();
        } else {
            LOG_ERROR << "Invalid pointer for onCompletion handle";
            return LCT_INVALID_POINTER;
        }

        return errCode;
    }

private:
    HandleType  m_onCompletion = nullptr;
};


template <typename HandleType>
class CLctNetAsyncAcceptEvent<HandleType, CLctSocket>: public CLctNetBasicAsyncAcceptEvent
{
public:
    explicit CLctNetAsyncAcceptEvent(HandleType&& handle):
        m_onCompletion(handle)
    {
    }

    ~CLctNetAsyncAcceptEvent()
    {
        LOG_DEBUG_MARK();
    }

    void setSocket(const CLctSocket& socket) override
    {
        m_socket = socket;
    }

    LCT_ERR_CODE onCompletion() override
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        if (nullptr != m_onCompletion) {
            m_onCompletion(m_socket);
        } else {
            LOG_ERROR << "Invalid pointer for onCompletion handle";
            return LCT_INVALID_POINTER;
        }

        return errCode;
    }

private:
    HandleType  m_onCompletion  = nullptr;
    CLctSocket  m_socket{ELctSocketType::SOCKET_TYPE_TCP_SERVER};
};



#endif /* SRC_NETWORK_LCT_NET_ASYNC_ACCEPT_EVENT_H_ */
