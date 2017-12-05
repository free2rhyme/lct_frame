/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_time.h
 * @version     1.0
 * @date        Sep 24, 2017 11:22:08 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_TIME_INL_
#define SRC_COMMON_LCT_TIME_INL_

inline CLctTime::LctTmType CLctTime::timePoint2Tm(const LctTimePointType& tmPoint) const
{
   LctTmType tstruct = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr};
   std::time_t val = LctClockType::to_time_t(tmPoint) + m_timezoneGap;
   tstruct = *gmtime_r(&val, &tstruct);
   return tstruct;
}

inline int64_t CLctTime::timeZoneGap() const
{
   return m_timezoneGap;
}

#endif /* SRC_COMMON_LCT_TIME_INL_ */


