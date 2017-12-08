/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        sc_id_config_mgr.h
 * @version     1.0
 * @date        Oct 24, 2017 11:33:24 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_TOOL_LCT_ID_TABLE_PREP_ID_CONFIG_MGR_H_
#define SRC_TOOL_LCT_ID_TABLE_PREP_ID_CONFIG_MGR_H_

#include "lct_singleton.h"
#include "lct_common_define.h"

class CScConfigMgr final:public CLctSingleton<CScConfigMgr>
{
public:
   LCT_ERR_CODE init(const std::string& configFilePath = LOCAL_CONFIG_FILE_PATH);

   uint16_t servicePort() const
   {
      return m_servicePort;
   }

   std::string serviceIp() const
   {
      return m_serviceIp;
   }

   uint16_t databasePort() const
   {
      return m_databasePort;
   }

   std::string databaseIp() const
   {
      return m_databaseIp;
   }

   std::string databaseName() const
   {
      return m_databaseName;
   }

   std::string databaseTableName() const
   {
      return m_databaseTableName;
   }

   std::string keyspace() const
   {
      return m_keyspace;
   }

   std::string logDir() const
   {
      return m_logDir;
   }

private:
   CScConfigMgr();
   ~CScConfigMgr();

   friend class CLctSingleton;

private:
   uint16_t               m_servicePort           = 0;
   uint16_t               m_databasePort          = 0;
   std::string            m_serviceIp;
   std::string            m_databaseIp;
   std::string            m_databaseName;
   std::string            m_databaseTableName;
   std::string            m_keyspace;
   std::string            m_logDir;
};

#define SC_CONFIG_MGR CScConfigMgr::instance()



#endif /* SRC_TOOL_LCT_ID_TABLE_PREP_ID_CONFIG_MGR_H_ */
