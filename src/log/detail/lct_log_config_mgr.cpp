/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_log_config_mgr.h
 * @version      1.0
 * @date         Jul 20, 2017 3:55:29 AM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/

#include "lct_properties.h"
#include "lct_log_config_mgr.h"

static constexpr int64_t IVA_LOG_LEVEL_TRACE = 0x20;
static constexpr int64_t IVA_LOG_LEVEL_DEBUG = 0x10;
static constexpr int64_t IVA_LOG_LEVEL_INFOR = 0x08;
static constexpr int64_t IVA_LOG_LEVEL_WARNG = 0x04;
static constexpr int64_t IVA_LOG_LEVEL_ERROR = 0x02;
static constexpr int64_t IVA_LOG_LEVEL_CRITC = 0x01;

LCT_ERR_CODE CLctLogConfigMgr::init(const int64_t logLevelVal)
{
    std::cout << "Configed logLevel value(" << logLevelVal << ")" << std::endl;

    m_traceLevel  = IVA_LOG_LEVEL_TRACE & logLevelVal;
    m_debugLevel  = IVA_LOG_LEVEL_DEBUG & logLevelVal;
    m_inforLevel  = IVA_LOG_LEVEL_INFOR & logLevelVal;
    m_warngLevel  = IVA_LOG_LEVEL_WARNG & logLevelVal;
    m_errorLevel  = IVA_LOG_LEVEL_ERROR & logLevelVal;
    m_critcLevel  = IVA_LOG_LEVEL_CRITC & logLevelVal;
  
    return LCT_SUCCESS;
}

CLctLogConfigMgr::CLctLogConfigMgr()
{
}

 CLctLogConfigMgr::~CLctLogConfigMgr()
{
}


