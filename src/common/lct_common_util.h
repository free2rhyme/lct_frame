/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_common_util.h
 * @version      1.0
 * @date         May 17, 2017 11:33:14 AM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_COMMON_UTIL_H_
#define SRC_COMMON_LCT_COMMON_UTIL_H_

#include <string>
#include <vector>
#include <chrono>
#include "lct_common_define.h"

extern const int64_t LctCurrentDateTimeValue();

extern std::string LctFormatDateTime(const int64_t& millisecondsSinceEpoch);    //YYYY-MM-DD HH:MM:SS
extern std::string LctFormatTime(const int64_t& millisecondsSinceEpoch);        //HH:MM:SS

extern const std::string LctCurrentDateTime();  //YYYY-MM-DD HH:MM:SS
extern const std::string LctCurrentTime();      //HH:MM:SS
extern const std::string LctCurrentDate();      //YYYYMMDD
extern const std::string LctCurrentDay();       //YYMMDD

extern const std::string LctFormatedTime2Date(const std::string& time);  //from YYYY-MM-DD HH:MM:SS to YYYY-MM-DD
extern const int32_t LctFormatedTime2IntDate(const std::string& time);      //from YYYY-MM-DD HH:MM:SS to YYYYMMDD

extern const std::vector<int32_t> RetrieveRangeDate(const int32_t beginDate, const int32_t endDate);

std::string Bin2Hex(const std::string& input, bool uppercase = false);

/**
 * invalid character will viewed as '0', and if the input length is even then it will be prepend a '0'
 */
std::string Hex2Bin(const std::string& input);

#pragma pack(1)

#define ST_FEATURE_MAGIC 0x4257AAEE

struct SFeatureHead
{
    unsigned int magic = ST_FEATURE_MAGIC; // compatible for historical
    unsigned int version = 0;
    unsigned int dim = 0;
    float data[];
};

#pragma pack()

//extern float CompareFeature(const SFeatureHead* inputf, const SFeatureHead* dbf);

#endif /* SRC_COMMON_LCT_COMMON_UTIL_H_ */
