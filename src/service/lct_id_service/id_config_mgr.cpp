/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_config_mgr.cpp
 * @version     1.0
 * @date        Jul 1, 2017 5:52:30 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "id_config_mgr.h"

#include "lct_properties.h"
#include "lct_log.h"

LCT_ERR_CODE CSidConfigMgr::init(const std::string& configFilePath)
{
   CLctProperties properties(configFilePath);
   LCT_ERR_CODE errCode = properties.parse();
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to parse properties file(" << configFilePath
            << ") due to error(" << ErrCodeFormat(errCode) << ")";
      return errCode;
   } else {
      LOG_DEBUG << "Successful to load Properties file";
   }

   m_servicePort                      = properties.getInt32("Poi.Correlate.Id.Service.Port");
   m_databasePort                     = properties.getInt32("Poi.Correlate.Id.Database.Port");
   m_serviceIp                        = properties.getString("Poi.Correlate.Id.Service.Ip");
   m_databaseIp                       = properties.getString("Poi.Correlate.Id.Database.Ip");
   m_databaseName                     = properties.getString("Poi.Correlate.Id.Database.Name");
   m_databaseTableName                = properties.getString("Poi.Correlate.Id.Database.Table.Name");
   m_keyspace                         = properties.getString("Poi.Correlate.Id.Database.keyspace");
   m_logDir                           = properties.getString("Poi.Correlate.Id.Log.Dir");

   return LCT_SUCCESS;
}

CSidConfigMgr::CSidConfigMgr()
{
}

CSidConfigMgr::~CSidConfigMgr()
{
}
