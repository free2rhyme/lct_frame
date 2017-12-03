/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_service_mgr.cpp
 * @version     1.0
 * @date        Jun 30, 2017 7:22:58 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "id_service_mgr.h"

#include "id_config_mgr.h"
#include "id_data_source.h"
#include "id_manager.h"

LCT_ERR_CODE CSidServiceMgr::init()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    /**************** Data Source Manager ****************/
    errCode = ID_DATA_SRC->init(
            SID_CONFIG_MGR->databasePort(),
            SID_CONFIG_MGR->databaseIp(),
            SID_CONFIG_MGR->keyspace(),
            SID_CONFIG_MGR->databaseTableName());
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    } else {
        LOG_INFOR << "Successful to init Data Source Manager";
    }

    /**************** ID Manager Manager ****************/
    errCode = SID_ID_MGR->init();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    } else {
        LOG_INFOR << "Successful to init IVA Id Manager";
    }


    /**************** Network Service ****************/
    m_serviceShp.reset(new CSidService(SID_CONFIG_MGR->servicePort(), SID_CONFIG_MGR->serviceIp()));
    errCode = m_serviceShp->init();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    LOG_INFOR << "Successful to init IVA Unique Id Service";
    return errCode;
}

LCT_ERR_CODE CSidServiceMgr::run()
{
    /**************** Network Service ****************/
    return  m_serviceShp->serve();
}

LCT_ERR_CODE CSidServiceMgr::shutdown()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    /**************** Network Service ****************/
    if(m_serviceShp != nullptr){
        errCode = m_serviceShp->shutdown();

        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
        }
    }

    /**************** Data Source Manager ****************/
    errCode = ID_DATA_SRC->close();
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << ErrCodeFormat(errCode);
        return errCode;
    }

    return  errCode;
}

CSidServiceMgr::CSidServiceMgr()
{
}

CSidServiceMgr::~CSidServiceMgr()
{
}


