/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_time.cpp
 * @version     1.0
 * @date        Sep 24, 2017 11:22:10 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_time.h"
#include <iomanip>
#include <cstring>

int64_t CLctTime::currentDateTimeValue() const
{
   return std::chrono::duration_cast<LctTimeValueType>(LctClockType::now().time_since_epoch()).count();
}

//YYYY-MM-DD HH:MM:SS
std::string CLctTime::formatDateTime(const int64_t& millisecondsSinceEpoch) const
{
   const auto durationSinceEpoch = LctTimeValueType(millisecondsSinceEpoch);
   const LctTimePointType tpAfterDuration(durationSinceEpoch);
   LctTmType tstruct = timePoint2Tm(tpAfterDuration);
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
   return buf;
}

//@return: YYYY-MM-DD HH:MM:SS +8000 UTC
std::string CLctTime::formatUtcDateTime(const int64_t& millisecondsSinceEpoch) const
{
   const auto durationSinceEpoch = LctTimeValueType(millisecondsSinceEpoch);
   const LctTimePointType tpAfterDuration(durationSinceEpoch);
   LctTmType tstruct = timePoint2Tm(tpAfterDuration);
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%Y-%m-%d %X %z UTC", &tstruct);
   return buf;
}

//HH:MM:SS
std::string CLctTime::formatTime(const int64_t& millisecondsSinceEpoch) const
{
   const auto durationSinceEpoch = LctTimeValueType(millisecondsSinceEpoch);
   const LctTimePointType tpAfterDuration(durationSinceEpoch);
   LctTmType tstruct = timePoint2Tm(tpAfterDuration);
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%X", &tstruct);
   return buf;
}

//YYYY-MM-DD
std::string CLctTime::formatedDate() const
{
   LctTmType tstruct = timePoint2Tm(LctClockType::now());
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
   return buf;
}

//YYYY-MM-DD HH:MM:SS
std::string CLctTime::currentDateTime() const
{
   LctTmType tstruct = timePoint2Tm(LctClockType::now());
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
   return buf;
}

//HH:MM:SS
std::string CLctTime::currentTime() const
{
   LctTmType tstruct = timePoint2Tm(LctClockType::now());
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%X", &tstruct);
   return buf;
}

//YYYYMMDD
std::string CLctTime::currentDate() const
{
   LctTmType tstruct = timePoint2Tm(LctClockType::now());
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);
   return buf;
}

//YYMMDD
std::string CLctTime::currentDay() const
{
   LctTmType tstruct = timePoint2Tm(LctClockType::now());
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%y%m%d", &tstruct);
   return buf;
}

//@return: YYYY-MM-DD HH:MM:SS +8000 UTC
const std::string CLctTime::currentUtcTime() const
{
   LctTmType tstruct = timePoint2Tm(LctClockType::now());
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%Y-%m-%d %X %z UTC", &tstruct);
   return buf;
}

//from YYYY-MM-DD HH:MM:SS to YYYY-MM-DD
std::string CLctTime::formatedTime2Date(const std::string& time) const
{
   if (time.size() != 19)
   {
      return "";
   }
   return time.substr(0, time.find_first_of(' '));
}

//from YYYY-MM-DD HH:MM:SS to YYYYMMDD
int32_t CLctTime::formatedTime2IntDate(const std::string& time) const
{
   static constexpr std::size_t fixedLen = strlen("YYYY-MM-DD HH:MM:SS");
   if (time.size() != fixedLen)
   {
      return 0;
   }
   try{
      const int32_t year  = std::stoi(time.substr(0, 4));
      const int32_t month = std::stoi(time.substr(5, 2));
      const int32_t date  = std::stoi(time.substr(8, 2));
      return year * 10000 + month * 100 + date;
   } catch (const std::exception& e) {
     return 0;
   } catch (...) {
     return 0;
   }
}

//from YYYY-MM-DD HH:MM:SS.000000+0800 to UTC seconds
int64_t CLctTime::utcTime2Seconds(const std::string& time) const
{
   static constexpr std::size_t fixedLen = strlen("YYYY-MM-DD HH:MM:SS.000000+0800");
   if (time.size() != fixedLen) {
      return 0;
   }

   const std::string utcTime = time.substr(0, 19) + time.substr(26, 5);

   try {
      LctTmType ts;
      memset(&ts, 0, sizeof(ts));
      strptime(utcTime.data(), "%Y-%m-%d %H:%M:%S %z", &ts);
      return mktime(&ts);
   } catch (const std::exception& e) {
     return 0;
   } catch (...) {
     return 0;
   }
}


LCT_ERR_CODE CLctTime::initialize()
{
   std::time_t now  = LctClockType::to_time_t(LctClockType::now());
   LctTmType tstruct = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr};
   tstruct = *localtime_r(&now, &tstruct);

   char buf[20] = {0};
   strftime(buf, sizeof(buf), "%z", &tstruct);

   try{
      const int32_t hour1   = buf[1] - '0';
      const int32_t hour2   = buf[2] - '0';
      const int32_t minute1 = buf[3] - '0';
      const int32_t minute2 = buf[4] - '0';
       m_timezoneGap = 3600 * (hour1 * 10 + hour2) + 60 * (minute1 * 10 + minute2);
       m_timezoneGap *= ('-' == buf[0]) ? -1 : 1;
   } catch (const std::exception& e) {
      return LCT_FAIL;
   } catch (...) {
      return LCT_FAIL;
   }
   return LCT_SUCCESS;
}
