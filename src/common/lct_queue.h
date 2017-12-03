/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_queue.h
 * @version     1.0
 * @date        May 23, 2017 6:02:31 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_QUEUE_H_
#define SRC_COMMON_LCT_QUEUE_H_

#include <queue>
#include <limits>
#include <mutex>
#include <condition_variable>

#include "lct_common_define.h"

template <typename T>
class CLctQueue final
{
private:
   typedef std::mutex            CLctQueueMutex;
   typedef std::condition_variable   CLctCond;

private:
   typedef T      CObjectType;
   typedef T*      CPointerType;

public:
   /*********************************************
    * enWaitTime(milliseconds), enqueue:
    *      By default, it is infinite
    *      0, return immediately
    *      0 < #waitTime < m_infinite, means waiting Time
    *
    * deWaitTime(milliseconds), dequeue:
    *      By default, it is infinite
    *      0, return immediately
    *      0 < #waitTime < m_infinite, means waiting Time
    *
    * maxSize:
    *      By default, its size is infinite
    *      0, return immediately
    *      0 < #maxSize < m_infinite, means the max size
    *
    *********************************************/
   explicit CLctQueue();
   explicit CLctQueue(const int32_t maxSize);
   explicit CLctQueue(const int64_t enWaitTime, const int64_t deWaitTime);
   explicit CLctQueue(const int32_t maxSize, const int64_t enWaitTime, const int64_t deWaitTime);

   ~CLctQueue();

   DISALLOW_COPY_MOVE_OR_ASSIGN(CLctQueue);

public:
   LCT_ERR_CODE enqueue(const CObjectType&);
   LCT_ERR_CODE enqueue(CObjectType&&);
   LCT_ERR_CODE dequeue(CObjectType&);

   bool   empty() const;
   int32_t size()  const;
   void   notifyAll();

private:
   bool                       m_quit = false;
   const int32_t              m_maxSize;
   const int64_t              m_enqueueWaitTime; // milliseconds
   const int64_t              m_dequeueWaitTime; // milliseconds
   CLctCond                   m_cond;
   mutable CLctQueueMutex     m_mutex;

   typedef typename std::queue<T> CLctQueueDataType;
   CLctQueueDataType          m_dataQueue;

public:
   constexpr static int32_t SIZE_INFINITE = std::numeric_limits<int32_t>::max();
   constexpr static int64_t TIME_INFINITE = std::numeric_limits<int64_t>::max();
};

#include "detail/lct_queue.hpp"


#endif /* SRC_COMMON_LCT_QUEUE_H_ */
