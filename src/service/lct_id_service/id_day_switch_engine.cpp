/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_day_switch_engine.cpp
 * @version     1.0
 * @date        Jun 30, 2017 7:27:58 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "id_day_switch_engine.h"

#include "id_config_mgr.h"
#include "id_data_source.h"

CSidDaySwitchEngine::CSidDaySwitchEngine(const LCT_SEED_ID_TYPE& seedId):
    CSidEngine(ESeedType::SEED_TYPE_DAY_SWITCH, seedId)
{
}

CSidDaySwitchEngine::~CSidDaySwitchEngine()
{
}

LCT_ERR_CODE CSidDaySwitchEngine::init()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    if(LCT_SUCCESS == readPersistance()){
        m_engineInfo.CurrentId.store(m_engineInfo.PreallocValve);
        m_engineInfo.DayStamp = LctCurrentDay();
        errCode = reallocValve();
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            return errCode;
        }    
    } else {
        return daySwitch();
    }
    return errCode;
}

LCT_ERR_CODE CSidDaySwitchEngine::daySwitch()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    std::lock_guard<CEngineMutex> lk(m_mutex);

    if(LctCurrentDay() != m_engineInfo.DayStamp){

        m_engineInfo.CurrentId.store(0);
        m_engineInfo.DayStamp      = LctCurrentDay();
        m_engineInfo.PreallocValve = m_engineInfo.PreallocSize;

        errCode = persist();
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            return errCode;
        }
    }

    return errCode;
}

LCT_ERR_CODE CSidDaySwitchEngine::reallocValve()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    std::lock_guard<CEngineMutex> lk(m_mutex);

    if(m_engineInfo.CurrentId.load() >= m_engineInfo.PreallocValve){
        m_engineInfo.PreallocValve += m_engineInfo.PreallocSize;
        errCode = persist();
        if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            return errCode;
        }    
    }

    return errCode;
}

LCT_ERR_CODE CSidDaySwitchEngine::readPersistance()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    
    errCode = ID_DATA_SRC->query(seedId(), seedType(), m_engineInfo);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to query engine info from db";
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CSidDaySwitchEngine::persist()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    
    errCode = ID_DATA_SRC->persist(seedId(), seedType(), m_engineInfo);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to save engine info into db";
        return errCode;
    }

    return errCode;
}

