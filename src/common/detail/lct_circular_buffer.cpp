/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_circular_buffer.cpp
 * @version     1.0
 * @date        Aug 23, 2017 3:05:34 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_circular_buffer.h"

CLctCircularBuffer<char>::CLctCircularBuffer(const std::size_t size /* = DEFAULT_SIZE */) :
        m_size(std::max(size, static_cast<std::size_t>(1)))
{
    m_begin = (char*)malloc(sizeof(char) * m_size);
    m_end   = m_begin + m_size;

    std::fill_n(m_begin, sizeof(char) * m_size, '\0');

    m_vfirst = m_begin;
    m_vlast  = m_begin;
}

CLctCircularBuffer<char>::CLctCircularBuffer(const std::string& data) :
        m_size(std::max(data.size(), static_cast<std::size_t>(1)))
{
    m_begin = (char*)malloc(sizeof(char) * m_size);
    m_end   = m_begin + m_size;

    data.copy(m_begin, sizeof(char) * m_size);

    m_vfirst = m_begin;
    m_vlast  = m_begin;
}

CLctCircularBuffer<char>::CLctCircularBuffer(const char* data, std::size_t size) :
        m_size(std::max(size, static_cast<std::size_t>(1)))
{
    m_begin = (char*)malloc(sizeof(char) * m_size);
    m_end   = m_begin + m_size;

    std::copy_n(data, sizeof(char) * m_size, m_begin);

    m_vfirst = m_begin;
    m_vlast  = m_begin;
}

CLctCircularBuffer<char>::~CLctCircularBuffer()
{
    free (m_begin);

    m_begin = nullptr;
    m_end   = nullptr;
    m_vfirst = nullptr;
    m_vlast  = nullptr;

    m_count = 0;
    m_size = 0;
}

LCT_ERR_CODE CLctCircularBuffer<char>::emplace(const char obj)
{
    return append(obj);
}

LCT_ERR_CODE CLctCircularBuffer<char>::push(const char obj)
{
    return append(obj);
}

LCT_ERR_CODE CLctCircularBuffer<char>::pushN(const char* pBegin, std::size_t n)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (n <= space()) {
        std::copy_n(pBegin, n, m_vlast);
        increment(m_vlast, n);
        m_count += n;
    } else {
        for (std::size_t i = 0; i < n; ++i) {
            if ((pBegin + i) == nullptr) {
                return LCT_FAIL;
            }
            errCode = append(*(pBegin + i));
            if (LCT_SUCCESS != errCode) {
                return errCode;
            }
        }
    }
    return errCode;
}

LCT_ERR_CODE CLctCircularBuffer<char>::peek(char& obj)
{
    if (m_count == 0) {
        return LCT_FAIL;
    }
    obj = *m_vfirst;
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctCircularBuffer<char>::pop(char& obj)
{
    if (m_count == 0) {
        return LCT_FAIL;
    }
    obj = *m_vfirst;

    return pop();
}

LCT_ERR_CODE CLctCircularBuffer<char>::pop()
{
    if (m_count == 0){
        return LCT_FAIL;
    }
    increment(m_vfirst);
    --m_count;
    return LCT_SUCCESS;
}

LCT_ERR_CODE CLctCircularBuffer<char>::popN(char* pBegin, std::size_t n)
{
    if (m_count < n) {
        return LCT_FAIL;
    }

    LCT_ERR_CODE errCode = LCT_SUCCESS;
    if (m_vlast > m_vfirst) {
        std::copy_n(m_vfirst, n, pBegin);
        increment(m_vfirst, n);
        m_count -= n;
    } else {
        while ((m_count > 0) && (n-- > 0)) {
            errCode = pop(*pBegin++);
            if (LCT_SUCCESS != errCode) {
                return errCode;
            }
        }
    }

    return errCode;
}

LCT_ERR_CODE CLctCircularBuffer<char>::popN(std::size_t n)
{
    if (m_count < n) {
         return LCT_FAIL;
     }
     LCT_ERR_CODE errCode = LCT_SUCCESS;
     if (m_vlast > m_vfirst) {
         increment(m_vfirst, n);
         m_count -= n;
     } else {
         while ((m_count > 0) && (n-- > 0)) {
             errCode = pop();
             if (LCT_SUCCESS != errCode) {
                 return errCode;
             }
         }
     }

     return errCode;
}

LCT_ERR_CODE CLctCircularBuffer<char>::append(const char ptr)
{
    if (full()) {
        return LCT_FAIL;
    }
    *m_vlast = ptr;
    increment(m_vlast);
    if (m_count < m_size) {
        ++m_count;
    } else {
        increment(m_vfirst);
    }
    return LCT_SUCCESS;
}

void CLctCircularBuffer<char>::increment(char*& ptr) const
{
    if (++ptr == m_end) {
        ptr = m_begin;
    }
}

void CLctCircularBuffer<char>::decrement(char*& ptr) const
{
    if (ptr == m_begin) {
        ptr = m_end;
    }
    --ptr;
}


void CLctCircularBuffer<char>::increment(char*& ptr, std::size_t n) const
{
    ptr += n;
    if (ptr == m_end) {
        ptr = m_begin;
    }
}

void CLctCircularBuffer<char>::decrement(char*& ptr, std::size_t n) const
{
    if (ptr == m_begin) {
        ptr = m_end;
    }
    ptr -= n;
}

bool CLctCircularBuffer<char>::empty() const
{
    return m_count == 0;
}

bool CLctCircularBuffer<char>::full() const
{
    return (!m_overlap) && (m_count == m_size);
}

std::size_t CLctCircularBuffer<char>::size() const
{
    return m_count;
}

std::size_t CLctCircularBuffer<char>::capacity() const
{
    return m_size;
}

std::size_t CLctCircularBuffer<char>::space() const
{
    if (m_overlap) {
        return (m_end - m_vlast);
    }

    if (m_vlast > m_vfirst) {
        return (m_end - m_vlast);
    } else if (m_vlast < m_vfirst) {
        return (m_vfirst - m_vlast);
    } else {
        if (m_count > 0) {
            return 0;
        } else {
            return (m_end - m_vlast);
        }
    }
}

void CLctCircularBuffer<char>::disableOverlap()
{
    m_overlap = false;
}

void CLctCircularBuffer<char>::enableOverlap()
{
    m_overlap = true;
}

LCT_ERR_CODE CLctCircularBuffer<char>::reallocate(const std::size_t size)
{
    if (size <= m_size) {
        return LCT_SUCCESS;
    }

    const std::size_t vfirstOffset = m_vfirst - m_begin;
    const std::size_t vlastOffset = m_vlast - m_begin;

    char* pNewBlock = (char*)malloc(sizeof(char) * size);

    std::copy_n(m_begin, sizeof(char) * m_size, pNewBlock);

    std::fill_n(pNewBlock + m_size, sizeof(char) * (size - m_size), '\0');

    m_begin = pNewBlock;
    m_end = m_begin + size;

    m_vfirst = m_begin + vfirstOffset;
    m_vlast = m_begin + vlastOffset;

    m_size = size;

    return LCT_SUCCESS;
}
