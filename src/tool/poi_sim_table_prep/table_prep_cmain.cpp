/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_task_prep_cmain.cpp
 * @version     1.0
 * @date        Oct 24, 2017 11:18:30 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_common_define.h"
#include "poi_config_mgr.h"
#include "lct_cassandra_helper.h"
#include "lct_log.h"


static void usage(const char* executive)
{
    std::cout << "usage:" << "\n\t";
    std::cout << executive << "                    # service booting up with default config_file\n\t";
    std::cout << executive << " -f config_file     # service booting up with specified config_file\n\t" << std::endl;
}

static LCT_ERR_CODE argvHandle(int argc, char* argv[], std::string& configFilePath)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    switch(argc) {
        case 1: {
            break;
        }
        case 3: {
            if (std::string(argv[1]) != std::string("-f")) {
                errCode = LCT_FAIL;
                break;
            }
            configFilePath = argv[2];
            break;
        }
        default: {
            errCode = LCT_FAIL;
        }
    }

    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    return errCode;
}

static LCT_ERR_CODE createTaskTable()
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    std::stringstream ss;
    ss << " CREATE TABLE IF NOT EXISTS " << POI_CONFIG_MGR->taskKeyspace() << "." << POI_CONFIG_MGR->taskTable() << " (";
    ss << " task_id bigint, ";
    ss << " created_time bigint, ";
    ss << " started_time bigint, ";
    ss << " finished_time bigint, ";
    ss << " comment text, ";
    ss << " target_a_route text, ";
    ss << " target_b_route text, ";
    ss << " topk int, ";
    ss << " threshold float, ";
    ss << " status int, ";
    ss << " priority int, ";
    ss << " progress bigint, ";
    ss << " PRIMARY KEY (task_id) ";
    ss << " ); ";

    errCode = LCT_CASSANDRA_HELPER->createTable(POI_CONFIG_MGR->taskKeyspace(), ss.str());
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to create task table";
        return errCode;
    } else {
        LOG_DEBUG << "Successful to create table(" << POI_CONFIG_MGR->taskTable() << ")";
    }

    return errCode;
}

int main(int argc, char* argv[]) 
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;
    std::string configFilePath = "";
    errCode = argvHandle(argc, argv, configFilePath);
    if (LCT_SUCCESS != errCode) {
        if (LCT_VERSION_INFO == errCode) {
            return 0;
        }
        usage(argv[0]);
        return errCode;
    }

    if (configFilePath.empty()) {
        errCode = POI_CONFIG_MGR->init();
    } else {
        errCode = POI_CONFIG_MGR->init(configFilePath);
    }

    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to init config";
        return errCode;
    }
    
    errCode = LCT_CASSANDRA_HELPER->init(POI_CONFIG_MGR->databasePort(), POI_CONFIG_MGR->databaseHost());
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to init cassandra helper for dbPort(" 
            << POI_CONFIG_MGR->databasePort() << ") dbHosts(" << POI_CONFIG_MGR->databaseHost() << ")";
        return errCode;
    }

    do {

        errCode = LCT_CASSANDRA_HELPER->createKeyspace(POI_CONFIG_MGR->keyspace());
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to create keyspace(" << POI_CONFIG_MGR->keyspace() << ")";
            break;
        } else {
            LOG_DEBUG << "Successful to create keyspace(" << POI_CONFIG_MGR->keyspace() << ")";
        }

        errCode = LCT_CASSANDRA_HELPER->createKeyspace(POI_CONFIG_MGR->taskKeyspace());
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to create keyspace(" << POI_CONFIG_MGR->taskKeyspace() << ")";
            break;
        } else {
            LOG_DEBUG << "Successful to create keyspace(" << POI_CONFIG_MGR->taskKeyspace() << ")";
        }

        errCode = createTaskTable();
        if (LCT_SUCCESS != errCode) {
            LOG_ERROR << "Failed to create task table";
            break;
        }
    } while (0);

    errCode = LCT_CASSANDRA_HELPER->close();
    if (LCT_SUCCESS != errCode) {
        LOG_ERROR << "Failed to close cassandra helper for dbPort(" << POI_CONFIG_MGR->databasePort() << ") dbHosts(" << POI_CONFIG_MGR->databaseHost() << ")";
        return errCode;
    }


	return 0;
}
