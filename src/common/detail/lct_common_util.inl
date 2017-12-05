/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_common_util.inl
 * @version     1.0
 * @date        Oct 12, 2017 2:44:08 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_DETAIL_LCT_COMMON_UTIL_INL_
#define SRC_COMMON_DETAIL_LCT_COMMON_UTIL_INL_

#include "lct_time.h"

typedef std::chrono::system_clock          LctClockType;
typedef std::chrono::time_point<LctClockType> LctTimePointType;
typedef std::chrono::milliseconds          LctTimeValueType;

//milli seconds sice epoch
inline int64_t LctCurrentDateTimeValue()
{
   return std::chrono::duration_cast<LctTimeValueType>(LctClockType::now().time_since_epoch()).count();;
}

inline std::string LctFormatDateTime(const int64_t& millisecondsSinceEpoch)
{
   return LCT_TIME->formatDateTime(millisecondsSinceEpoch);
}

//@return: YYYY-MM-DD HH:MM:SS +8000 UTC
inline std::string LctFormatUtcDateTime(const int64_t& millisecondsSinceEpoch)
{
   return LCT_TIME->formatUtcDateTime(millisecondsSinceEpoch);
}

inline std::string LctFormatTime(const int64_t& millisecondsSinceEpoch)
{
   return LCT_TIME->formatTime(millisecondsSinceEpoch);
}

inline const std::string LctCurrentDateTime()  //YYYY-MM-DD HH:MM:SS
{
   return LCT_TIME->currentDateTime();
}

inline const std::string LctCurrentTime()  //HH:MM:SS
{
   return LCT_TIME->currentTime();
}

inline const std::string LctCurrentDate()  //YYYYMMDD
{
   return LCT_TIME->currentDate();
}

inline const std::string LctCurrentDay()   //YYMMDD
{
   return LCT_TIME->currentDay();
}

//@return: YYYY-MM-DD HH:MM:SS +8000 UTC
inline const std::string LctCurrentUtcTime()
{
   return LCT_TIME->currentUtcTime();
}

//@argv:   YYYY-MM-DD HH:MM:SS.000000+8000
//@return: UTC seconds
inline int64_t LctUtcTime2Seconds(const std::string& time)
{
   return LCT_TIME->utcTime2Seconds(time);
}

inline int64_t IvaLocal2UtcTimezoneGap()
{
   return LCT_TIME->timeZoneGap();
}

#endif /* SRC_COMMON_DETAIL_LCT_COMMON_UTIL_INL_ */
