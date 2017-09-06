/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_async_handler.h
 * @version     1.0
 * @date        Aug 21, 2017 8:35:37 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_ASYNC_READ_EVENT_H_
#define SRC_NETWORK_LCT_NET_ASYNC_READ_EVENT_H_

#include "detail/lct_network_common_define.h"
#include "lct_log.h"
#include "lct_queue.h"

class CLctNetBasicAsyncReadEvent
{
public:
    CLctNetBasicAsyncReadEvent()
    {
        //LOG_DEBUG_MARK();
    }

    virtual ~CLctNetBasicAsyncReadEvent()
    {
        //LOG_DEBUG_MARK();
    }

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctNetBasicAsyncReadEvent);

    virtual LCT_ERR_CODE extractMessage(CLctSocketBufferShp& bytesRecvInBuff) = 0;
    virtual LCT_ERR_CODE onCompletion() = 0;
};

typedef std::shared_ptr<CLctNetBasicAsyncReadEvent> CLctNetAsyncReadEventShp;

template <typename HandleType, typename BufferStream, typename DelimType>
class CLctNetAsyncReadEvent: public CLctNetBasicAsyncReadEvent
{
private:
    explicit CLctNetAsyncReadEvent(HandleType&&, BufferStream&, const DelimType&)
    {
    }
    ~CLctNetAsyncReadEvent()
    {
    }
};

template <typename HandleType, typename BufferStream>
class CLctNetAsyncReadEvent<HandleType, BufferStream, std::size_t>: public CLctNetBasicAsyncReadEvent
{
public:
    explicit CLctNetAsyncReadEvent(HandleType&& handle, BufferStream& stream, const std::size_t size):
            m_splittingSize(size), m_onCompletion(std::move(handle)), m_streamBuffer(stream)
    {
        if (m_streamBuffer.size() < size) {
            m_streamBuffer.resize(size);
        }
        LOG_DEBUG_MARK();
    }

    ~CLctNetAsyncReadEvent()
    {
        LOG_DEBUG_MARK();
    }

    LCT_ERR_CODE extractMessage(CLctSocketBufferShp& bytesRecvInBuff) override
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        LOG_DEBUG << "bytesRecvInBuff-size(" << bytesRecvInBuff->size() << ") and limited m_splittingSize(" << m_splittingSize << ")";

        printf("m_streamBuffer(%p), begin(%p)\n", &m_streamBuffer, m_streamBuffer.begin());
        if (bytesRecvInBuff->size() >= m_splittingSize){
            const std::size_t loopSize = bytesRecvInBuff->loopSize();
            LOG_DEBUG << "loopSize(" << loopSize << ")";
            if (loopSize >= m_splittingSize) {
                std::copy_n(bytesRecvInBuff->first(), m_splittingSize, m_streamBuffer.begin());
                bytesRecvInBuff->popN(m_splittingSize);

                std::cout << "m_streamBuffer is " << m_streamBuffer << std::endl;
            } else {
                std::copy_n(bytesRecvInBuff->first(), loopSize, m_streamBuffer.begin());
                bytesRecvInBuff->popN(loopSize);

                std::copy_n(bytesRecvInBuff->first(), m_splittingSize - loopSize, m_streamBuffer.begin() + loopSize);
                bytesRecvInBuff->popN(m_splittingSize - loopSize);
            }
        } else {
            LOG_WARNG << "bytesRecvInBuff->size(" << bytesRecvInBuff->size() << ") is not reach expected size(" << m_splittingSize << ")";
            return LCT_TCP_MSG_NOT_READY;
        }

        return errCode;
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
    std::size_t     m_splittingSize;
    HandleType      m_onCompletion = nullptr;
    BufferStream&   m_streamBuffer;

    typedef std::shared_ptr<BufferStream>   CStreamShp;
    typedef CLctQueue<CStreamShp>           CStreamQueueShp;
    CStreamQueueShp                         m_streamQueue;
};

template <typename HandleType, typename BufferStream>
class CLctNetAsyncReadEvent<HandleType, BufferStream, char>: public CLctNetBasicAsyncReadEvent
{
public:
    explicit CLctNetAsyncReadEvent(HandleType&& handle, BufferStream& stream, const char delim) :
            m_delim(delim), m_onCompletion(std::move(handle)), m_streamBuffer(stream)
    {
        if (m_streamBuffer.size() < STREAM_DEFAULT_SIZE) {
            m_streamBuffer.resize(STREAM_DEFAULT_SIZE);
        }

        LOG_DEBUG_MARK();
    }

