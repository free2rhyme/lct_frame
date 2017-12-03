/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_id_generator.h
 * @version     1.0
 * @date        May 23, 2017 4:03:58 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_ID_GENERATOR_H_
#define SRC_COMMON_LCT_ID_GENERATOR_H_

#include <map>
#include <mutex>

#include "lct_common_define.h"
#include "lct_singleton.h"

class CLctIdGenerator final:public CLctSingleton<CLctIdGenerator>
{
public:
   LCT_ERR_CODE init();
   // LCT_ERR_CODE getId(const CLctBizType, LctIdType&);

   LctIdType getId(const CLctBizType&);

private:
   friend class CLctSingleton;

   CLctIdGenerator();
   ~CLctIdGenerator();

   LCT_ERR_CODE getIdImpl(const CLctBizType&, LctIdType&);

private:
   typedef std::map<CLctBizType, int64_t>   CIdContainer;
   CIdContainer                m_idContainer;

   typedef std::mutex          CIdContainerMutex;
   mutable CIdContainerMutex   m_mutex;
};

#define LCT_ID_GEN CLctIdGenerator::instance()


#endif /* SRC_COMMON_LCT_ID_GENERATOR_H_ */
