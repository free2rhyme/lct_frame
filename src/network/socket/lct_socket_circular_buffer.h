/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_socket_circular_buffer.h
 * @version     1.0
 * @date        Aug 23, 2017 11:11:01 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_SOCKET_CIRCULAR_BUFFER_H_
#define SRC_NETWORK_LCT_SOCKET_CIRCULAR_BUFFER_H_

#include "lct_circular_buffer.h"

class CLctSocketCircularBuffer final: public CLctCircularBuffer<char>
{
public:
    explicit CLctSocketCircularBuffer(const std::size_t size = CLctCircularBuffer<char>::DEFAULT_SIZE);
    explicit CLctSocketCircularBuffer(const std::string& data);
    explicit CLctSocketCircularBuffer(const char* data, std::size_t size);
    virtual ~CLctSocketCircularBuffer();

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctSocketCircularBuffer);

public:
    std::size_t loopSize() const;
    std::size_t findFirstOf(char c, std::size_t pos = 0) const;

    char*& first();
    char*& last();

    LCT_ERR_CODE expand(const std::size_t n);
    LCT_ERR_CODE reallocate(const std::size_t);
};

#endif /* SRC_NETWORK_LCT_SOCKET_CIRCULAR_BUFFER_H_ */
