/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_grpc_service_helper.hpp
 * @version      1.0
 * @date         May 17, 2017 6:42:45 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_WRAPPER_GRPC_LCTGRPCSERVICEHELPER_HPP_
#define SRC_WRAPPER_GRPC_LCTGRPCSERVICEHELPER_HPP_

#include <memory>
#include <string>
#include <grpc++/server.h>
#include "lct_common_define.h"
#include "lct_log.h"

typedef grpc::Server            GRpcServer;
typedef grpc::ServerBuilder     GRpcServerBuilder;
typedef grpc::ServerContext     GRpcServerContext;
typedef grpc::Status            GRpcStatus;

typedef std::unique_ptr<GRpcServer>  GRpcServerUnp;

template <typename CServiceImpl>
class CLctGRpcService
{
public:
    explicit CLctGRpcService(const int32_t svcPort, const std::string& svcIp):
        m_isServing(false), m_servicePort(svcPort), m_serviceIp(svcIp)
    {
    }

    virtual ~CLctGRpcService()
    {
        shutdown();
    }

    // Number of server completion queues to create to listen to incoming RPCs.
    LCT_ERR_CODE setCqNumber(const int32_t count)
    {
        if(count <= 0){
            return LCT_INVALID_PARAM;
        }

        m_cqNumber = count;
        return LCT_SUCCESS;
    }

    // Minimum number of threads per completion queue that should be listening
    // to incoming RPCs.
    LCT_ERR_CODE setCqMinPoller(const int32_t count)
    {
        if(count <= 0){
            return LCT_INVALID_PARAM;
        }

        m_cqMinPollers = count;
        return LCT_SUCCESS;
    }

    // Maximum number of threads per completion queue that should be listening
    // to incoming RPCs.
    LCT_ERR_CODE setCqMaxPoller(const int32_t count)
    {
        if(count <= 0){
            return LCT_INVALID_PARAM;
        }

        m_cqMaxPollers = count;
        return LCT_SUCCESS;
    }

    // The timeout for server completion queue's AsyncNext call.
    LCT_ERR_CODE setTimeout(const int32_t msec)
    {
        if(msec <= 0){
            return LCT_INVALID_PARAM;
        }

        m_timeoutMsec = msec;
        return LCT_SUCCESS;
    }

    LCT_ERR_CODE init()
    {
        const std::string svcAddr = m_serviceIp + ":" + std::to_string(m_servicePort);

        try{
            m_serverBuilder.AddListeningPort(svcAddr, grpc::InsecureServerCredentials());

            // Number of server completion queues to create to listen to incoming RPCs.
            m_serverBuilder.SetSyncServerOption(GRpcServerBuilder::SyncServerOption::NUM_CQS, m_cqNumber);

            // Minimum number of threads per completion queue that should be listening
            // to incoming RPCs.
            m_serverBuilder.SetSyncServerOption(GRpcServerBuilder::SyncServerOption::MIN_POLLERS, m_cqMinPollers);

            // Maximum number of threads per completion queue that should be listening
            // to incoming RPCs.
            m_serverBuilder.SetSyncServerOption(GRpcServerBuilder::SyncServerOption::MAX_POLLERS, m_cqMaxPollers);

            // The timeout for server completion queue's AsyncNext call.
            m_serverBuilder.SetSyncServerOption(GRpcServerBuilder::SyncServerOption::CQ_TIMEOUT_MSEC, m_timeoutMsec);
            
            m_svcImplShp.reset(new CServiceImpl());
            m_serverBuilder.RegisterService(m_svcImplShp.get());
            
        } catch (const std::exception& e) {
            LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
            return LCT_UNEXPECTED;
        } catch (...) {
            LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
            return LCT_UNEXPECTED;
        }

        return LCT_SUCCESS;
    }

    LCT_ERR_CODE serve(bool syncWait = true)
    {
        try{
            m_serverUnp = m_serverBuilder.BuildAndStart();
            if(!m_serverUnp){
                LOG_ERROR << ErrCodeFormat(LCT_FAIL);
                return LCT_FAIL;
            }
            m_isServing = true;
            if(syncWait){
                m_serverUnp->Wait();
            }
        } catch (const std::exception& e) {
            LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
            return LCT_UNEXPECTED;
        } catch (...) {
            LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
            return LCT_UNEXPECTED;
        }
        return LCT_SUCCESS;
    }

    LCT_ERR_CODE shutdown()
    {
        if(m_isServing){
            try{
                if(m_serverUnp != nullptr){
                    m_serverUnp->shutdown();
                }
            } catch (const std::exception& e) {
                LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
                return LCT_UNEXPECTED;
            } catch (...) {
                LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
                return LCT_UNEXPECTED;
            }
            m_isServing = false;
        }
        return LCT_SUCCESS;
    }

    int32_t getPort() const
    {
        return m_servicePort;
    }

    std::string getIp() const
    {
        return m_serviceIp;
    }

    bool isServing() const
    {
        return m_isServing;
    }

private:
    bool                    m_isServing;
    int32_t                 m_servicePort;
    int32_t                 m_cqNumber            = 1;
    int32_t                 m_cqMinPollers        = 1;
    int32_t                 m_cqMaxPollers        = std::numeric_limits<int32_t>::max();
    int32_t                 m_timeoutMsec         = 1000; //milliseconds
    std::string             m_serviceIp;

private:
    typedef typename std::unique_ptr<CServiceImpl>    CLctSvcImplShp;

    GRpcServerUnp           m_serverUnp;
    CLctSvcImplShp          m_svcImplShp;
    GRpcServerBuilder       m_serverBuilder;

private:
    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctGRpcService);
};

class CLctGRpcServiceHelper
{
public:
    template <typename CServiceImpl>
    static LCT_ERR_CODE serve(const int32_t svcPort, const std::string& svcIp, bool syncWait = true){
        const std::string svcAddr = svcIp + ":" + std::to_string(svcPort);
        try{
            GRpcServerBuilder serverBuilder;
            serverBuilder.AddListeningPort(svcAddr, grpc::InsecureServerCredentials());

            std::unique_ptr<CServiceImpl> svcUnq(new CServiceImpl());
            serverBuilder.RegisterService(svcUnq.get());

            GRpcServerUnp serverUnp = serverBuilder.BuildAndStart();
            if(syncWait){
                serverUnp->Wait();
            }
        } catch (const std::exception& e) {
            LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
            return LCT_UNEXPECTED;
        } catch (...) {
            LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
            return LCT_UNEXPECTED;
        }
        return LCT_SUCCESS;
    }

private:
    DISALLOW_INSTANTIATE(CLctGRpcServiceHelper);
    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctGRpcServiceHelper);
};


#endif /* SRC_WRAPPER_GRPC_LCTGRPCSERVICEHELPER_HPP_ */
