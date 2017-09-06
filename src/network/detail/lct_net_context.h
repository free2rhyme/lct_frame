/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_context.h
 * @version     1.0
 * @date        Aug 25, 2017 10:30:58 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_CONTEXT_H_
#define SRC_NETWORK_LCT_NET_CONTEXT_H_

#include "lct_network_common_define.h"
#include "lct_singleton.h"
#include <mutex>
#include <atomic>

class CLctNetContext final: public CLctSingleton<CLctNetContext>
{
public:
    LCT_ERR_CODE init();
    LCT_ERR_CODE stop();

    bool isInitiated() const;

private:
    CLctNetContext();
    ~CLctNetContext();

    friend class CLctSingleton;
private:
    typedef std::mutex          CNetContextMutex;
    mutable CNetContextMutex    m_mutex;
    std::atomic_bool            m_isInitiated{false};
};

#define LCT_NET_CONTEXT CLctNetContext::instance()

#endif /* SRC_NETWORK_LCT_NET_CONTEXT_H_ */
