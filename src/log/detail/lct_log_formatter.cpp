/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_log_formatter.cpp
 * @version     1.0
 * @date        Jun 9, 2017 6:22:41 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include <thread>
#include <iomanip>
#include <unistd.h>

#include "lct_log_formatter.h"
#include "lct_log_stream.h"
#include "lct_logger.h"

static constexpr char FORMAT_DELIMITER_START   = '[';
static constexpr char FORMAT_DELIMITER_END     = ']';
static constexpr char FORMAT_DELIMITER_EMPTY   = ' ';
static constexpr char FORMAT_DELIMITER_COLON   = ':';
static constexpr char FORMAT_DELIMITER_DOT     = '.';

/*******************************************************************************
 *   Date & Time appenders
 * *****************************************************************************/
LCT_ERR_CODE CIVALogFormatterDate::format(CLctLogStream& stream) const
{
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%y-%m-%d", &(stream.timestamp()));
   stream << FORMAT_DELIMITER_START << buf;
   return LCT_SUCCESS;
}

LCT_ERR_CODE CIVALogFormatterTime::format(CLctLogStream& stream) const
{
   char buf[80] = {0};
   strftime(buf, sizeof(buf), "%X", &(stream.timestamp()));
   stream << FORMAT_DELIMITER_EMPTY << buf;
   return LCT_SUCCESS;
}

// milliseconds
LCT_ERR_CODE CIVALogFormatterMillisecond::format(CLctLogStream& stream) const
{
   const auto& millis = std::chrono::duration_cast<std::chrono::milliseconds>(stream.timeDurationSinceEpoch()).count() % 1000;
   stream << FORMAT_DELIMITER_DOT << std::setfill('0') << std::setw(3) << millis << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

// microseconds
LCT_ERR_CODE CIVALogFormatterMicrosecond::format(CLctLogStream& stream) const
{
   const auto& micros = std::chrono::duration_cast<std::chrono::microseconds>(stream.timeDurationSinceEpoch()).count() % 1000000;
   stream << FORMAT_DELIMITER_DOT << std::setfill('0') << std::setw(6) << micros << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

// nanoseconds
LCT_ERR_CODE CIVALogFormatterNanosecond::format(CLctLogStream& stream) const
{
   const auto& ns = std::chrono::duration_cast<std::chrono::nanoseconds>(stream.timeDurationSinceEpoch()).count() % 1000000000;
   stream << FORMAT_DELIMITER_DOT << std::setfill('0') << std::setw(9) << ns << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

/*******************************************************************************
 *   Thread & Process appenders
 * *****************************************************************************/

LCT_ERR_CODE CIVALogFormatterThread::format(CLctLogStream& stream) const
{
   std::thread::id thisThreadId = std::this_thread::get_id();
   stream << FORMAT_DELIMITER_START << std::hex << "0x" << thisThreadId << std::dec << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

LCT_ERR_CODE CIVALogFormatterPid::format(CLctLogStream& stream) const
{
   static pid_t thisPid = ::getpid();
   stream << FORMAT_DELIMITER_START << thisPid << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

/*******************************************************************************
 *   Logger name & Log level appenders
 * *****************************************************************************/

LCT_ERR_CODE CIVALogFormatterLoggerName::format(CLctLogStream& stream) const
{
   stream << FORMAT_DELIMITER_START<< stream.loggerName() << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}

LCT_ERR_CODE CIVALogFormatterLogLevel::format(CLctLogStream& stream) const
{
   stream << FORMAT_DELIMITER_START << LCT_LOG_MGR->logLevelString(stream.logLevel()) << FORMAT_DELIMITER_END;
   return LCT_SUCCESS;
}


LCT_ERR_CODE CIVALogFormatterShortLogLevel::format(CLctLogStream& stream) const
{
   return LCT_SUCCESS;
}

/*******************************************************************************
 *   Location appenders
 * *****************************************************************************/
LCT_ERR_CODE CIVALogFormatterLocation::format(CLctLogStream& stream) const
{
   stream << FORMAT_DELIMITER_START
         << stream.fileName()
         << FORMAT_DELIMITER_COLON
         << stream.lineNumber()
         << FORMAT_DELIMITER_COLON
         << stream.functionName()
         << FORMAT_DELIMITER_END;

   return LCT_SUCCESS;
}



