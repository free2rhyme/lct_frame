/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_day_switch_engine.h
 * @version     1.0
 * @date        Jun 30, 2017 7:27:43 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#ifndef SRC_SERVICE_LCT_ID_SERVICE_ID_DAY_SWITCH_ENGINE_H_
#define SRC_SERVICE_LCT_ID_SERVICE_ID_DAY_SWITCH_ENGINE_H_

#include "lct_common_util.h"
#include "lct_log.h"
#include <mutex>

#include "id_engine.h"

class CSidDaySwitchEngine final:public CSidEngine<CSidDaySwitchEngine>
{
public:
   LCT_ERR_CODE init();

   template <typename CIdType>
   LCT_ERR_CODE spawnId(CIdType& id)
   {
      LCT_ERR_CODE errCode = LCT_SUCCESS;

      if(LctCurrentDay() != m_engineInfo.DayStamp){
         errCode = daySwitch();
         if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            return errCode;
         }
      }

      const int64_t nextId = m_engineInfo.CurrentId.fetch_add(m_engineInfo.Step, std::memory_order_seq_cst);
      if(nextId < m_engineInfo.PreallocValve){
         id.set_date(m_engineInfo.DayStamp);
         id.set_uniqueid(nextId);
      } else {
         errCode = reallocValve();
         if(LCT_SUCCESS != errCode){
            LOG_ERROR << ErrCodeFormat(errCode);
            return errCode;
         }

         if(nextId < m_engineInfo.PreallocValve){
            id.set_date(m_engineInfo.DayStamp);
            id.set_uniqueid(nextId);
         } else {
            LOG_ERROR << ErrCodeFormat(LCT_FAIL);
            return LCT_FAIL;
         }
      }

      LOG_DEBUG << "id_date(" << id.date() << ") uniqueId(" << id.uniqueid() << ")";

      return errCode;
   }

   int32_t step() const;
   int64_t preallocSize() const;
   int64_t max() const;
   int64_t preallocValve() const;
   int64_t currentId() const;
   int64_t dayStamp() const;

public:
   explicit CSidDaySwitchEngine(const LCT_SEED_ID_TYPE& seedId);
   virtual ~CSidDaySwitchEngine();

private:
   LCT_ERR_CODE daySwitch();
   LCT_ERR_CODE reallocValve();
   LCT_ERR_CODE readPersistance();
   LCT_ERR_CODE persist();

private:
//    const int32_t           m_step              = 1;
//    const int64_t           m_preallocSize      = 100;
//    const int64_t           m_max               = std::numeric_limits<int64_t>::max();
//    int64_t                 m_preallocValve     = 0;
//    std::atomic<int64_t>    m_currentId         = {0};
//    std::string             m_dayStamp;
   CEngineInfo             m_engineInfo;

   typedef std::mutex      CEngineMutex;
   CEngineMutex            m_mutex;
};

typedef std::shared_ptr<CSidDaySwitchEngine> CSidDaySwitchEngineShp;


#endif /* SRC_SERVICE_LCT_ID_SERVICE_ID_DAY_SWITCH_ENGINE_H_ */
