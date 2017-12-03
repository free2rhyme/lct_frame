/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_config_mgr.h
 * @version     1.0
 * @date        Jul 20, 2017 3:55:29 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_properties.h"
#include "lct_log_config_mgr.h"

LCT_ERR_CODE CLctLogConfigMgr::init(const std::string& configFile)
{
   CLctProperties properties(configFile);
   LCT_ERR_CODE errCode = properties.parse();
   if(LCT_SUCCESS != errCode){
      std::cout << "Failed to parse Lct Log properties file(" << configFile
            << ") due to error(" << ErrCodeFormat(errCode) << ")" << std::endl;
      return errCode;
   } else {
      std::cout << "Successful to load Lct Log properties file" << std::endl;
   }

   m_traceLevel  = properties.getBool("lct_log.h.LevelEnable.Trace", true);
   m_debugLevel  = properties.getBool("lct_log.h.LevelEnable.Debug", true);
   m_inforLevel  = properties.getBool("lct_log.h.LevelEnable.Infor", true);
   m_warngLevel  = properties.getBool("lct_log.h.LevelEnable.Warng", true);
   m_errorLevel  = properties.getBool("lct_log.h.LevelEnable.Error", true);
   m_critcLevel  = properties.getBool("lct_log.h.LevelEnable.Critc", true);
  
   return LCT_SUCCESS;
}

CLctLogConfigMgr::CLctLogConfigMgr()
{
}

 CLctLogConfigMgr::~CLctLogConfigMgr()
{
}


