/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_common_define.h
 * @version     1.0
 * @date        Jun 30, 2017 7:22:32 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_SERVICE_LCT_ID_SERVICE_ID_COMMON_DEFINE_H_
#define SRC_SERVICE_LCT_ID_SERVICE_ID_COMMON_DEFINE_H_

#include <atomic>
#include "lct_common_define.h"
#include "lct_error_code.h"
#include "common.pb.h"
#include "service_id.pb.h"
#include "service_id.grpc.pb.h"
#include <grpc++/server.h>

typedef lowcost::vip::serviceid::CSidRequest        LctCSidRequest;
typedef lowcost::vip::serviceid::CSidResponse       LctCSidResponse;

typedef lowcost::vip::serviceid::ServiceLctUniqueId LctServiceLctUniqueId;

typedef lowcost::vip::common::CResult             CSidResult;
typedef lowcost::vip::common::CDummyReq           CSidDummyReq;
typedef lowcost::vip::common::CDummyResp          CSidDummyResp;


static constexpr uint16_t          UNINITIALIZED_PORT     = 0;
static constexpr const char* const UNINITIALIZED_IP       = "";

enum class ESeedType:int8_t
{
   SEED_TYPE_UNIQUE          = 1,
   SEED_TYPE_DAY_SWITCH      = 2,
   SEED_TYPE_DATE_SWITCH     = 3,
   SEED_TYPE_MONTH_SWITCH    = 4,
};

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

#endif /* SRC_SERVICE_LCT_ID_SERVICE_ID_COMMON_DEFINE_H_ */
