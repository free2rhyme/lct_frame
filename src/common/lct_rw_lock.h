/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_rw_lock.h
 * @version      1.0
 * @date         May 27, 2017 6:19:32 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_RW_LOCK_H_
#define SRC_COMMON_LCT_RW_LOCK_H_

#include <pthread.h>
#include "lct_common_define.h"

class lct_rw_lock final
{
private:
    pthread_rwlock_t m_lock;

public:
    lct_rw_lock();

    ~lct_rw_lock();

    void wrlock();
    void rdlock();
    void unlock();

    DISALLOW_COPY_MOVE_OR_ASSIGN(lct_rw_lock);
};

class CLctRwlockGuard final
{
private:
    lct_rw_lock&    m_lock;

public:
    explicit CLctRwlockGuard(lct_rw_lock& lk, bool write = false);
    ~CLctRwlockGuard();

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctRwlockGuard);
};

#endif /* SRC_COMMON_LCT_RW_LOCK_H_ */
