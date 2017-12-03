/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_common_util.h
 * @version     1.0
 * @date        May 17, 2017 11:33:14 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_COMMON_UTIL_H_
#define SRC_COMMON_LCT_COMMON_UTIL_H_

#include <string>
#include <vector>
#include <chrono>
#include "lct_common_define.h"

LCT_ERR_CODE LctVersion();

//milli seconds sice epoch
int64_t LctCurrentDateTimeValue();

//@return: YYYY-MM-DD HH:MM:SS
std::string LctFormatDateTime(const int64_t& millisecondsSinceEpoch);

//@return: YYYY-MM-DD HH:MM:SS +8000 UTC
std::string LctFormatUtcDateTime(const int64_t& millisecondsSinceEpoch);

//@return: HH:MM:SS
std::string LctFormatTime(const int64_t& millisecondsSinceEpoch);

//@return: YYYY-MM-DD HH:MM:SS
const std::string LctCurrentDateTime();

//@return: HH:MM:SS
const std::string LctCurrentTime();

//@return: YYYYMMDD
const std::string LctCurrentDate();

//@return: YYMMDD
const std::string LctCurrentDay();

//@return: YYYY-MM-DD HH:MM:SS +8000 UTC
const std::string LctCurrentUtcTime();

//@argv:   YYYY-MM-DD HH:MM:SS
//@return: YYYY-MM-DD
const std::string LctFormatedTime2Date(const std::string& time);

//@argv:   YYYY-MM-DD HH:MM:SS
//@return: YYYYMMDD
int32_t LctFormatedTime2IntDate(const std::string& time);

//@argv:   YYYY-MM-DD HH:MM:SS.000000+0800
//@return: UTC seconds
int64_t LctUtcTime2Seconds(const std::string& time);

//
//@argv:   YYYYMMDD, [beginDate , endDate]
const std::vector<int32_t> RetrieveRangeDate(const int32_t beginDate, const int32_t endDate);

//@argv:   [beginTime, endTime], seconds
const std::vector<int32_t> RetrieveRangeDate(const int64_t beginTimeSeconds, const int64_t endTimeSeconds);

std::string Bin2Hex(const std::string& input, bool uppercase = false);

/**
 * invalid character will viewed as '0', and if the input length is even then it will be prepend a '0'
 */
std::string Hex2Bin(const std::string& input);

#include "detail/lct_common_util.inl"

#endif /* SRC_COMMON_LCT_COMMON_UTIL_H_ */
