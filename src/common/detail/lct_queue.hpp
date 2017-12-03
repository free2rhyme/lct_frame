/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_queue.hpp
 * @version     1.0
 * @date        May 23, 2017 6:01:35 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_QUEUE_HPP_
#define SRC_COMMON_LCT_QUEUE_HPP_

template <typename T>
CLctQueue<T>::CLctQueue():
   m_maxSize(SIZE_INFINITE),
   m_enqueueWaitTime(TIME_INFINITE),
   m_dequeueWaitTime(TIME_INFINITE)
{
}

template <typename T>
CLctQueue<T>::CLctQueue(const int32_t maxSize):
   m_maxSize(std::max(maxSize, 1)),
   m_enqueueWaitTime(TIME_INFINITE),
   m_dequeueWaitTime(TIME_INFINITE)
{
}

template <typename T>
CLctQueue<T>::CLctQueue(const int64_t enWaitTime, const int64_t deWaitTime):
   m_maxSize(SIZE_INFINITE),
   m_enqueueWaitTime(std::max(enWaitTime, 0L)),
   m_dequeueWaitTime(std::max(deWaitTime, 0L))
{
}

template <typename T>
CLctQueue<T>::CLctQueue(const int32_t maxSize, const int64_t enWaitTime, const int64_t deWaitTime):
   m_maxSize(std::max(maxSize, 1)),
   m_enqueueWaitTime(std::max(enWaitTime, 0L)),
   m_dequeueWaitTime(std::max(deWaitTime, 0L))
{
}

template <typename T>
CLctQueue<T>::~CLctQueue()
{
}

template <typename T>
LCT_ERR_CODE CLctQueue<T>::enqueue(const CObjectType& obj)
{
   std::unique_lock<CLctQueueMutex> guard(m_mutex);
   while (size() >= m_maxSize) {
      if (m_quit) {
         return LCT_SERVICE_DOWN;
      } else if (0 == m_enqueueWaitTime) {
         return LCT_OVERFLOW;
      } else if(TIME_INFINITE == m_enqueueWaitTime){
         m_cond.wait(guard);
      } else {
         if (m_cond.wait_for(guard, std::chrono::milliseconds(m_enqueueWaitTime)) == std::cv_status::timeout) {
            return LCT_TIMEOUT;
         }
      }
   }
   m_dataQueue.push(obj);
   m_cond.notify_all();
   return LCT_SUCCESS;
}

template <typename T>
LCT_ERR_CODE CLctQueue<T>::enqueue(CObjectType&& obj)
{
   std::unique_lock<CLctQueueMutex> guard(m_mutex);

   while (size() >= m_maxSize) {
      if (m_quit) {
         return LCT_SERVICE_DOWN;
      } else if (0 == m_enqueueWaitTime) {
         return LCT_OVERFLOW;
      } else if (TIME_INFINITE == m_enqueueWaitTime) {
         m_cond.wait(guard);
      } else {
         if (m_cond.wait_for(guard, std::chrono::milliseconds(m_enqueueWaitTime)) == std::cv_status::timeout) {
            return LCT_TIMEOUT;
         }
      }
   }
   m_dataQueue.push(std::move(obj));
   m_cond.notify_all();
   return LCT_SUCCESS;
}

template <typename T>
LCT_ERR_CODE CLctQueue<T>::dequeue(CObjectType& obj)
{
   std::unique_lock<CLctQueueMutex> guard(m_mutex);
   while (empty()) {
      if (m_quit) {
         return LCT_SERVICE_DOWN;
      } else if (0 == m_dequeueWaitTime) {
         return LCT_EMPTY_RECORD;
      } else if (TIME_INFINITE == m_dequeueWaitTime) {
         m_cond.wait(guard);
      } else {
         if (m_cond.wait_for(guard, std::chrono::milliseconds(m_dequeueWaitTime)) == std::cv_status::timeout) {
            return LCT_TIMEOUT;
         }
      }
   }
   obj = m_dataQueue.front();
   m_dataQueue.pop();
   return LCT_SUCCESS;
}

template <typename T>
inline bool CLctQueue<T>::empty() const
{
   return m_dataQueue.empty();
}

template <typename T>
inline int32_t CLctQueue<T>::size() const
{
   return m_dataQueue.size();
}

template <typename T>
inline void CLctQueue<T>::notifyAll()
{
   std::unique_lock<CLctQueueMutex> guard(m_mutex);
   m_quit = true;
   m_cond.notify_all();
}


#endif /* SRC_COMMON_LCT_QUEUE_HPP_ */
