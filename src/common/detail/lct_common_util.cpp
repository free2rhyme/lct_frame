/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_common_util.cpp
 * @version      1.0
 * @date         May 17, 2017 11:33:26 AM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/

#include "lct_common_util.h"

#include <iostream>
//#include <Eigen/Dense>
#include <iomanip>

typedef std::chrono::system_clock             LctClockType;
typedef std::chrono::time_point<LctClockType> LctTimePointType;
typedef std::chrono::milliseconds             LctTimeValueType;

//milli seconds sice epoch
extern const int64_t LctCurrentDateTimeValue()
{
    return std::chrono::duration_cast<LctTimeValueType>(LctClockType::now().time_since_epoch()).count();;
}


std::string LctFormatDateTime(const int64_t& millisecondsSinceEpoch)
{
    const auto durationSinceEpoch = LctTimeValueType(millisecondsSinceEpoch);
    const LctTimePointType tpAfterDuration(durationSinceEpoch);
    std::time_t val = LctClockType::to_time_t(tpAfterDuration);
    struct tm tstruct;
    tstruct = *localtime_r(&val, &tstruct);
    char buf[80] = {0};
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

std::string LctFormatTime(const int64_t& millisecondsSinceEpoch)
{
    const auto durationSinceEpoch = LctTimeValueType(millisecondsSinceEpoch);
    const LctTimePointType tpAfterDuration(durationSinceEpoch);
    std::time_t val = LctClockType::to_time_t(tpAfterDuration);
    struct tm tstruct;
    tstruct = *localtime_r(&val, &tstruct);
    char buf[80] = {0};
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

const std::string LctCurrentDateTime()  //YYYY-MM-DD HH:MM:SS
{
    std::time_t now  = LctClockType::to_time_t(LctClockType::now());
    struct tm tstruct;
    tstruct = *localtime_r(&now, &tstruct);
    char buf[80] = {0};
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

const std::string LctCurrentTime()  //HH:MM:SS
{
    std::time_t now  = LctClockType::to_time_t(LctClockType::now());
    struct tm tstruct;
    tstruct = *localtime_r(&now, &tstruct);
    char buf[80] = {0};
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

const std::string LctCurrentDate()  //YYYYMMDD
{
    std::time_t now  = LctClockType::to_time_t(LctClockType::now());
    struct tm tstruct;
    tstruct = *localtime_r(&now, &tstruct);
    char buf[80] = {0};
    strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);
    return buf;
}

const std::string LctCurrentDay()   //YYMMDD
{
    std::time_t now  = LctClockType::to_time_t(LctClockType::now());
    struct tm tstruct;
    tstruct = *localtime_r(&now, &tstruct);
    char buf[80] = {0};
    strftime(buf, sizeof(buf), "%y%m%d", &tstruct);
    return buf;
}

const std::string LctFormatedTime2Date(const std::string& time)
{
    if (time.size() != 19)
    {
        return "";
    }
    return time.substr(0, time.find_first_of(' '));
}

const int32_t LctFormatedTime2IntDate(const std::string& time)
{
    if (time.size() != 19)
    {
        return 0;
    }
    try{
        const int32_t year  = std::stoi(time.substr(0, 4));
        const int32_t month = std::stoi(time.substr(5, 2));
        const int32_t date  = std::stoi(time.substr(8, 2));
        return year * 1000 + month * 100 + date;
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

//float CompareFeature(const SFeatureHead& inputf, const SFeatureHead& dbf)
//{
//    if (inputf.version != dbf.version || inputf.dim != dbf.dim)
//    {
//        return LCT_INVALID_PIC_SIMILARITY;
//    }
//
//    Eigen::Map<const Eigen::VectorXf> v1(inputf.data, inputf.dim), v2(dbf.data, dbf.dim);
//
//    return v1.dot(v2);
//}


class LctCalenderTool
{
private:
    static bool IsLeapYear(int32_t year) 
    {
        if ((year%4==0 && year%100!=0) || (year%400==0)){
            return true;
        }else{
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
        if (IsLeapYear(GetYear(date))) 
            return leapMonth[GetMonth(date) - 1] == GetDay(date);
        else 
            return month[GetMonth(date) - 1] == GetDay(date);
    }

    static bool IsEndOfYear(int32_t date)
    {
        if (GetMonth(date) == 12) 
            return true;
        return false;
    }

    static int32_t GetNextDate(int32_t date) {
        if (IsEndOfMonth(date)) {
            if (IsEndOfYear(date)) {
                date = (date / 10000 + 1) * 10000 + 101;
            }
            else {
                date = (date / 100 + 1) * 100 + 1;
            }
        }
        else {
            date += 1;
        }
        return date;
    }

    static std::vector<int32_t> GetRangeDate(const int32_t beginDate, const int32_t endDate)
    {
        std::vector<int32_t> dates;

        if (beginDate > endDate)
            return dates;

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

const std::vector<int32_t> LctCalenderTool::month     = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const std::vector<int32_t> LctCalenderTool::leapMonth = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

const std::vector<int32_t> RetrieveRangeDate(const int32_t beginDate, const int32_t endDate)
{
    return LctCalenderTool::GetRangeDate(beginDate, endDate);
}
