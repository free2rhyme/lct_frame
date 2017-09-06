/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_thread_pool.hpp
 * @version      1.0
 * @date         May 23, 2017 6:06:57 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_THREAD_POOL_HPP_
#define SRC_COMMON_LCT_THREAD_POOL_HPP_

#include "lct_thread_pool_impl.h"

template<typename Callable, typename... Args>
LCT_ERR_CODE CLctThreadpool::emplaceTask(Callable&& func, Args&&... vargs)
{
    if(!m_isRunning || (m_threadpoolImplPtr == nullptr)){
        return LCT_FAIL;
    }
    return m_threadpoolImplPtr->emplaceTask(func, vargs...);
}

#endif /* SRC_COMMON_LCT_THREAD_POOL_HPP_ */
