/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_singleton.h
 * @version      1.0
 * @date         May 23, 2017 4:10:13 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_SINGLETON_H_
#define SRC_COMMON_LCT_SINGLETON_H_

#include <mutex>
#include "lct_common_define.h"

template <typename ClassType>
class CLctSingleton
{
    typedef ClassType*    ClassPointerType;

public:
    static ClassPointerType instance();

    virtual LCT_ERR_CODE initialize()
    {
       return LCT_SUCCESS;
    }

protected:
    CLctSingleton();
    virtual ~CLctSingleton();

    DISALLOW_COPY_MOVE_OR_ASSIGN(CLctSingleton);

    bool isInstanced() const
    {
        return m_instancedFlag;
    }
private:
    static void InitInstance(ClassPointerType&);

private:
    static bool              m_instancedFlag;
    static std::once_flag    m_onceFlag;

};

template <typename ClassType>
bool  CLctSingleton<ClassType>::m_instancedFlag = false;

template <typename ClassType>
std::once_flag    CLctSingleton<ClassType>::m_onceFlag;

template <typename ClassType>
CLctSingleton<ClassType>::CLctSingleton()
{
}

template <typename ClassType>
CLctSingleton<ClassType>::~CLctSingleton()
{
}

template <typename ClassType>
typename CLctSingleton<ClassType>::ClassPointerType CLctSingleton<ClassType>::instance()
{
    static ClassPointerType _instance = nullptr;
    if(!m_instancedFlag){
        std::call_once(m_onceFlag, InitInstance, std::ref(_instance));
    }
    return _instance;
}

template <typename ClassType>
void CLctSingleton<ClassType>::InitInstance(ClassPointerType& objPtr)
{
    objPtr = new ClassType();

    LCT_ERR_CODE errCode = objPtr->initialize();
    if(LCT_SUCCESS != errCode){
       return;
    }

    m_instancedFlag = true;
}

#endif /* SRC_COMMON_LCT_SINGLETON_H_ */
