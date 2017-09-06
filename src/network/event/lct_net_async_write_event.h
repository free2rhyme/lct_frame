/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_async_write_event.h
 * @version     1.0
 * @date        Aug 24, 2017 4:18:59 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_ASYNC_WRITE_EVENT_H_
#define SRC_NETWORK_LCT_NET_ASYNC_WRITE_EVENT_H_

#include "detail/lct_network_common_define.h"
#include "socket/lct_base_socket.h"
#include "lct_log.h"
#include "lct_queue.h"

class CLctNetBasicAsyncWriteEvent
{
public:
    CLctNetBasicAsyncWriteEvent()
    {
        //LOG_DEBUG_MARK();
    }

    virtual ~CLctNetBasicAsyncWriteEvent()
    {
        //LOG_DEBUG_MARK();
    }

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctNetBasicAsyncWriteEvent);

    virtual LCT_ERR_CODE sendMessage(CLctSocketShp& socketShp)  = 0;
    virtual LCT_ERR_CODE onCompletion() = 0;
};

typedef std::shared_ptr<CLctNetBasicAsyncWriteEvent> CLctNetAsyncWriteEventShp;

template <typename HandleType, typename MessageStream>
class CLctNetAsyncWriteEvent: public CLctNetBasicAsyncWriteEvent
{
};

template <typename HandleType>
class CLctNetAsyncWriteEvent<HandleType, std::shared_ptr<std::string>>: public CLctNetBasicAsyncWriteEvent
{
private:
    typedef std::shared_ptr<std::string>  CMessageType;

public:
    explicit CLctNetAsyncWriteEvent(HandleType&& handle, const CMessageType& message):
            m_onCompletion(std::move(handle)), m_message(message)
    {
        LOG_DEBUG_MARK();
    }

    ~CLctNetAsyncWriteEvent()
    {
        LOG_DEBUG_MARK();
    }

    LCT_ERR_CODE sendMessage(CLctSocketShp& socketShp) override
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        if (nullptr == socketShp){
            LOG_ERROR << "Couldn't send message to a invalid sockt" << ErrCodeFormat(LCT_INVALID_POINTER);
            return LCT_INVALID_POINTER;
        }

        errCode = socketShp->send(m_message->c_str(), m_message->size());
        if (LCT_SUCCESS != errCode){
            LOG_ERROR << "Failed to send message to remote" << ErrCodeFormat(errCode);
            return errCode;
        }

        return errCode;
    }

    LCT_ERR_CODE onCompletion() override
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        m_onCompletion();

        return errCode;
    }

private:
    CMessageType    m_message;
    HandleType      m_onCompletion;
};

template <typename HandleType>
class CLctNetAsyncWriteEvent<HandleType, std::string>: public CLctNetBasicAsyncWriteEvent
{
private:
    typedef std::shared_ptr<std::string>  CMessageType;

public:
    explicit CLctNetAsyncWriteEvent(HandleType&& handle, const std::string& message):
            m_onCompletion(std::move(handle))
    {
        m_message = std::make_shared<std::string>(message);
        LOG_DEBUG_MARK();
    }

    ~CLctNetAsyncWriteEvent()
    {
        LOG_DEBUG_MARK();
    }

    LCT_ERR_CODE sendMessage(CLctSocketShp& socketShp) override
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        if (nullptr == socketShp){
            LOG_ERROR << "Couldn't send message to a invalid sockt" << ErrCodeFormat(LCT_INVALID_POINTER);
            return LCT_INVALID_POINTER;
        }

        errCode = socketShp->send(m_message->c_str(), m_message->size());
        if (LCT_SUCCESS != errCode){
            LOG_ERROR << "Failed to send message to remote" << ErrCodeFormat(errCode);
            return errCode;
        }

        return errCode;
    }

    LCT_ERR_CODE onCompletion() override
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        m_onCompletion();

        return errCode;
    }

private:
    CMessageType    m_message;
    HandleType      m_onCompletion;
};

#endif /* SRC_NETWORK_LCT_NET_ASYNC_WRITE_EVENT_H_ */
