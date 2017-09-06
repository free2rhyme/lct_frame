/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_rw_lock.cpp
 * @version      1.0
 * @date         May 27, 2017 6:21:45 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/

#include "lct_rw_lock.h"

lct_rw_lock::lct_rw_lock()
{
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setkind_np (&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    pthread_rwlock_init(&m_lock, &attr);
    pthread_rwlockattr_destroy(&attr);
}

lct_rw_lock::~lct_rw_lock()
{
    pthread_rwlock_destroy(&m_lock);
}

void lct_rw_lock::wrlock()
{
    pthread_rwlock_wrlock(&m_lock);
}

void lct_rw_lock::rdlock()
{
    pthread_rwlock_rdlock(&m_lock);
}

void lct_rw_lock::unlock()
{
    pthread_rwlock_unlock(&m_lock);
}

CLctRwlockGuard::CLctRwlockGuard(lct_rw_lock& lk, bool write /*= false */): m_lock(lk)
{
    if (write){
        m_lock.wrlock();
    } else {
        m_lock.rdlock();
    }
}

CLctRwlockGuard::~CLctRwlockGuard(){
    m_lock.unlock();
}
