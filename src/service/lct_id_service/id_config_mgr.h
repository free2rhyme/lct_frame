/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_config_mgr.h
 * @version     1.0
 * @date        Jul 1, 2017 5:52:24 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_SERVICE_LCT_ID_SERVICE_ID_CONFIG_MGR_H_
#define SRC_SERVICE_LCT_ID_SERVICE_ID_CONFIG_MGR_H_

#include "id_common_define.h"
#include "lct_singleton.h"

class CSidConfigMgr final:public CLctSingleton<CSidConfigMgr>
{
public:
    LCT_ERR_CODE init(const std::string& configFilePath = CONFIG_FILE_DEFAULT_PATH);

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
    CSidConfigMgr();
    ~CSidConfigMgr();

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

#define SID_CONFIG_MGR CSidConfigMgr::instance()



#endif /* SRC_SERVICE_LCT_ID_SERVICE_ID_CONFIG_MGR_H_ */
