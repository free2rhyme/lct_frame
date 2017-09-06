/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_work.h
 * @version     1.0
 * @date        Aug 25, 2017 11:37:02 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_WORK_H_
#define SRC_NETWORK_LCT_NET_WORK_H_

#include "event/lct_net_session_handler.h"

class CLctNetWork
{
public:
    explicit CLctNetWork();
    virtual ~CLctNetWork();

public:
    LCT_ERR_CODE start();
    LCT_ERR_CODE stop();

    bool isWorking() const;

    static bool isContextInited();

    static LCT_ERR_CODE prepareContext();
    static LCT_ERR_CODE cleanupContext();

    std::size_t socketRecvBuffSize() const;
    LCT_ERR_CODE setSocketRecvBuffSize(const std::size_t size);

protected:
    virtual LCT_ERR_CODE startWork()  = 0;
    virtual LCT_ERR_CODE stopWork()   = 0;
    virtual LCT_ERR_CODE checkWork()  = 0;

    virtual LCT_ERR_CODE resetRecvBuffSize();

    LCT_ERR_CODE checkService();
private:
    static std::atomic_bool     m_isContextInited;

    bool                        m_isWorking = false;
    std::size_t                 m_socketRecvBuffSize = DEFAULT_RECV_BUFFER_SIZE;
};



#endif /* SRC_NETWORK_LCT_NET_WORK_H_ */
