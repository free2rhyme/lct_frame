/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_rw_lock.h
 * @version     1.0
 * @date        May 27, 2017 6:19:32 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_RW_LOCK_H_
#define SRC_COMMON_LCT_RW_LOCK_H_

#include <pthread.h>
#include "lct_common_define.h"

class CLctRwLock final
{
public:
   void wrlock();
   void rdlock();
   void unlock();

public:
   CLctRwLock();
   ~CLctRwLock();
   DISALLOW_COPY_MOVE_OR_ASSIGN(CLctRwLock);

private:
   pthread_rwlock_t m_lock;
};

class CLctRwlockGuard final
{
public:
   explicit CLctRwlockGuard(CLctRwLock& lk, bool write = false);
   ~CLctRwlockGuard();

   DISALLOW_COPY_MOVE_OR_ASSIGN(CLctRwlockGuard);

private:
   CLctRwLock&  m_lock;
};

#endif /* SRC_COMMON_LCT_RW_LOCK_H_ */
