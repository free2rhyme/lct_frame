/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_async_connect_event.h
 * @version     1.0
 * @date        Aug 25, 2017 3:58:26 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_ASYNC_CONNECT_EVENT_H_
#define SRC_NETWORK_LCT_NET_ASYNC_CONNECT_EVENT_H_


#include "detail/lct_network_common_define.h"
#include "lct_socket.h"
#include "lct_log.h"

class CLctNetBasicAsyncConnectEvent
{
public:
    CLctNetBasicAsyncConnectEvent()
    {
        //LOG_DEBUG_MARK();
    }

    virtual ~CLctNetBasicAsyncConnectEvent()
    {
        //LOG_DEBUG_MARK();
    }

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctNetBasicAsyncConnectEvent);

    virtual LCT_ERR_CODE onCompletion() = 0;
};

typedef std::shared_ptr<CLctNetBasicAsyncConnectEvent> CLctNetAsyncConnectEventShp;

template <typename HandleType, typename...Args>
class CLctNetAsyncConnectEvent: public CLctNetBasicAsyncConnectEvent
{
};


template <typename HandleType>
class CLctNetAsyncConnectEvent<HandleType>: public CLctNetBasicAsyncConnectEvent
{
public:
    explicit CLctNetAsyncConnectEvent(HandleType&& handle):
        m_onCompletion(handle)
    {
    }

    ~CLctNetAsyncConnectEvent()
    {
        LOG_DEBUG_MARK();
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
    HandleType  m_onCompletion;
};

template <typename HandleType>
class CLctNetAsyncConnectEvent<HandleType, CLctSocket>: public CLctNetBasicAsyncConnectEvent
{
public:
    explicit CLctNetAsyncConnectEvent(HandleType&& handle, const CLctSocket& socket):
        m_onCompletion(handle), m_socket(socket)
    {
    }

    ~CLctNetAsyncConnectEvent()
    {
        LOG_DEBUG_MARK();
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
    HandleType  m_onCompletion;
    CLctSocket  m_socket;
};


#endif /* SRC_NETWORK_LCT_NET_ASYNC_CONNECT_EVENT_H_ */
