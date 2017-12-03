/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_data_source.cpp
 * @version     1.0
 * @date        Oct 17, 2017 10:18:54 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/


#include "id_data_source.h"

#include "lct_cassandra_helper.h"

CSidDataSource::CSidDataSource()
{
}


CSidDataSource::~CSidDataSource()
{
    close();
}

LCT_ERR_CODE CSidDataSource::init(
        const uint16_t dbPort,
        const std::string& dbHosts,
        const std::string& keyspace,
        const std::string& idTable)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    m_keyspace     = keyspace;
    m_dbPort       = dbPort;
    m_dbHosts      = dbHosts;
    m_idTable      = idTable;

    errCode = LCT_CASSANDRA_HELPER->init(dbPort, dbHosts);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to init cassandra helper for dbPort(" << m_dbPort << ") dbHosts(" << m_dbHosts << ")";
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CSidDataSource::close()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    errCode = LCT_CASSANDRA_HELPER->close();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to close cassandra helper for dbPort(" << m_dbPort << ") dbHosts(" << m_dbHosts << ")";
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CSidDataSource::query(const LCT_SEED_ID_TYPE seedId, const ESeedType seedType, CEngineInfo& info)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    std::stringstream ss;
    ss << " SELECT id_max, seed_step, prealloc_valve, prealloc_size, prealloc_time ";
    ss << " FROM " << m_keyspace << "." << m_idTable;
    ss << " WHERE seed_id = "    << seedId;
    ss << " AND seed_type = "    << static_cast<int32_t>(seedType);

    auto fn = [&info](const CassRow* row){
        cass_int64_t idMax = 0;
        cass_value_get_int64(cass_row_get_column(row, 0), &idMax);

        cass_int32_t seedStep = 0;
        cass_value_get_int32(cass_row_get_column(row, 1), &seedStep);

        cass_int64_t preallocValue = 0;
        cass_value_get_int64(cass_row_get_column(row, 2), &preallocValue);

        cass_int64_t preallocSize = 0;
        cass_value_get_int64(cass_row_get_column(row, 3), &preallocSize);

        const char* preallocTime = nullptr;
        size_t ptLength = 0;
        cass_value_get_string(cass_row_get_column(row, 4), &preallocTime, &ptLength);

        info.Max              = idMax;
        info.Step             = seedStep;
        info.PreallocValve    = preallocValue;
        info.PreallocSize     = preallocSize;
        info.DayStamp.assign(preallocTime, ptLength);
    };

    errCode = LCT_CASSANDRA_HELPER->query(m_keyspace, ss.str(), fn);
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to query id table for seedId(" << seedId << ")";
        return errCode;
    }

    return errCode;
}

LCT_ERR_CODE CSidDataSource::persist(const LCT_SEED_ID_TYPE seedId, const ESeedType seedType, const CEngineInfo& info)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    std::stringstream ss;
    ss << " UPDATE " << m_keyspace << "." << m_idTable << " SET ";
    ss << " prealloc_valve =  "  << info.PreallocValve    << " , ";
    ss << " prealloc_time  = '"  << info.DayStamp         << "'  ";
    ss << " WHERE seed_id  =  "  << seedId;
    ss << " AND seed_type  = "   << static_cast<int32_t>(seedType);

    errCode = LCT_CASSANDRA_HELPER->update(m_keyspace, ss.str());
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to update id table into db with cql(" << ss.str() << ")";
        return errCode;
    }

    return errCode;
}
