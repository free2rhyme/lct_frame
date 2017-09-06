/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_job.h
 * @version     1.0
 * @date        Jun 16, 2017 4:07:03 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_EVENT_CLCTJOB_H_
#define SRC_EVENT_CLCTJOB_H_

#include <memory>
#include <map>
#include <tuple>
#include <atomic>
#include "lct_common_define.h"
#include "lct_event_common_define.h"
#include "lct_rw_lock.h"
#include "iva_log.h"
#include "lct_task.h"

template <typename CJobType, typename... CSubjobType>
class CLctJob:public CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>
{
public:
    void Process(const std::shared_ptr<CLctJob>&);

public:
    LCT_ERR_CODE Execute();

public:
    LCT_ERR_CODE Schedule();
    LCT_ERR_CODE Start();
    LCT_ERR_CODE Finish();
    LCT_ERR_CODE Cancel();
    LCT_ERR_CODE Suspend();
    LCT_ERR_CODE Resume();
    LCT_ERR_CODE Cleanup();

public:
    LctTaskIdType          JobId()           const;
    LctEventIdType         EventId()         const;
    ELctProcessType        ProcessType()     const;

public:
    explicit CLctJob(const LctEventIdType& eventId,
            const LCTProcessPriorityType priority,
            bool repeated);

    virtual ~CLctJob();

    DEFAULT_COPY_MOVE_AND_ASSIGN(CLctJob);

private:
    template<typename...CWhateverEventType>
    friend class CLctEventManager;

    template<typename...CWhateverEventType>
    friend class CLctJob<CJobType, CSubjobType...>;

    friend class CLctTask<CLctJob<CJobType, CSubjobType...>, CSubjobType...>;

//    friend class CLctTaskProcessor;

    friend CJobType;


private:
    typedef CJobType*       CEventPointerType;
};


#include "detail/lct_job.hpp"

#endif /* SRC_EVENT_CLCTJOB_H_ */
