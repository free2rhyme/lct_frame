/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_common_define.h
 * @version     1.0
 * @date        May 17, 2017 11:30:57 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_COMMON_DEFINE_H_
#define SRC_COMMON_LCT_COMMON_DEFINE_H_

#include <iostream>
#include <unistd.h>
#include <memory>
#include <typeinfo>
#include <vector>
#include <unordered_map>

#include "lct_error_code.h"

static constexpr const char* const CONFIG_FILE_DEFAULT_PATH = "./config/lct_service_config.properties";
static constexpr const char* const LOCAL_CONFIG_FILE_PATH = "./config/lct_service_config.properties";

//#define log_trace()  std::cout << "ProcessId("<< getpid() <<")\t ThreadId(0X" << std::hex  << pthread_self() << ")\t" << std::dec <<  __FILE__ << "\t" << __LINE__ << "\t" << __PRETTY_FUNCTION__ << std::endl;
//#define log_debug(x) std::cout << "ProcessId("<< getpid() <<")\t ThreadId(0X" << std::hex  << pthread_self() << ")\t" << std::dec <<  __FILE__ << "\t" << __LINE__ << "\t" << __PRETTY_FUNCTION__ << "\t" << #x << "(" << x << ")"<< std::endl;
//#define log_error(x) std::cerr << "ProcessId("<< getpid() <<")\t ThreadId(0X" << std::hex  << pthread_self() << ")\t" << std::dec <<  __FILE__ << "\t" << __LINE__ << "\t" << __PRETTY_FUNCTION__ << "\t" << #x << "(" << x << ")"<< std::endl;

#define DISALLOW_INSTANTIATE(TypeName) \
TypeName() = delete; \
~TypeName() = delete

#define DISALLOW_COPY_OR_ASSIGN(TypeName) \
TypeName(const TypeName&) = delete; \
TypeName& operator=(const TypeName&) = delete

#define DISALLOW_COPY_MOVE_OR_ASSIGN(TypeName) \
TypeName(const TypeName&) = delete; \
TypeName(TypeName&&) = delete; \
TypeName& operator=(const TypeName&) = delete; \
TypeName& operator=(TypeName&&) = delete;

#define DEFAULT_COPY_MOVE_AND_ASSIGN(TypeName) \
TypeName(const TypeName&) = default; \
TypeName(TypeName&&) = default; \
TypeName& operator=(const TypeName&) = default; \
TypeName& operator=(TypeName&&) = default;

typedef int64_t   LctIdType;
typedef LctIdType LctEventIdType;
typedef LctIdType LctTaskIdType;

struct CNullMutex
{
   void lock()
   {
   }
   void unlock()
   {
   }
   bool try_lock()
   {
      return true;
   }
};

template <typename T>
const char* ClassName(const T&)
{
   return typeid(T).name();
}

template <typename T>
const char* ClassName(const std::shared_ptr<const T>&)
{
   return typeid(T).name();
}

typedef int64_t CLctBizType;
constexpr static CLctBizType LCT_BIZ_EVENT_TYPE = 0x0001;
constexpr static CLctBizType LCT_BIZ_TASKS_TYPE = 0x0002;
constexpr static CLctBizType LCT_BIZ_MODEL_TYPE = 0x0003;

typedef int32_t LCT_SEED_ID_TYPE;
static constexpr LCT_SEED_ID_TYPE LCT_EVENT_ENGINE_SEED_ID = 1001;
static constexpr LCT_SEED_ID_TYPE LCT_TASKS_ENGINE_SEED_ID = 1002;
static constexpr LCT_SEED_ID_TYPE LCT_MODEL_ENGINE_SEED_ID = 1003;

typedef struct LctDatabaseInfo
{
   int32_t Port = 0;
   std::string Ip;
   std::string DatabaseName;
   std::string TableName;
   LctDatabaseInfo()
   {
   }
   LctDatabaseInfo(const int32_t prt, const std::string& iip, const std::string& dbNm, const std::string& tbNm) :
         Port(prt), Ip(iip), DatabaseName(dbNm), TableName(tbNm)
   {
   }
} CLctDatabaseInfo;

typedef std::shared_ptr<CLctDatabaseInfo>       CLctDatabaseInfoShp;
typedef std::vector<CLctDatabaseInfo>           CLctDatabaseInfoVector;
typedef std::shared_ptr<CLctDatabaseInfoVector> CLctDatabaseInfoVectorShp;

#endif /* SRC_COMMON_LCT_COMMON_DEFINE_H_ */
