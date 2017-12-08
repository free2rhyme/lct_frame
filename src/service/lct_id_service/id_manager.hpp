/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_manager.hpp
 * @version     1.0
 * @date        Jun 30, 2017 7:23:59 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
 
#ifndef SRC_SERVICE_LCT_ID_SERVICE_ID_MANAGER_HPP_
#define SRC_SERVICE_LCT_ID_SERVICE_ID_MANAGER_HPP_

template <typename... CIdEngineType>
LCT_ERR_CODE CSidManager<CIdEngineType...>::init()
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;
   {
      CSidDaySwitchEngineShp eventEngine = std::make_shared<CSidDaySwitchEngine>(LCT_EVENT_ENGINE_SEED_ID);
      errCode = eventEngine->init();
      if(LCT_SUCCESS != errCode){
         LOG_ERROR << ErrCodeFormat(errCode);
          return errCode;
      }
      errCode = appendEngine(eventEngine);
      if(LCT_SUCCESS != errCode){
         LOG_ERROR << ErrCodeFormat(errCode);
         return errCode;
      }
   }

   {
      CSidDaySwitchEngineShp taskEngine = std::make_shared<CSidDaySwitchEngine>(LCT_TASKS_ENGINE_SEED_ID);
      errCode = taskEngine->init();
      if(LCT_SUCCESS != errCode){
         LOG_ERROR << ErrCodeFormat(errCode);
         return errCode;
      }
      errCode = appendEngine(taskEngine);
      if(LCT_SUCCESS != errCode){
         LOG_ERROR << ErrCodeFormat(errCode);
         return errCode;
      }
   }

   {
      CSidDaySwitchEngineShp taskEngine(new CSidDaySwitchEngine(LCT_MODEL_ENGINE_SEED_ID));
      errCode = taskEngine->init();
      if(LCT_SUCCESS != errCode){
         LOG_ERROR << ErrCodeFormat(errCode);
         return errCode;
      }
      errCode = appendEngine(taskEngine);
      if(LCT_SUCCESS != errCode){
         LOG_ERROR << ErrCodeFormat(errCode);
         return errCode;
      }
   }

   return  errCode;
}

template <typename... CIdEngineType>
template <typename CIdType>
LCT_ERR_CODE CSidManager<CIdEngineType...>::fetchId(const LCT_SEED_ID_TYPE& seedId, CIdType& id)
{
   return fetchId(seedId, id, m_idEngineTuple);
}

template <typename... CIdEngineType>
template <typename CEngineClass>
LCT_ERR_CODE CSidManager<CIdEngineType...>::appendEngine(const CEngineClass& idEngine)
{
   auto& mmap = cplusplus14::get<std::map<LCT_SEED_ID_TYPE, CEngineClass>>(m_idEngineTuple);
   mmap[idEngine->seedId()] = idEngine;
   return LCT_SUCCESS;
}

template <typename... CIdEngineType>
template <size_t I, class CIdType, class ...CEngineClass>
auto CSidManager<CIdEngineType...>::fetchId(const LCT_SEED_ID_TYPE seedId, CIdType& id, std::tuple<CEngineClass...>& engineTuple)->typename std::enable_if<I == sizeof...(CEngineClass), LCT_ERR_CODE>::type
{
   return LCT_INVALID_BIZ_TYPE;
}

template <typename... CIdEngineType>
template <size_t I, class CIdType, class ...CEngineClass>
auto CSidManager<CIdEngineType...>::fetchId(const LCT_SEED_ID_TYPE seedId, CIdType& id, std::tuple<CEngineClass...>& engineTuple)->typename std::enable_if<I < sizeof...(CEngineClass), LCT_ERR_CODE>::type
{
   auto& mmap = std::get<I>(engineTuple);

   auto itRe = mmap.find(seedId);
   if(itRe != mmap.end()){
      auto& engineRef = itRe->second;
      return engineRef->spawnId(id);
   }

   return fetchId<I+1>(seedId, id, engineTuple);
}

template <typename... CIdEngineType>
CSidManager<CIdEngineType...>::CSidManager()
{
}

template <typename... CIdEngineType>
CSidManager<CIdEngineType...>::~CSidManager()
{
}


#endif /* SRC_SERVICE_LCT_ID_SERVICE_ID_MANAGER_HPP_ */
