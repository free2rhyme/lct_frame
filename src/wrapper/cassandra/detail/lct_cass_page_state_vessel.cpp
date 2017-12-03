/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_cass_page_state_vessel.cpp
 * @version     1.0
 * @date        Oct 19, 2017 6:01:29 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_cass_page_state_vessel.h"
#include "lct_common_util.h"
#include "lct_log.h"

LCT_ERR_CODE CLctCassPageStateVessel::save(const std::string& cql, const LctCassPageInfo& pageInfo)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   CLctRwlockGuard lk(m_mutex, true);

   //cost of avoiding async purging thread
   tidy();

   LctCassPageInfo copy(pageInfo);

   copy.Timestamp = LctCurrentDateTimeValue();

   //replace the old one if that exits
   m_pageStateMap[cql] = copy;

   return errCode;
}


LCT_ERR_CODE CLctCassPageStateVessel::remove(const std::string& cql)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   CLctRwlockGuard lk(m_mutex, true);

   auto it = m_pageStateMap.find(cql);
   if (m_pageStateMap.end() == it) {
      LOG_ERROR << "There is not such page state for cql(" << cql << ")";
      return LCT_NOT_SUCH_RECORD;
   }

   m_pageStateMap.erase(it);

   return errCode;
}

LCT_ERR_CODE CLctCassPageStateVessel::retrieve(const std::string& cql, LctCassPageInfo& pageInfo)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   CLctRwlockGuard lk(m_mutex);

   //cost of avoiding async purging thread
   tidy();

   auto it = m_pageStateMap.find(cql);
   if (m_pageStateMap.end() == it) {
      LOG_ERROR << "There is not such page state for cql(" << cql << ")";
      return LCT_NOT_SUCH_RECORD;
   }
   pageInfo = it->second;


   return errCode;
}

LCT_ERR_CODE CLctCassPageStateVessel::tidy()
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   const int64_t nowTime = LctCurrentDateTimeValue();

   auto it = m_pageStateMap.begin();
   while (it != m_pageStateMap.end()) {
      LctCassPageInfo& ref = it->second;

      if (nowTime - PAGE_STATE_OBSOLETE_DURATION > ref.Timestamp) {
         it = m_pageStateMap.erase(it);
      } else {
         ++it;
      }
   }

   return errCode;
}

CLctCassPageStateVessel::CLctCassPageStateVessel()
{
}

CLctCassPageStateVessel::~CLctCassPageStateVessel()
{
}



