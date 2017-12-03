/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        id_service_mgr.h
 * @version     1.0
 * @date        Jun 30, 2017 7:22:50 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_SERVICE_IVAIDSERVICE_CSIDSERVICEMANAGER_H_
#define SRC_SERVICE_IVAIDSERVICE_CSIDSERVICEMANAGER_H_

#include "id_common_define.h"
#include "id_service.h"
#include "lct_singleton.h"

class CSidServiceMgr final:public CLctSingleton<CSidServiceMgr>
{
public:
    LCT_ERR_CODE init();
    LCT_ERR_CODE run();
    LCT_ERR_CODE shutdown();

private:
    friend class CLctSingleton;
    CSidServiceMgr();
    ~CSidServiceMgr();

private:
    CSidServiceShp    m_serviceShp;
};

#define LCT_SID_MGR CSidServiceMgr::instance()


#endif /* SRC_SERVICE_IVAIDSERVICE_CSIDSERVICEMANAGER_H_ */
