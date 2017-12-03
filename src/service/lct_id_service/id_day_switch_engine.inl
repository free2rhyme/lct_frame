/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_day_switch_engine.inl
 * @version     1.0
 * @date        Oct 17, 2017 10:24:42 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_SERVICE_LCT_ID_SERVICE_ID_DAY_SWITCH_ENGINE_INL_
#define SRC_SERVICE_LCT_ID_SERVICE_ID_DAY_SWITCH_ENGINE_INL_

inline int32_t CSidDaySwitchEngine::step() const
{
    return m_step;
}

inline int64_t CSidDaySwitchEngine::preallocSize() const
{
    return m_preallocSize;
}

inline int64_t CSidDaySwitchEngine::max() const
{
    return m_max;
}

inline int64_t CSidDaySwitchEngine::preallocValve() const
{
    return m_preallocValve;
}

inline int64_t CSidDaySwitchEngine::currentId() const
{
    return m_currentId;
}

inline int64_t CSidDaySwitchEngine::dayStamp() const
{
    return m_dayStamp;
}

#endif /* SRC_SERVICE_LCT_ID_SERVICE_ID_DAY_SWITCH_ENGINE_INL_ */
