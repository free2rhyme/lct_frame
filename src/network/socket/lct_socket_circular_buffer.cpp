/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_socket_circular_buffer.cpp
 * @version     1.0
 * @date        Aug 23, 2017 11:14:01 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_socket_circular_buffer.h"

CLctSocketCircularBuffer::CLctSocketCircularBuffer(const std::size_t size):
        CLctCircularBuffer<char>(size)
{
    disableOverlap();
}

CLctSocketCircularBuffer::CLctSocketCircularBuffer(const std::string& data):
        CLctCircularBuffer<char>(data)
{
    disableOverlap();
}

CLctSocketCircularBuffer::CLctSocketCircularBuffer(const char* data, std::size_t size):
        CLctCircularBuffer<char>(data, size)
{
    disableOverlap();
}

CLctSocketCircularBuffer::~CLctSocketCircularBuffer()
{
}

std::size_t CLctSocketCircularBuffer::loopSize() const
{
    if (m_count <= 0){
        return 0;
    }

    if (m_vlast > m_vfirst) {
        return (m_vlast - m_vfirst);
    } else if (m_vlast < m_vfirst) {
        return (m_end - m_vfirst);
    } else {
        return (m_end - m_vfirst);
    }
}

std::size_t CLctSocketCircularBuffer::findFirstOf(char c, std::size_t pos/* = 0 */) const
{
    char* ptr = m_vfirst + pos;
    while (ptr != m_vlast) {
        if (*ptr == c){
            return pos;
        } else {
            increment(ptr);
            ++pos;
        }
    }
    return npos;
}


char*& CLctSocketCircularBuffer::first()
{
    return m_vfirst;
}

char*& CLctSocketCircularBuffer::last()
{
    return m_vlast;
}

LCT_ERR_CODE CLctSocketCircularBuffer::expand(const std::size_t n)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if (n > space()) {
        return LCT_FAIL;
    }
    m_vlast += n;
    m_count += n;

    return errCode;
}

