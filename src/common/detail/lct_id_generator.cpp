/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_id_generator.cpp
 * @version     1.0
 * @date        May 23, 2017 4:39:25 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#include "lct_id_generator.h"

CLctIdGenerator::CLctIdGenerator()
{
}

CLctIdGenerator::~CLctIdGenerator()
{
}

LCT_ERR_CODE CLctIdGenerator::init()
{
   return LCT_SUCCESS;
}

// LCT_ERR_CODE CLctIdGenerator::GetId(const CLctBizType bizType, LctIdType& id)
// {
//    return GetIdImpl(bizType, id);
// }

LctIdType CLctIdGenerator::getId(const CLctBizType& bizType){
   LctIdType id = -1;
   getIdImpl(bizType, id);
   return id;
}

LCT_ERR_CODE CLctIdGenerator::getIdImpl(const CLctBizType& bizType, LctIdType& id)
{
   std::lock_guard<CIdContainerMutex> lg(m_mutex);
   CIdContainer::iterator it = m_idContainer.find(bizType);
   if (it == m_idContainer.end()) {
      if(bizType == LCT_BIZ_EVENT_TYPE) {
         id = 1000000;
      } else if(bizType == LCT_BIZ_TASKS_TYPE) {
         id = 100000;
      } else if(bizType == LCT_BIZ_MODEL_TYPE) {
         id = 2000000;
      } else {
         id = 10000;
      }
   } else {
      id = 1 + it->second;
   }
   m_idContainer[bizType] = id;
   return LCT_SUCCESS;
}


