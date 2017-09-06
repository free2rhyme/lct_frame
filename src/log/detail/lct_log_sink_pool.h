/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_log_sink_pool.h
 * @version      1.0
 * @date         Jun 9, 2017 9:29:06 AM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_LOG_LCT_LOG_SINK_POOL_H_
#define SRC_LOG_LCT_LOG_SINK_POOL_H_

#include <thread>
#include <memory>
#include "lct_common_define.h"
#include "lct_queue.h"
#include "lct_cplusplus_14.h"

template <typename... CSinkType>
class CLctLogSinkPool final
{
public:
    explicit CLctLogSinkPool(bool isAsyncSink = true):
        m_isAsyncSink(isAsyncSink)
    {
    }

    ~CLctLogSinkPool()
    {
        stop();
    }

    LCT_ERR_CODE start()
    {
        m_isRunning = true;

        if(m_isAsyncSink){
            m_worker.reset(new std::thread(&CLctLogSinkPool::Run, this));
        }

        return LCT_SUCCESS;
    }

    LCT_ERR_CODE stop()
    {
        m_isRunning = false;

        m_logQueue.notifyAll();

        if(m_worker != nullptr){
            m_worker->join();
        }
        return LCT_SUCCESS;
    }

    LCT_ERR_CODE log(const CLctLogMessageShp& message)
    {
        if(m_isAsyncSink){
            return asyncSink(message);
        } else {
            return sink(message);
        }
    }

    LCT_ERR_CODE Run()
    {
        while(m_isRunning){
            CLctLogMessageShp logMsgShp;
            if(LCT_SUCCESS == m_logQueue.dequeue(logMsgShp)){
                sink(logMsgShp);
            }
        }
        return LCT_SUCCESS;
    }


    template <typename CSink>
    LCT_ERR_CODE appendSink(const CSink& sink)
    {
        auto& sinkRef = cplusplus14::get<CSink>(m_sinks);
        sinkRef = sink;
        return LCT_SUCCESS;
    }

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctLogSinkPool);

private:
    LCT_ERR_CODE asyncSink(const CLctLogMessageShp& message)
    {
        return m_logQueue.enqueue(message);
    }

    LCT_ERR_CODE sink(const CLctLogMessageShp& stream) const
    {
        return Sink(stream, m_sinks);
    }

    template<size_t I = 0, typename... CSinksType>
    static auto Sink(const CLctLogMessageShp& message, const std::tuple<CSinksType...>& sinkTuple)->typename std::enable_if<I == sizeof...(CSinksType), LCT_ERR_CODE>::type
    {
    }

    template<size_t I = 0, typename... CSinksType>
    static auto Sink(const CLctLogMessageShp& message, const std::tuple<CSinksType...>& sinkTuple)->typename std::enable_if<I < sizeof...(CSinksType), LCT_ERR_CODE>::type
    {
        LCT_ERR_CODE errCode = LCT_SUCCESS;

        auto& sink = std::get<I>(sinkTuple);

        if(sink != nullptr){
            errCode = sink->log(message);
        }

        return Sink<I+1>(message, sinkTuple);
    }

private:
    bool                        m_isAsyncSink;
    bool                        m_isRunning      = false;

    typedef typename std::tuple<CSinkType...>    CLctSinkTuple;
    CLctSinkTuple                                m_sinks;

    typedef CLctQueue<CLctLogMessageShp>    CLctLogQueue;
    CLctLogQueue                            m_logQueue;

    typedef std::shared_ptr<std::thread>    CAsyncWorker;
    CAsyncWorker                            m_worker;
};


#endif /* SRC_LOG_LCT_LOG_SINK_POOL_H_ */
