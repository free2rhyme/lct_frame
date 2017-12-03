/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_thread_pool_impl.hpp
 * @version     1.0
 * @date        May 23, 2017 6:04:47 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_THREAD_POOL_IMPL_HPP_
#define SRC_COMMON_LCT_THREAD_POOL_IMPL_HPP_

template<typename Callable, typename... Args>
LCT_ERR_CODE CLctThreadpoolImpl::emplaceTask(Callable&& func, Args&&... vargs)
{
   std::function<typename std::result_of<Callable(Args...)>::type()> task(std::bind(std::forward<Callable>(func), std::forward<Args>(vargs)...));
   return m_tasksQueue.enqueue(std::move(task));
}

#endif /* SRC_COMMON_LCT_THREAD_POOL_IMPL_HPP_ */
