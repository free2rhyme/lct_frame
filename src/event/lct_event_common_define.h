/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_event_common_define.h
 * @version     1.0
 * @date        Jun 13, 2017 5:05:45 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_EVENT_LCTEVENTCOMMONDEFINE_H_
#define SRC_EVENT_LCTEVENTCOMMONDEFINE_H_

enum class ELctTaskStatus: int16_t
{
    LCT_TASK_STATUS_UNKNOWN     = 0,
    LCT_TASK_STATUS_INIT        = 1,
    LCT_TASK_STATUS_RUNNING     = 4,
    LCT_TASK_STATUS_WAITING     = 5,
    LCT_TASK_STATUS_SUSPENDED   = 7,
    LCT_TASK_STATUS_CANCELLED   = 10,
    LCT_TASK_STATUS_SUCCESS     = 11,
    LCT_TASK_STATUS_FAIL        = 12
};

static constexpr LctEventIdType INVALID_EVENT_ID = -1;

enum class ELctProcessType: int8_t
{
    LCT_PROCESS_TYPE_UNKNOWN   = 0,
    LCT_PROCESS_TYPE_TASK      = 1,
    LCT_PROCESS_TYPE_EVENT     = 2,
    LCT_PROCESS_TYPE_JOB       = 3,
};


typedef int32_t LCTProcessPriorityType;

static constexpr LCTProcessPriorityType LCT_PROCESS_PRIORITY_HIGH = 1;
static constexpr LCTProcessPriorityType LCT_PROCESS_PRIORITY_LOW  = 9;


#endif /* SRC_EVENT_LCTEVENTCOMMONDEFINE_H_ */
