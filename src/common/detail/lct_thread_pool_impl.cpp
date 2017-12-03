/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_thread_pool_impl.cpp
 * @version     1.0
 * @date        May 23, 2017 6:04:13 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_log.h"
#include "lct_thread_pool_impl.h"

CLctThreadpoolImpl::CLctThreadpoolImpl(const int32_t threadCount):
    m_runningFlag(false),
    m_threadCount(std::max(threadCount, 1))
{
}

CLctThreadpoolImpl::~CLctThreadpoolImpl()
{
    shutdown();
}

LCT_ERR_CODE CLctThreadpoolImpl::start()
{
    m_runningFlag = true;
    m_workers.reserve(m_threadCount);
    for(int32_t i = 0; i < m_threadCount; ++i){
        m_workers.emplace_back(std::thread([this]{
            LOG_INFOR << "lct_thread_pool is starting thread(0x" << std::hex << std::this_thread::get_id() << ")" << std::dec;
            while(m_runningFlag){
                CLctThreadTask    task;
                if (LCT_SUCCESS == this->m_tasksQueue.dequeue(task)){
                    task();
                }
            }
        }));
    }

    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctThreadpoolImpl::shutdown()
{
    if(m_runningFlag){
        m_runningFlag = false;
        m_tasksQueue.notifyAll();
        for(std::thread &worker: m_workers){
            worker.join();
        }
    }
    return LCT_SUCCESS;
}


