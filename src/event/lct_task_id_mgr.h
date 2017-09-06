/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_task_id_mgr.h
 * @version      1.0
 * @date         Jul 1, 2017 10:40:50 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/

#include "lct_common_define.h"
#include "lct_singleton.h"
#include "lct_event_common_define.h"

#ifndef SRC_EVENT_CLCTTASKIDMGR_H_
#define SRC_EVENT_CLCTTASKIDMGR_H_

class CLctTaskIdMgr final:public CLctSingleton<CLctTaskIdMgr>
{
public:
    LCT_ERR_CODE init(const int32_t idSvcPort, const std::string& idSvcIp);
    LctIdType fetchId(const CLctBizType&) const;

private:
    friend class CLctSingleton;

    CLctTaskIdMgr();
    ~CLctTaskIdMgr();

private:
    LCT_ERR_CODE fetchId(const CLctBizType& bizType, LctIdType& id) const;

private:
    bool         m_isInitiated   = 0;
    int32_t      m_idSvcPort     = 0;
    std::string  m_idSvcIp;
};

#define LCT_TASK_ID_MGR CLctTaskIdMgr::instance()

#endif /* SRC_EVENT_CLCTTASKIDMGR_H_ */
