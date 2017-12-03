/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        poi_config_mgr.inl
 * @version     1.0
 * @date        Oct 24, 2017 12:10:46 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_TOOL_POI_SIM_TABLE_PREP_CONFIG_MGR_INL_
#define SRC_TOOL_POI_SIM_TABLE_PREP_CONFIG_MGR_INL_


inline uint16_t CPoiConfigManager::servicePort() const
{
    return m_servicePort;
}

inline std::string CPoiConfigManager::serviceIp() const
{
    return m_serviceIp;
}

inline uint16_t CPoiConfigManager::idServicePort() const
{
    return m_idServicePort;
}

inline std::string CPoiConfigManager::idServiceIp() const
{
    return m_idServiceIp;
}

inline std::string CPoiConfigManager::logDir() const
{
    return m_logDir;
}

inline int64_t CPoiConfigManager::logFileSize() const
{
    return m_logFileSize;
}

inline int64_t CPoiConfigManager::logFileCount() const
{
    return m_logFileCount;
}

inline int64_t CPoiConfigManager::featureVersion() const
{
    return m_featureVersion;
}

inline uint16_t CPoiConfigManager::databasePort() const
{
    return m_databasePort;
}

inline std::string CPoiConfigManager::databaseHost() const
{
    return m_databaseHost;
}

inline std::string CPoiConfigManager::keyspace() const
{
    return m_keyspace;
}

inline std::string CPoiConfigManager::featureTable() const
{
    return m_featureTableNm;
}

inline std::string CPoiConfigManager::taskKeyspace() const
{
    return m_taskKeyspace;
}

inline std::string CPoiConfigManager::taskTable() const
{
    return m_taskTableNm;
}

inline std::string CPoiConfigManager::similarityTablePrefix() const
{
    return m_similarityTablePrefix;
}

inline int32_t CPoiConfigManager::workerThreadCount() const
{
    return m_workerThreadCount;
}

inline int32_t CPoiConfigManager::cassBatchChunkSize() const
{
    return m_cassBatchChunkSize;
}

#endif /* SRC_TOOL_POI_SIM_TABLE_PREP_CONFIG_MGR_INL_ */
