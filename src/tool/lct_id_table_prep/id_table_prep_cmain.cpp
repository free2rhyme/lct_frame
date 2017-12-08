/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_table_prep_cmain.cpp
 * @version     1.0
 * @date        Oct 24, 2017 11:18:30 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "../lct_id_table_prep/sc_id_config_mgr.h"
#include "lct_common_define.h"
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

static LCT_ERR_CODE createIdTable()
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   std::stringstream ss;
   ss << " CREATE TABLE IF NOT EXISTS " << SC_CONFIG_MGR->keyspace() << "." << SC_CONFIG_MGR->databaseTableName() << " (";
   ss << " seed_id int, ";
   ss << " seed_type int, ";
   ss << " id_max bigint, ";
   ss << " seed_step int, ";
   ss << " prealloc_valve bigint, ";
   ss << " prealloc_size bigint, ";
   ss << " prealloc_time text, ";
   ss << " PRIMARY KEY (seed_id, seed_type)";
   ss << " ); ";

   errCode = LCT_CASSANDRA_HELPER->createTable(SC_CONFIG_MGR->keyspace(), ss.str());
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to create id table";
      return errCode;
   } else {
      LOG_DEBUG << "Successful to create Id table" << SC_CONFIG_MGR->databaseTableName() << ")";
   }

   return errCode;
}


struct CEngineInfo final
{
   CEngineInfo(){}
   ~CEngineInfo(){}
   int32_t                 Step              = 1;
   int64_t                 PreallocSize      = 100;
   int64_t                 Max               = std::numeric_limits<int64_t>::max();
   int64_t                 PreallocValve     = 0;
   std::atomic<int64_t>    CurrentId         = {0};
   std::string             DayStamp;

   DEFAULT_COPY_MOVE_AND_ASSIGN(CEngineInfo);
};

static LCT_ERR_CODE insertIdTable(const int32_t seedId, const int32_t seedType, const CEngineInfo& engineInfo)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   std::stringstream ss;
   ss << " INSERT INTO " << SC_CONFIG_MGR->keyspace() << "." << SC_CONFIG_MGR->databaseTableName() << " (";
   ss << " seed_id,         ";
   ss << " seed_type,       ";
   ss << " id_max,          ";
   ss << " seed_step,       ";
   ss << " prealloc_valve,  ";
   ss << " prealloc_size,   ";
   ss << " prealloc_time )  ";
   ss << " VALUES (";
   ss << seedId                       << " , ";
   ss << seedType                     << " , ";
   ss << engineInfo.Max               << " , ";
   ss << engineInfo.Step              << " , ";
   ss << engineInfo.PreallocValve     << " , ";
   ss << engineInfo.PreallocSize      << " , ";
   ss << " ' " << engineInfo.DayStamp << "'  ";
   ss << " ); ";

   errCode = LCT_CASSANDRA_HELPER->insert(SC_CONFIG_MGR->keyspace(), ss.str());
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to insert id to table";
      return errCode;
   }

   return errCode;
}

static LCT_ERR_CODE initEngine() 
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;
   CEngineInfo engine;

   errCode = insertIdTable(LCT_EVENT_ENGINE_SEED_ID, 2, engine);
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to insert engineId(" << LCT_EVENT_ENGINE_SEED_ID << ") into table error " << ErrCodeFormat(errCode);
      return errCode;
   } else {
       LOG_DEBUG << "Successful to insert engineId(" << LCT_EVENT_ENGINE_SEED_ID << ") into table ";
   }

   errCode = insertIdTable(LCT_TASKS_ENGINE_SEED_ID, 2, engine);
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to insert engineId(" << LCT_TASKS_ENGINE_SEED_ID << ") into table error " << ErrCodeFormat(errCode);
      return errCode;
   } else {
      LOG_DEBUG << "Successful to insert engineId(" << LCT_TASKS_ENGINE_SEED_ID << ") into table ";
   }


   errCode = insertIdTable(LCT_MODEL_ENGINE_SEED_ID, 2, engine);
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to insert engineId(" << LCT_MODEL_ENGINE_SEED_ID << ") into table error " << ErrCodeFormat(errCode);
      return errCode;
   } else {
      LOG_DEBUG << "Successful to insert engineId(" << LCT_MODEL_ENGINE_SEED_ID << ") into table ";
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
      errCode = SC_CONFIG_MGR->init();
   } else {
      errCode = SC_CONFIG_MGR->init(configFilePath);
   }

   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to init config";
      return errCode;
   }

   errCode = LCT_CASSANDRA_HELPER->init(SC_CONFIG_MGR->databasePort(), SC_CONFIG_MGR->databaseIp());
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to init cassandra helper for dbPort("
         << SC_CONFIG_MGR->databasePort() << ") dbHosts(" << SC_CONFIG_MGR->databaseIp() << ")";
      return errCode;
   }

   do {
      errCode = LCT_CASSANDRA_HELPER->createKeyspace(SC_CONFIG_MGR->keyspace());
      if (LCT_SUCCESS != errCode) {
          LOG_ERROR << "Failed to create keyspace(" << SC_CONFIG_MGR->keyspace() << ")";
          break;
      } else {
         LOG_DEBUG << "Successful to create keyspace(" << SC_CONFIG_MGR->keyspace() << ")";
      }

      errCode = createIdTable();
      if (LCT_SUCCESS != errCode) {
         LOG_ERROR << "Failed to create id table";
         break;
      }

      errCode = initEngine();
      if (LCT_SUCCESS != errCode) {
         LOG_ERROR << "Failed to create id table";
         break;
      }
   } while (0);

   errCode = LCT_CASSANDRA_HELPER->close();
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to close cassandra helper for dbPort(" << SC_CONFIG_MGR->databasePort() << ") dbHosts(" << SC_CONFIG_MGR->databaseIp() << ")";
      return errCode;
   }


	return 0;
}