    ~CLctNetAsyncReadEvent()
    {
        LOG_DEBUG_MARK();
    }

    LCT_ERR_CODE extractMessage(CLctSocketBufferShp& bytesRecvInBuff) override
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        LOG_DEBUG << "bytesRecvInBuff->size(" << bytesRecvInBuff->size() << ")";

        std::size_t pos = bytesRecvInBuff->findFirstOf(m_delim);
        if (CLctSocketBuffer::npos == pos){
            LOG_WARNG << "bytesRecvInBuff->size(" << bytesRecvInBuff->size() << ") has not found delim";

            if (bytesRecvInBuff->size() >= m_streamBuffer.size()) {
                LOG_ERROR << "bytesRecvInBuff->size(" << bytesRecvInBuff->size() << ") is more than m_streamBuffer.size(" << m_streamBuffer.size() << ")";
                return LCT_EXTRACT_TCP_MSG_FAIL;
            }

            return LCT_TCP_MSG_NOT_READY;
        }

        if (!m_isAssigned) {
            errCode = extractMessage(m_streamBuffer, bytesRecvInBuff, pos);
            if (LCT_SUCCESS != errCode) {
                LOG_ERROR << "Failed to extract message" << ErrCodeFormat(errCode);
                return errCode;
            } else {
                m_isAssigned = true;
            }
        }

        extractMessage2Queue(bytesRecvInBuff);

        return errCode;
    }

    LCT_ERR_CODE onCompletion() override
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        m_onCompletion();

        m_isAssigned = false;

        LOG_DEBUG << "m_streamQueue.size(" << m_streamQueue.size() << ")";

        while (!m_streamQueue.empty()) {
            CStreamShp sst;
            errCode = m_streamQueue.dequeue(sst);
            if (LCT_SUCCESS != errCode) {
                LOG_ERROR << "Failed to dequeue message" << ErrCodeFormat(errCode);
                return errCode;
            }
            m_streamBuffer = *sst;
            m_onCompletion();
        }

        return errCode;
    }

private:
    LCT_ERR_CODE extractMessage2Queue(CLctSocketBufferShp& bytesRecvInBuff)
    {
        LCT_ERR_CODE errCode = LCT_SUCCESS;
        std::size_t pos = bytesRecvInBuff->findFirstOf(m_delim);
        while(pos != CLctSocketBuffer::npos) {

            CStreamShp sst = std::make_shared<BufferStream>();
            sst->resize(m_streamBuffer.size());

            errCode = extractMessage(*sst, bytesRecvInBuff, pos);
            if (LCT_SUCCESS != errCode) {
                LOG_ERROR << "Failed to extract message from socket buffer" << ErrCodeFormat(errCode);
                break;
            }
            errCode = m_streamQueue.enqueue(sst);
            if (LCT_SUCCESS != errCode) {
                LOG_ERROR << "Failed to enqueue message" << ErrCodeFormat(errCode);
                break;
            }

            pos = bytesRecvInBuff->findFirstOf(m_delim);
        }

        return LCT_SUCCESS;
    }

    LCT_ERR_CODE extractMessage(BufferStream& stream, CLctSocketBufferShp& bytesRecvInBuff, const std::size_t pos) const
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        const std::size_t loopSize = bytesRecvInBuff->loopSize();
        const std::size_t size = pos + 1;
        if (loopSize >= size) {
            std::copy_n(bytesRecvInBuff->first(), pos, stream.begin());
            bytesRecvInBuff->popN(size);
        } else {
            std::copy_n(bytesRecvInBuff->first(), loopSize, stream.begin());
            bytesRecvInBuff->popN(loopSize);

            std::copy_n(bytesRecvInBuff->first(), size - loopSize - 1, stream.begin() + loopSize);
            bytesRecvInBuff->popN(size - loopSize);
        }

        return errCode;
    }

private:
    static constexpr std::size_t STREAM_DEFAULT_SIZE = 1024;

    bool            m_isAssigned   = false;
    char            m_delim;
    HandleType      m_onCompletion = nullptr;
    BufferStream&   m_streamBuffer;

    typedef std::shared_ptr<BufferStream>   CStreamShp;
    typedef CLctQueue<CStreamShp>           CStreamQueueShp;
    CStreamQueueShp                         m_streamQueue {0, 0};
};

#endif /* SRC_NETWORK_LCT_NET_ASYNC_READ_EVENT_H_ */
