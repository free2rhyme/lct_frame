/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_thread_pool.h
 * @version     1.0
 * @date        May 23, 2017 6:05:25 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_THREAD_POOL_H_
#define SRC_COMMON_LCT_THREAD_POOL_H_

#include <future>
#include <mutex>
#include "lct_common_define.h"

class CLctThreadpoolImpl;

class CLctThreadpool final
{
public:
   LCT_ERR_CODE init(const int32_t threadCount = -1);
   LCT_ERR_CODE start();
   LCT_ERR_CODE shutdown();

   template<typename Callable, typename... Args>
   LCT_ERR_CODE emplaceTask(Callable&& func, Args&&... vargs);

   bool isRunning() const
   {
      return m_isRunning;
   }

   int32_t size() const;

public:
   CLctThreadpool();
   ~CLctThreadpool();

   DISALLOW_COPY_MOVE_OR_ASSIGN(CLctThreadpool);

private:
   bool                  m_isRunning           = false;
   int32_t               m_threadCount         = 0;
   CLctThreadpoolImpl*   m_threadpoolImplPtr   = nullptr;
};

typedef std::shared_ptr<CLctThreadpool> CLctThreadpoolShp;

#include "detail/lct_thread_pool.hpp"

#endif /* SRC_COMMON_LCT_THREAD_POOL_H_ */
