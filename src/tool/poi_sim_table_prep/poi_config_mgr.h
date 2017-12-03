/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        poi_config_mgr.h
 * @version     1.0
 * @date        Oct 24, 2017 12:10:46 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_TOOL_POI_SIM_TABLE_PREP_CONFIG_MGR_H_
#define SRC_TOOL_POI_SIM_TABLE_PREP_CONFIG_MGR_H_

#include "lct_singleton.h"
#include "lct_common_define.h"

class CPoiConfigManager final:public CLctSingleton<CPoiConfigManager>
{
private:
    static constexpr uint16_t          UNINITIALIZED_PORT     = 0;
    static constexpr const char* const UNINITIALIZED_IP       = "";
    static constexpr LctEventIdType    UNINITIALIZED_EVENT_ID = -1;

public:
    LCT_ERR_CODE init(const std::string& file = LOCAL_CONFIG_FILE_PATH);

    uint16_t servicePort()  const;
    std::string serviceIp() const;

    uint16_t idServicePort()  const;
    std::string idServiceIp() const;

    std::string logDir()   const;
    int64_t logFileSize()  const;
    int64_t logFileCount() const;

    int64_t featureVersion() const;

    uint16_t databasePort() const;
    std::string databaseHost() const;
    std::string keyspace() const;
    std::string featureTable() const;
    std::string taskKeyspace() const;
    std::string taskTable() const;
    std::string similarityTablePrefix() const;

    int32_t  workerThreadCount() const;
    int32_t  cassBatchChunkSize() const;

private:
    CPoiConfigManager();
    ~CPoiConfigManager();

    friend class CLctSingleton;

private:
    uint16_t               m_servicePort                    = UNINITIALIZED_PORT;
    std::string            m_serviceIp                      = UNINITIALIZED_IP;

    uint16_t               m_idServicePort                  = UNINITIALIZED_PORT;
    std::string            m_idServiceIp                    = UNINITIALIZED_IP;

    uint16_t               m_databasePort                   = UNINITIALIZED_PORT;
    std::string            m_databaseHost                   = UNINITIALIZED_IP;
    std::string            m_keyspace;

    std::string            m_featureTableNm;
    std::string            m_taskKeyspace;
    std::string            m_taskTableNm;
    std::string            m_similarityTablePrefix;

    std::string            m_logDir;
    int64_t                m_logFileSize                    = 0;
    int64_t                m_logFileCount                   = 0;

    int64_t                m_featureVersion                 = 0;
    int32_t                m_workerThreadCount              = 0;
    int32_t                m_cassBatchChunkSize             = 0;
};

#define POI_CONFIG_MGR CPoiConfigManager::instance()

#include "poi_config_mgr.inl"

#endif /* SRC_TOOL_POI_SIM_TABLE_PREP_CONFIG_MGR_H_ */
