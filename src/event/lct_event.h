/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_event.h
 * @version      1.0
 * @date         May 23, 2017 7:46:29 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_EVENT_CLCTEVENT_H_
#define SRC_EVENT_CLCTEVENT_H_

#include "lct_task.h"

template <typename CEventType, typename... CJobType>
class CLctEvent: public CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>
{
private:
    void process(const std::shared_ptr<CLctEvent>&);

protected:
    LCT_ERR_CODE execute();
    LCT_ERR_CODE persistence() const;

public:
    std::string eventComment() const;
    void eventComment(const std::string& comment);

private:
    LCT_ERR_CODE schedule();
    LCT_ERR_CODE start();
    LCT_ERR_CODE finish();
    LCT_ERR_CODE cancel();
    LCT_ERR_CODE suspend();
    LCT_ERR_CODE resume();
    LCT_ERR_CODE cleanup();
    LCT_ERR_CODE persist();

private:
    ELctProcessType processType() const;

public:
    explicit CLctEvent(const LCTProcessPriorityType priority);
    explicit CLctEvent(const LCTProcessPriorityType priority, const std::string& comment);
    explicit CLctEvent(const LctEventIdType& eventId, const LCTProcessPriorityType priority, const std::string& comment,
        const int64_t& creationTime, const int64_t& beginexecTime, const int64_t& finishExecTime);
    virtual ~CLctEvent();

    DEFAULT_COPY_MOVE_AND_ASSIGN(CLctEvent);

private:
    typedef CEventType*  CEventPointerType;

private:
    template<typename...CWhateverEventType>
    friend class CLctEventManager;

    template<typename...CWhateverEventType>
    friend class CLctEvent<CEventType, CJobType...>;

    friend class CLctTask<CLctEvent<CEventType, CJobType...>, CJobType...>;

    friend class CLctTaskProcessor;

    std::string      m_comment;
};

#include "detail/CLctEvent.hpp"

#endif /* SRC_EVENT_CLCTEVENT_H_ */
