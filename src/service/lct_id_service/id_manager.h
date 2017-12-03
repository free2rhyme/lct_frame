/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_manager.h
 * @version     1.0
 * @date        Jun 30, 2017 7:23:52 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_SERVICE_LCT_ID_SERVICE_ID_MANAGER_H_
#define SRC_SERVICE_LCT_ID_SERVICE_ID_MANAGER_H_

#include "id_common_define.h"
#include "id_day_switch_engine.h"
#include "lct_singleton.h"

template <typename... CIdEngineType>
class CSidManager final: public CLctSingleton<CSidManager<CIdEngineType...>>
{
public:
    LCT_ERR_CODE init();

    template <typename CIdType>
    LCT_ERR_CODE fetchId(const LCT_SEED_ID_TYPE& seedId, CIdType& id);

private:
    friend class CLctSingleton<CSidManager<CIdEngineType...>>;

    CSidManager();
    ~CSidManager();

private:
    template <typename CEngineClass>
    LCT_ERR_CODE appendEngine(const CEngineClass& idEngine);

private:
    template<size_t I = 0, class CIdType, class ...CIdEnginesType>
    auto fetchId(const LCT_SEED_ID_TYPE seedId, CIdType& id, std::tuple<CIdEnginesType...>& engineTuple)->typename std::enable_if<I == sizeof...(CIdEnginesType), LCT_ERR_CODE>::type;

    template<size_t I = 0, class CIdType, class ...CIdEnginesType>
    auto fetchId(const LCT_SEED_ID_TYPE seedId, CIdType& id, std::tuple<CIdEnginesType...>& engineTuple)->typename std::enable_if<I < sizeof...(CIdEnginesType), LCT_ERR_CODE>::type;

private:
    typedef typename std::tuple<std::map<LCT_SEED_ID_TYPE, std::shared_ptr<CIdEngineType>>...> CLctTupleContainer;

    CLctTupleContainer     m_idEngineTuple;
};

#define SID_ID_MGR CSidManager<CSidDaySwitchEngine>::instance()

#include "id_manager.hpp"

#endif /* SRC_SERVICE_LCT_ID_SERVICE_ID_MANAGER_H_ */
