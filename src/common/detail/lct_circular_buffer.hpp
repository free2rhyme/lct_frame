/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_circular_buffer.hpp
 * @version     1.0
 * @date        Aug 22, 2017 4:35:51 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_CIRCULAR_BUFFER_HPP_
#define SRC_COMMON_LCT_CIRCULAR_BUFFER_HPP_

#include <algorithm>

template <typename T>
CLctCircularBuffer<T>::CLctCircularBuffer(const std::size_t sz /* = DEFAULT_SIZE */) :
      m_size(std::max(sz, static_cast<std::size_t>(1)))
{
   m_begin = (CPointerType)malloc(sizeof(CObjectType) * m_size);
   m_end   = m_begin + m_size;

   std::fill_n((char*)m_begin, sizeof(CObjectType) * m_size, 0);

   m_vfirst = m_begin;
   m_vlast  = m_begin;
}

template <typename T>
CLctCircularBuffer<T>::~CLctCircularBuffer()
{
   for (CPointerType itRe = m_vfirst; itRe != m_vlast; increment(itRe)) {
      itRe->~T();
   }

   free(m_begin);

   m_begin  = nullptr;
   m_end   = nullptr;
   m_vfirst = nullptr;
   m_vlast  = nullptr;

   m_count = 0;
   m_size  = 0;
}

template <typename T>
template <typename ... Args>
LCT_ERR_CODE CLctCircularBuffer<T>::emplace(Args&&... vargs)
{
   CPointerType curPtr = new (m_vlast) T(vargs...);
   return append(curPtr);
}

template <typename T>
LCT_ERR_CODE CLctCircularBuffer<T>::emplace(CObjectType&& obj)
{
   CPointerType curPtr = new (m_vlast) T(std::move(obj));
   return append(curPtr);
}

template <typename T>
LCT_ERR_CODE CLctCircularBuffer<T>::push(const CObjectType& obj)
{
   CPointerType curPtr = new (m_vlast) T(obj);
   return append(curPtr);
}

template <typename T>
LCT_ERR_CODE CLctCircularBuffer<T>::pushN(const CPointerType pBegin, std::size_t n)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;
   for (std::size_t i = 0; i < n; ++i) {
      if ((pBegin + i) == nullptr) {
         return LCT_FAIL;
      }
      CPointerType curPtr = new (m_vlast) T(*(pBegin + i));
      errCode = append(curPtr);
      if (LCT_SUCCESS != errCode) {
         return errCode;
      }
   }
   return errCode;
}

template <typename T>
LCT_ERR_CODE CLctCircularBuffer<T>::peek(CObjectType& obj)
{
   if (m_count == 0) {
      return LCT_FAIL;
   }

   obj = *m_vfirst;
   return LCT_SUCCESS;
}

template <typename T>
LCT_ERR_CODE CLctCircularBuffer<T>::pop(CObjectType& obj)
{
   if (m_count == 0) {
      return LCT_FAIL;
   }
   obj = *m_vfirst;
   CPointerType curPtr = m_vfirst;
   increment(m_vfirst);
   curPtr->~T();
   --m_count;

   return LCT_SUCCESS;
}

template <typename T>
LCT_ERR_CODE CLctCircularBuffer<T>::popN(CPointerType pBegin, std::size_t n)
{
   if (m_count < n) {
      return LCT_FAIL;
   }

   LCT_ERR_CODE errCode = LCT_SUCCESS;
   while ((m_count > 0) && (n-- > 0)) {
      errCode = pop(pBegin++);
      if (LCT_SUCCESS != errCode) {
         return errCode;
      }
   }

   return errCode;
}

template <typename T>
template <typename Func>
LCT_ERR_CODE CLctCircularBuffer<T>::iterate(Func&& fn) const
{
   if (m_count == 0) {
      return LCT_SUCCESS;
   }

   CPointerType itRe = m_vfirst;
   do {
      fn(*itRe);
      increment(itRe);
   } while (itRe != m_vlast);

   return LCT_SUCCESS;
}

template <typename T>
LCT_ERR_CODE CLctCircularBuffer<T>::append(CPointerType& ptr)
{
   if (full()) {
      return LCT_FAIL;
   }
   m_vlast = ptr;
   increment(m_vlast);
   if (m_count < m_size) {
      ++m_count;
   } else {
      increment(m_vfirst);
   }
   return LCT_SUCCESS;
}

template <typename T>
void CLctCircularBuffer<T>::increment(CPointerType& ptr) const
{
   if (++ptr == m_end) {
      ptr = m_begin;
   }
}

template <typename T>
void CLctCircularBuffer<T>::decrement(CPointerType& ptr) const
{
   if (ptr == m_begin) {
      ptr = m_end;
   }
   --ptr;
}

template <typename T>
bool CLctCircularBuffer<T>::empty() const
{
   return m_count == 0;
}

template <typename T>
bool CLctCircularBuffer<T>::full() const
{
   return (!m_overlap) && (m_count == m_size);
}

template <typename T>
std::size_t CLctCircularBuffer<T>::size() const
{
   return m_count;
}

template <typename T>
std::size_t CLctCircularBuffer<T>::capacity() const
{
   return m_size;
}
template <typename T>
void CLctCircularBuffer<T>::disableOverlap()
{
   m_overlap = false;
}

template <typename T>
void CLctCircularBuffer<T>::enableOverlap()
{
   m_overlap = true;
}

template <typename T>
LCT_ERR_CODE CLctCircularBuffer<T>::reallocate(const std::size_t sz)
{
   if (size <= m_size) {
      return LCT_SUCCESS;
   }

   const std::size_t vfirstOffset = m_vfirst - m_begin;
   const std::size_t vlastOffset  = m_vlast  - m_begin;

   CPointerType pNewBlock = (CPointerType)malloc(sizeof(CObjectType) * sz);

   std::copy_n((char*)m_begin, sizeof(CObjectType) * m_size, pNewBlock);

   std::fill_n((char*)(pNewBlock + m_size), sizeof(CObjectType) * (size - m_size), 0);

   m_begin = pNewBlock;
   m_end   = m_begin + sz;

   m_vfirst = m_begin + vfirstOffset;
   m_vlast  = m_begin + vlastOffset;

   m_size = size;

   return LCT_SUCCESS;
}

template <typename Func>
LCT_ERR_CODE CLctCircularBuffer<char>::iterate(Func&& fn) const
{
   if (m_count == 0) {
      return LCT_SUCCESS;
   }

   char* itRe = m_vfirst;
   do {
      fn(*itRe);
      increment(itRe);
   } while (itRe != m_vlast);

   return LCT_SUCCESS;
}

#endif /* SRC_COMMON_LCT_CIRCULAR_BUFFER_HPP_ */
