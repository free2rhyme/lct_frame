/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_rw_lock.cpp
 * @version     1.0
 * @date        May 27, 2017 6:21:45 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_rw_lock.h"

CLctRwLock::CLctRwLock()
{
   pthread_rwlockattr_t attr;
   pthread_rwlockattr_init(&attr);
   pthread_rwlockattr_setkind_np (&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
   pthread_rwlock_init(&m_lock, &attr);
   pthread_rwlockattr_destroy(&attr);
}

CLctRwLock::~CLctRwLock()
{
   pthread_rwlock_destroy(&m_lock);
}

void CLctRwLock::wrlock()
{
   pthread_rwlock_wrlock(&m_lock);
}

void CLctRwLock::rdlock()
{
   pthread_rwlock_rdlock(&m_lock);
}

void CLctRwLock::unlock()
{
   pthread_rwlock_unlock(&m_lock);
}

CLctRwlockGuard::CLctRwlockGuard(CLctRwLock& lk, bool write /*= false */): m_lock(lk)
{
   if (write) {
      m_lock.wrlock();
   } else {
      m_lock.rdlock();
   }
}

CLctRwlockGuard::~CLctRwlockGuard(){
   m_lock.unlock();
}
