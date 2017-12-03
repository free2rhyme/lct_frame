/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_circular_buffer.h
 * @version     1.0
 * @date        Aug 22, 2017 4:24:25 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_CIRCULAR_BUFFER_H_
#define SRC_COMMON_LCT_CIRCULAR_BUFFER_H_

#include "lct_common_define.h"

template <typename T>
class CLctCircularBuffer
{
private:
   typedef T  CObjectType;
   typedef T* CPointerType;

public:
   explicit CLctCircularBuffer(const std::size_t size = DEFAULT_SIZE);

   virtual ~CLctCircularBuffer();

   DISALLOW_COPY_MOVE_OR_ASSIGN(CLctCircularBuffer);

public:
   template <typename ... Args>
   LCT_ERR_CODE emplace(Args&&... vargs);
   LCT_ERR_CODE emplace(CObjectType&&);
   LCT_ERR_CODE push(const CObjectType&);
   LCT_ERR_CODE pushN(const CPointerType pBegin, std::size_t n);
   LCT_ERR_CODE peek(CObjectType&);
   LCT_ERR_CODE pop(CObjectType&);
   LCT_ERR_CODE popN(CPointerType pBegin, std::size_t n);

   template <typename Func>
   LCT_ERR_CODE iterate(Func&& fn) const;

   bool empty() const;
   bool full()  const;
   std::size_t size() const;
   std::size_t capacity() const;

   void disableOverlap();
   void enableOverlap();

   LCT_ERR_CODE reallocate(const std::size_t);

protected:
   LCT_ERR_CODE append(CPointerType& ptr);
   void increment(CPointerType& ptr) const;
   void decrement(CPointerType& ptr) const;

protected:
   bool       m_overlap  = false;

   CPointerType m_begin  = nullptr;
   CPointerType m_end    = nullptr;

   CPointerType m_vfirst = nullptr;
   CPointerType m_vlast  = nullptr;

   std::size_t m_count = 0;
   std::size_t m_size  = DEFAULT_SIZE;

protected:
   constexpr static std::size_t DEFAULT_SIZE = 1024;
};

template <>
class CLctCircularBuffer<char>
{
public:
   explicit CLctCircularBuffer(const std::size_t size = DEFAULT_SIZE);
   explicit CLctCircularBuffer(const std::string& data);
   explicit CLctCircularBuffer(const char* data, std::size_t size);
   virtual ~CLctCircularBuffer();

   DISALLOW_COPY_MOVE_OR_ASSIGN(CLctCircularBuffer);

public:
   LCT_ERR_CODE emplace(const char);
   LCT_ERR_CODE push(const char);
   LCT_ERR_CODE pushN(const char* pBegin, std::size_t n);
   LCT_ERR_CODE peek(char&);
   LCT_ERR_CODE pop(char&);
   LCT_ERR_CODE pop();
   LCT_ERR_CODE popN(char* pBegin, std::size_t n);
   LCT_ERR_CODE popN(std::size_t n);

   template <typename Func>
   LCT_ERR_CODE iterate(Func&& fn) const;

   bool empty() const;
   bool full()  const;
   std::size_t size() const;
   std::size_t capacity() const;
   std::size_t space() const;

   void disableOverlap();
   void enableOverlap();

   LCT_ERR_CODE reallocate(const std::size_t);

protected:
   LCT_ERR_CODE append(const char ptr);
   void increment(char*& ptr) const;
   void decrement(char*& ptr) const;
   void increment(char*& ptr, std::size_t n) const;
   void decrement(char*& ptr, std::size_t n) const;

protected:
   bool  m_overlap = false;

   char* m_begin   = nullptr;
   char* m_end     = nullptr;

   char* m_vfirst  = nullptr;
   char* m_vlast   = nullptr;

   std::size_t m_count = 0;
   std::size_t m_size  = DEFAULT_SIZE;

protected:
   constexpr static std::size_t DEFAULT_SIZE = 1024;

public:
   constexpr static const std::size_t npos = -1;
};

#include "detail/lct_circular_buffer.hpp"

#endif /* SRC_COMMON_LCT_CIRCULAR_BUFFER_H_ */
