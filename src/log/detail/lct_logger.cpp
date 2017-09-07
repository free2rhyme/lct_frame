/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_logger.cpp
 * @version      1.0
 * @date         Jun 9, 2017 10:00:20 AM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
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

LCT_ERR_CODE CLctLogger::init(const std::string& logLocalDir, 
      const int64_t logLevelVal, 
      const int64_t rotateMaxFileSize,
      const int64_t rotateMaxFileCount)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   errCode = LCT_LOG_CONFIG_MGR->init(logLevelVal);
   if (LCT_SUCCESS != errCode) {
      return errCode;
   }

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

   errCode = initSinks(rectifiedPath, rotateMaxFileSize, rotateMaxFileCount);
   if (LCT_SUCCESS != errCode) {
      return errCode;
   }

   m_isInitiated = true;

   return errCode;
}

LCT_ERR_CODE CLctLogger::initSinks(const std::string& logLocalDir, const int64_t rotateMaxFileSize,
      const int64_t rotateMaxFileCount)
{
//    CRotateFileSinkShp rotateSinkShp( new CRotateFileSink(true, logLocalDir,
//        program_invocation_short_name, LOG_ROTATE_FILE_MAX_SIZE, LOG_ROTATE_FILE_MAX_COUNT));

//    CDailyFileSinkShp dailyFileSink(new CDailyFileSink<CDailyDateFileNameCalculator>(true,
//        logLocalDir, program_invocation_short_name, LOG_ROTATE_FILE_DAILY_HOUR, LOG_ROTATE_FILE_DAILY_MINUTE));

   CDailyRotateFileSinkShp dailyRotatSinkShp(
         new CDailyRotateFileSink(true, logLocalDir, program_invocation_short_name, rotateMaxFileSize,
               rotateMaxFileCount, LOG_ROTATE_FILE_DAILY_HOUR, LOG_ROTATE_FILE_DAILY_MINUTE));

   //Debug version, not implemented yet
   CRemoteSinkShp remoteSink(new CRemoteSink("127.0.0.1", 10000));

//    m_sinkPool.AppendSink(rotateSinkShp);
//    m_sinkPool.AppendSink(dailyFileSink);
   m_sinkPool.appendSink(dailyRotatSinkShp);
   m_sinkPool.appendSink(remoteSink);

   return m_sinkPool.start();
}

LCT_ERR_CODE CLctLogger::initFormatter()
{
   m_formatterPool.appendFormatter(CLCTLogFormatterDate());
   m_formatterPool.appendFormatter(CLCTLogFormatterTime());
   m_formatterPool.appendFormatter(CLCTLogFormatterMicrosecond());
   m_formatterPool.appendFormatter(CLCTLogFormatterLogLevel());
   m_formatterPool.appendFormatter(CLCTLogFormatterThread());
   m_formatterPool.appendFormatter(CLCTLogFormatterLocation());

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

