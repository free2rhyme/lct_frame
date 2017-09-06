/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_socket_manager.h
 * @version     1.0
 * @date        Aug 15, 2017 8:34:11 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_SOCKET_MANAGER_H_
#define SRC_NETWORK_LCT_SOCKET_MANAGER_H_

#include "detail/lct_network_common_define.h"
#include "lct_singleton.h"
#include <unordered_map>
#include <thread>

class CLctSocketManager: public CLctSingleton<CLctSocketManager>
{
public:
    LCT_ERR_CODE init();
    LCT_ERR_CODE run();
    LCT_ERR_CODE stop();

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctSocketManager);

public:
    LCT_ERR_CODE addSocket(const CLctSocketShp& socketShp);
    LCT_ERR_CODE addSocket(const CLctSocketShp& socketShp, const uint32_t eventType);
    LCT_ERR_CODE findSocket(const int32_t socketFd, CLctSocketShp& socketShp) const;
    LCT_ERR_CODE eraseSocket(const int32_t socketFd);

protected:
    LCT_ERR_CODE select();

    LCT_ERR_CODE processSocketEvent(const int32_t socketFd, const uint32_t epollEvents);

private:
    LCT_ERR_CODE saveSocket(const CLctSocketShp& socketShp);

private:
    CLctSocketManager();
    ~CLctSocketManager();

    friend class CLctSingleton;

private:
    bool    m_running   = false;
    int32_t m_epollFd   = LCT_INVALID_SOCKET_FD;

    typedef std::unordered_map<int32_t, CLctSocketShp> CLctSocketShpMap;
    typedef std::mutex                                 CLctSocketMapMutex;
    typedef std::thread                                CAsyncThreadType;
    typedef std::shared_ptr<CAsyncThreadType>          CAsyncThreadShp;

    CAsyncThreadShp                 m_ioSocketThread;
    CLctSocketShpMap                m_socketShpMap;
    mutable CLctSocketMapMutex      m_socketMapMutex;

};

#define LCT_SOCKET_MGR CLctSocketManager::instance()

#endif /* SRC_NETWORK_LCT_SOCKET_MANAGER_H_ */
