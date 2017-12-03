/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_engine.h
 * @version     1.0
 * @date        Jun 30, 2017 7:26:39 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_SERVICE_LCT_ID_SERVICE_ID_ENGINE_H_
#define SRC_SERVICE_LCT_ID_SERVICE_ID_ENGINE_H_

#include "id_common_define.h"
#include "lct_error_code.h"

template <typename CIdEngineType>
class CSidEngine
{
public:
    explicit CSidEngine(const ESeedType type, const LCT_SEED_ID_TYPE& seedId):
        m_seedType(type), m_seedId(seedId)
    {
    }

    virtual ~CSidEngine()
    {
    }

    DEFAULT_COPY_MOVE_AND_ASSIGN(CSidEngine);

public:
    LCT_ERR_CODE init()
    {
        return static_cast<CIdEngineType*>(this)->init();
    }

    LCT_SEED_ID_TYPE seedId() const
    {
        return m_seedId;
    }

    ESeedType seedType() const
    {
        return m_seedType;
    }

    template <typename CIdType>
    LCT_ERR_CODE spawnId(CIdType& id)
    {
        return static_cast<CIdEngineType*>(this)->spawnId(id);
    }
private:
    ESeedType             m_seedType;
    LCT_SEED_ID_TYPE      m_seedId;
};

#endif /* SRC_SERVICE_LCT_ID_SERVICE_ID_ENGINE_H_ */
