/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_common_util.cpp
 * @version     1.0
 * @date        May 17, 2017 11:33:26 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_common_util.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include "version.h"

LCT_ERR_CODE LctVersion()
{
   std::stringstream ss;
   ss << BRANCH_MAJOR_VERSION << '.';
   ss << BRANCH_MINOR_VERSION << '.';
   ss << BRANCH_PATCH_NUMBER  << '.';
   ss << BRANCH_BUILD_NUMBER  << "\n\n";
   std::cout << ss.str();
   return LCT_VERSION_INFO;
}

const std::string LctFormatedTime2Date(const std::string& time)
{
   if (time.size() != 19)
   {
      return "";
   }
   return time.substr(0, time.find_first_of(' '));
}

int32_t LctFormatedTime2IntDate(const std::string& time)
{
   if (time.size() != 19) {
      return 0;
   }
   try {
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


std::string Bin2Hex(const std::string& input, bool uppercase)
{
   const char *hex;
   if (uppercase) {
      hex = "0123456789ABCDEF";
   } else {
      hex = "0123456789abcdef";
   }

   std::string res;
   res.reserve(input.size() * 2);
   for(unsigned i = 0; i < input.length(); i++) {
      const unsigned char& c = (unsigned char)input[i];
      res += hex[c >> 4];
      res += hex[c & 0xf];
   }

   return res;
}

std::string Hex2Bin(const std::string& input)
{
   std::string src = input;
   if (0 < (src.length() % 2)) {
      src = "0" + src;
   }

   std::string res;
   res.reserve(src.size()/2);

   for(unsigned i = 0; i < src.length(); i += 2) {
      unsigned char r = 0;
      {
         unsigned char c = (unsigned char)src[i];
         if ('0' <= c && '9' >= c) {
            r |= (c - '0');
         } else if ('a' <= c && 'f' >= c) {
            r |= 10 + (c - 'a');
         } else if ('A' <= c && 'F' >= c) {
            r |= 10 + (c - 'A');
         }
      }

      r = r << 4;

      {
         unsigned char c = (unsigned char)src[i+1];
         if ('0' <= c && '9' >= c) {
            r |= (c - '0');
         } else if ('a' <= c && 'f' >= c) {
            r |= 10 + (c - 'a');
         } else if ('A' <= c && 'F' >= c) {
            r |= 10 + (c - 'A');
         }
      }

      res += r;
   }

   return res;
}

class LctCalenderTool
{
private:
   static bool IsLeapYear(int32_t year)
   {
      if ((((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0)){
         return true;
      } else {
         return false;
      }
   }

   static int32_t GetYear(int32_t date) {
      return date / 10000;
   }

   static int32_t GetMonth(int32_t date) {
      return date / 100 % 100;
   }

   static int32_t GetDay(int32_t date) {
      return date % 100;
   }

public:
   static bool IsEndOfMonth(int32_t date)
   {
      if (IsLeapYear(GetYear(date))) {
         return leapMonth[GetMonth(date) - 1] == GetDay(date);
      } else {
         return month[GetMonth(date) - 1] == GetDay(date);
      }
   }

   static bool IsEndOfYear(int32_t date)
   {
      if (GetMonth(date) == 12) {
         return true;
      } else {
         return false;
      }
   }

   static int32_t GetNextDate(int32_t date)
   {
      if (IsEndOfMonth(date)) {
         if (IsEndOfYear(date)) {
            date = (date / 10000 + 1) * 10000 + 101;
         } else {
            date = (date / 100 + 1) * 100 + 1;
         }
      } else {
         date += 1;
      }
      return date;
   }

   static std::vector<int32_t> GetRangeDate(const int32_t beginDate, const int32_t endDate)
   {
      std::vector<int32_t> dates;

      if (beginDate > endDate) {
         return dates;
      }

      for (int32_t date = beginDate; date <= endDate; ) {
         dates.push_back(date);

         date = GetNextDate(date);
      }
      return dates;
   }

private:
   static const std::vector<int32_t> month;
   static const std::vector<int32_t> leapMonth;
};

const std::vector<int32_t> LctCalenderTool::month    = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const std::vector<int32_t> LctCalenderTool::leapMonth = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

const std::vector<int32_t> RetrieveRangeDate(const int32_t beginDate, const int32_t endDate)
{
   return LctCalenderTool::GetRangeDate(beginDate, endDate);
}

//@argv:   [beginTime, endTime], seconds
const std::vector<int32_t> RetrieveRangeDate(const int64_t beginTimeSeconds, const int64_t endTimeSeconds)
{
   return RetrieveRangeDate(LctFormatedTime2IntDate(LctFormatDateTime(beginTimeSeconds * 1000)),
         LctFormatedTime2IntDate(LctFormatDateTime(endTimeSeconds * 1000)));
}
