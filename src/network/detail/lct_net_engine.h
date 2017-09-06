/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_net_engine.h
 * @version     1.0
 * @date        Aug 19, 2017 10:45:03 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_ENGINE_H_
#define SRC_NETWORK_LCT_NET_ENGINE_H_

#include <unordered_map>
#include "lct_network_common_define.h"
#include "lct_thread_pool.h"
#include "lct_singleton.h"
#include "lct_log.h"

class CLctNetEngine final: public CLctSingleton<CLctNetEngine>
{
public:
    LCT_ERR_CODE startService();
    LCT_ERR_CODE stopService();

public:
    template <typename CAsyncEventType>
    LCT_ERR_CODE postCompletionEvent(const std::shared_ptr<CAsyncEventType>& eventShp)
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        if(nullptr == m_workerGroup){
            LOG_ERROR << "Net engine has not been initialized yet, service is not ready";
            return LCT_FAIL;
        }

        if(nullptr == eventShp){
            LOG_ERROR << "Invalid eventShp";
            return LCT_INVALID_PARAM;
        }

        errCode = m_workerGroup->emplaceTask(&CAsyncEventType::onCompletion, eventShp);
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << "Failed to post a completion-event to worker thread pool";
            return errCode;
        }

        return errCode;
    }

    template <typename CAsyncEventType>
    LCT_ERR_CODE postWriteEvent(const std::shared_ptr<CAsyncEventType>& eventShp, CLctSocketShp& socketShp)
    {
        LOG_DEBUG_MARK();

        LCT_ERR_CODE errCode = LCT_SUCCESS;

        if(nullptr == m_workerGroup){
            LOG_ERROR << "Net engine has not been initialized yet, service is not ready";
            return LCT_FAIL;
        }

        if(nullptr == eventShp){
            LOG_ERROR << "Invalid eventShp";
            return LCT_INVALID_PARAM;
        }

        errCode = m_workerGroup->emplaceTask(&CAsyncEventType::sendMessage, eventShp, socketShp);
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << "Failed to post a completion-event to worker thread pool";
            return errCode;
        }

        return errCode;
    }

private:
    CLctNetEngine();
    ~CLctNetEngine();

    friend class CLctSingleton;

private:
    bool                    m_isServing  = false;
    CLctThreadpoolShp       m_workerGroup;
};

#define LCT_NET_ENGINE CLctNetEngine::instance()

#endif /* SRC_NETWORK_LCT_NET_ENGINE_H_ */
