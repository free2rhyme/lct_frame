/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_formatter.h
 * @version     1.0
 * @date        Jun 8, 2017 6:56:49 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_LOG_LCT_LOG_FORMATTER_H_
#define SRC_LOG_LCT_LOG_FORMATTER_H_

#include "lct_common_define.h"

template <typename CFormatType, typename CLogStream>
class CLctLogFormatter
{
public:
   CLctLogFormatter()
   {
   }

   virtual ~CLctLogFormatter()
   {
   }

   LCT_ERR_CODE format(CLogStream& stream) const
   {
      return static_cast<CFormatType*>(this)->Format(stream);
   }

};

class CLctLogStream;

/*******************************************************************************
 *   Date & Time appenders
 * *****************************************************************************/
class CIVALogFormatterDate final:public CLctLogFormatter<CIVALogFormatterDate, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

class CIVALogFormatterTime final:public CLctLogFormatter<CIVALogFormatterTime, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

// milliseconds
class CIVALogFormatterMillisecond final:public CLctLogFormatter<CIVALogFormatterMillisecond, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

// microseconds
class CIVALogFormatterMicrosecond final:public CLctLogFormatter<CIVALogFormatterMicrosecond, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

// nanoseconds
class CIVALogFormatterNanosecond final:public CLctLogFormatter<CIVALogFormatterNanosecond, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

/*******************************************************************************
 *   Thread & Process appenders
 * *****************************************************************************/
class CIVALogFormatterThread final:public CLctLogFormatter<CIVALogFormatterThread, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

class CIVALogFormatterPid final:public CLctLogFormatter<CIVALogFormatterPid, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};


/*******************************************************************************
 *   Logger Name & Log lever appenders
 * *****************************************************************************/
class CIVALogFormatterLoggerName final:public CLctLogFormatter<CIVALogFormatterLoggerName, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

class CIVALogFormatterLogLevel final:public CLctLogFormatter<CIVALogFormatterLogLevel, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};

class CIVALogFormatterShortLogLevel final:public CLctLogFormatter<CIVALogFormatterShortLogLevel, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};


/*******************************************************************************
 *   Location appenders
 * *****************************************************************************/
class CIVALogFormatterLocation final:public CLctLogFormatter<CIVALogFormatterLocation, CLctLogStream>
{
public:
   LCT_ERR_CODE format(CLctLogStream& stream) const;
};


#endif /* SRC_LOG_LCT_LOG_FORMATTER_H_ */
