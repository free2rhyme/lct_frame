/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_logger.cpp
 * @version     1.0
 * @date        Jun 9, 2017 10:00:20 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_log_sink.h"
#include "lct_logger.h"
#include "lct_log_config_mgr.h"

/*********************************************************
 *
 * CLctLog Implementation
 *
 *********************************************************/

bool CLctLog::operator ==(CLctLogStream& stream) const
{
   return (LCT_SUCCESS == LCT_LOG_MGR->log(stream));
}

/*********************************************************
 *
 * CLctLogger Implementation
 *
 *********************************************************/
CLctLogger::CLctLogger() :
      m_sinkPool(true)
{
   initLogLevelByDefault();
}

CLctLogger::~CLctLogger()
{
   close();
}

static constexpr int32_t LOG_ROTATE_FILE_DAILY_HOUR = 0;
static constexpr int32_t LOG_ROTATE_FILE_DAILY_MINUTE = 0;

LCT_ERR_CODE CLctLogger::init(const std::string& logLocalDir, const int64_t rotateMaxFileSize,
      const int64_t rotateMaxFileCount)
{
   std::ios::sync_with_stdio(false);

   LCT_ERR_CODE errCode = LCT_SUCCESS;

   errCode = initLogLevel();
   if (LCT_SUCCESS != errCode) {
      return errCode;
   }

   errCode = initFormatter();
   if (LCT_SUCCESS != errCode) {
      return errCode;
   }

   std::string rectifiedPath = logLocalDir;
   auto it = rectifiedPath.rbegin();
   if ((rectifiedPath.rend() != it) && ('/' != *it)) {
       rectifiedPath = rectifiedPath + '/';
   }

   int64_t rtSize  = rotateMaxFileSize  > 0 ? rotateMaxFileSize  : LOG_ROTATE_FILE_MAX_SIZE;
   int64_t rtCount = rotateMaxFileCount > 0 ? rotateMaxFileCount : LOG_ROTATE_FILE_MAX_COUNT;

   errCode = initSinks(rectifiedPath, rtSize, rtCount);
   if (LCT_SUCCESS != errCode) {
      return errCode;
   }

   m_isInitiated = true;

   return errCode;
}

LCT_ERR_CODE CLctLogger::init(const std::string& logLocalDir, const std::string& configFile,
        const int64_t rotateMaxFileSize, const int64_t rotateMaxFileCount)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   errCode = LCT_LOG_CONFIG_MGR->init(configFile);
   if (LCT_SUCCESS != errCode) {
      return errCode;
   }

   return init(logLocalDir, rotateMaxFileSize, rotateMaxFileCount);
}

LCT_ERR_CODE CLctLogger::initSinks(const std::string& logLocalDir, const int64_t rotateMaxFileSize,
      const int64_t rotateMaxFileCount)
{
   if (logLocalDir.empty()) {
      CDailyRotateTerminalSinkShp sink = std::make_shared<CDailyRotateTerminalSink>(); 
      m_sinkPool.appendSink(sink);
   } else {
      CDailyRotateFileSinkShp sink = std::make_shared<CDailyRotateFileSink>(true, logLocalDir, program_invocation_short_name, rotateMaxFileSize,
               rotateMaxFileCount, LOG_ROTATE_FILE_DAILY_HOUR, LOG_ROTATE_FILE_DAILY_MINUTE); 
      m_sinkPool.appendSink(sink);
   }
   //Debug version, not implemented yet
   // CRemoteSinkShp remoteSink(new CRemoteSink("127.0.0.1", 10000));
   // m_sinkPool.appendSink(remoteSink);

   return m_sinkPool.start();
}

LCT_ERR_CODE CLctLogger::initFormatter()
{
   m_formatterPool.appendFormatter(CIVALogFormatterDate());
   m_formatterPool.appendFormatter(CIVALogFormatterTime());
   m_formatterPool.appendFormatter(CIVALogFormatterMicrosecond());
   m_formatterPool.appendFormatter(CIVALogFormatterLogLevel());
   m_formatterPool.appendFormatter(CIVALogFormatterThread());
   m_formatterPool.appendFormatter(CIVALogFormatterLocation());

   return LCT_SUCCESS;
}

LCT_ERR_CODE CLctLogger::initLogLevel()
{
   m_loglevels.erase(m_loglevels.begin(), m_loglevels.end());

   if (LCT_LOG_CONFIG_MGR->traceLevel()) {
      m_loglevels[ELogLevel::TRACE] = std::string("TRACE");
   }

   if(LCT_LOG_CONFIG_MGR->debugLevel()) {
      m_loglevels[ELogLevel::DEBUG] = std::string("DEBUG");
   }

   if(LCT_LOG_CONFIG_MGR->inforLevel()) {
      m_loglevels[ELogLevel::INFOR] = std::string("INFOR");
   }

   if(LCT_LOG_CONFIG_MGR->warngLevel()) {
      m_loglevels[ELogLevel::WARNG] = std::string("WARNG");
   }

   if(LCT_LOG_CONFIG_MGR->errorLevel()) {
      m_loglevels[ELogLevel::ERROR] = std::string("ERROR");
   }

   if(LCT_LOG_CONFIG_MGR->critcLevel()) {
      m_loglevels[ELogLevel::CRITC] = std::string("CRITC");
   }

   return LCT_SUCCESS;
}

LCT_ERR_CODE CLctLogger::initLogLevelByDefault()
{
   m_loglevels[ELogLevel::TRACE] = std::string("TRACE");
   m_loglevels[ELogLevel::DEBUG] = std::string("DEBUG");
   m_loglevels[ELogLevel::INFOR] = std::string("INFOR");
   m_loglevels[ELogLevel::WARNG] = std::string("WARNG");
   m_loglevels[ELogLevel::ERROR] = std::string("ERROR");
   m_loglevels[ELogLevel::CRITC] = std::string("CRITC");
   return LCT_SUCCESS;
}

LCT_ERR_CODE CLctLogger::close()
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   m_sinkPool.stop();

   m_isInitiated = false;

   return errCode;
}

bool CLctLogger::isLogged(const ELogLevel level) const
{
   return ((m_loglevels.find(level) != m_loglevels.end()) || !m_isInitiated);
}

const std::string CLctLogger::logLevelString(const ELogLevel level) const
{
   CLogLevelContainer::const_iterator itRe = m_loglevels.find(level);
   if (itRe != m_loglevels.end()) {
      return itRe->second;
   } else {
      return "";
   }
}

LCT_ERR_CODE CLctLogger::log(CLctLogStream& logStream)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   errCode = logStream.logEnd();
   if (LCT_SUCCESS != errCode) {
      m_defaultStream << errCode << std::endl;
      return errCode;
   }

   errCode = m_formatterPool.format(logStream);
   if (LCT_SUCCESS != errCode) {
      m_defaultStream << errCode << std::endl;
      return errCode;
   }

   if (m_isInitiated) {
      errCode = m_sinkPool.log(logStream.messagePtr());
      if (LCT_SUCCESS != errCode) {
         m_defaultStream << errCode << std::endl;
         return errCode;
      }
   } else {
      m_defaultStream << logStream.message();
   }
   return errCode;
}

