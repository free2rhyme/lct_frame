/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        poi_config_mgr.cpp
 * @version     1.0
 * @date        Oct 24, 2017 12:10:48 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "poi_config_mgr.h"
#include "lct_properties.h"
#include "lct_log.h"

CPoiConfigManager::CPoiConfigManager()
{
}

CPoiConfigManager::~CPoiConfigManager()
{
}

LCT_ERR_CODE CPoiConfigManager::init(const std::string& file)
{
    CLctProperties properties(file);
    LCT_ERR_CODE errCode = properties.parse();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to parse properties file(" << file
                << ") due to error(" << ErrCodeFormat(errCode) << ")";
        return errCode;
    } else {
        LOG_DEBUG << "Successful to load Properties file";
    }

    m_servicePort                      = properties.getInt32 ("Poi.Correlate.Service.Port");
    m_serviceIp                        = properties.getString("Poi.Correlate.Service.Ip");

    m_idServicePort                    = properties.getInt32 ("Poi.Correlate.Id.Service.Port");
    m_idServiceIp                      = properties.getString("Poi.Correlate.Id.Service.Ip");

    m_databasePort                     = properties.getInt32 ("Poi.Correlate.Database.Port");
    m_databaseHost                     = properties.getString("Poi.Correlate.Database.Host");
    m_keyspace                         = properties.getString("Poi.Correlate.Database.Keyspace");

    m_featureTableNm                   = properties.getString("Poi.Correlate.Feature.Table.Name");
    m_taskKeyspace                     = properties.getString("Poi.Correlate.Task.Keyspace");
    m_taskTableNm                      = properties.getString("Poi.Correlate.Task.Table.Name");
    m_similarityTablePrefix            = properties.getString("Poi.Correlate.Similarity.TablePrefix");

    m_logDir                           = properties.getString("Poi.Correlate.Log.Dir");
    m_logFileSize                      = properties.getInt64 ("Poi.Correlate.Log.File.Size");
    m_logFileCount                     = properties.getInt64 ("Poi.Correlate.Log.File.Count");

    m_featureVersion                   = properties.getInt64 ("Poi.Correlate.Feature.Version");
    m_workerThreadCount                = properties.getInt32 ("Poi.Correlate.Service.WorkerCount");
    m_cassBatchChunkSize               = properties.getInt32 ("Poi.Correlate.Cass.BatchChunkSize");

    return LCT_SUCCESS;
}

