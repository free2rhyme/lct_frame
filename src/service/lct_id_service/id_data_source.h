/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_data_source.h
 * @version     1.0
 * @date        Oct 17, 2017 10:18:36 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_SERVICE_LCT_ID_SERVICE_ID_DATA_SOURCE_H_
#define SRC_SERVICE_LCT_ID_SERVICE_ID_DATA_SOURCE_H_

#include "id_common_define.h"
#include "lct_singleton.h"

class CSidDataSource final: public CLctSingleton<CSidDataSource>
{
public:
    LCT_ERR_CODE init(
            const uint16_t dbPort,
            const std::string& dbHosts,
            const std::string& keyspace,
            const std::string& idTable);

    LCT_ERR_CODE close();

    LCT_ERR_CODE query(const LCT_SEED_ID_TYPE seedId, const ESeedType seedType, CEngineInfo& info);

    LCT_ERR_CODE persist(const LCT_SEED_ID_TYPE seedId, const ESeedType seedType, const CEngineInfo& info);

public:
    CSidDataSource();
    ~CSidDataSource();

    DISALLOW_COPY_MOVE_OR_ASSIGN(CSidDataSource);

private:
    uint16_t      m_dbPort  = UNINITIALIZED_PORT;
    std::string   m_dbHosts = UNINITIALIZED_IP;
    std::string   m_keyspace;
    std::string   m_idTable;
};

#define ID_DATA_SRC CSidDataSource::instance()

#endif /* SRC_SERVICE_LCT_ID_SERVICE_ID_DATA_SOURCE_H_ */
