/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_file_helper.cpp
 * @version     1.0
 * @date        Jun 9, 2017 3:15:06 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include <memory>

#include "lct_file_helper.h"
#include "lct_log.h"
#include <errno.h>

CFileHelper::CFileHelper(const bool isBinary):m_isBinary(isBinary), m_fd(nullptr)
{
}

CFileHelper::~CFileHelper()
{
   close();
}

LCT_ERR_CODE CFileHelper::open(const std::string& fname, bool truncate /* = false */)
{
   close();
   m_fileName = fname;

   std::string mode;
   if (m_isBinary) {
      mode = truncate?("wb"):("ab");
   } else {
      mode = truncate?("w"):("a");
   }

   m_fd = std::fopen(fname.c_str(), mode.c_str());
   if (m_fd == nullptr) {
      return errno;
   }

   return LCT_SUCCESS;
}

LCT_ERR_CODE CFileHelper::reopen(bool truncate)
{
   if (m_fileName.empty()) {
      return LCT_FAIL;
   }
   return open(m_fileName, truncate);
}

LCT_ERR_CODE CFileHelper::flush()
{
   if (!m_fd) {
      return LCT_INVALID_FD;
   }
   if (0 != std::fflush(m_fd)) {
      return LCT_FAIL;
   }
   return LCT_SUCCESS;
}

LCT_ERR_CODE CFileHelper::close()
{
   if (m_fd) {
      flush();
      if (0 != std::fclose(m_fd)) {
         return LCT_FAIL;
      }
      m_fd = nullptr;
   }
   return LCT_SUCCESS;
}

size_t CFileHelper::size() const
{
   if (!m_fd) {
      return 0;
   }
   return FileSize(m_fd);
}

const std::string& CFileHelper::fileName() const
{
   return m_fileName;
}


template <>
LCT_ERR_CODE CFileHelper::write<const char*>(const char* const& msg)
{
   if (msg == nullptr) {
      return LCT_SUCCESS;
   }
   return write(std::string(msg));
}

template <>
LCT_ERR_CODE CFileHelper::write<std::string>(const std::string& msg)
{
   if (!m_fd) {
      return LCT_INVALID_FD;
   }
   const size_t msg_size = msg.size();
   if (msg_size == 0) {
      return LCT_SUCCESS;
   }
   auto data = msg.data();
   try {
      if (std::fwrite(data, 1, msg_size, m_fd) != msg_size){
         return errno;
      }
   } catch (const std::exception& e) {
      return LCT_UNEXPECTED;
   } catch (...) {
      return LCT_UNEXPECTED;
   }
   return LCT_SUCCESS;
}

template <>
LCT_ERR_CODE CFileHelper::write<std::shared_ptr<std::string>>(const std::shared_ptr<std::string>& msg)
{
   if (msg == nullptr) {
      return LCT_INVALID_POINTER;
   }
   return write(*msg);
}

template <>
LCT_ERR_CODE CFileHelper::write<std::shared_ptr<const std::string>>(const std::shared_ptr<const std::string>& msg)
{
   if (msg == nullptr) {
      return LCT_INVALID_POINTER;
   }
   return write(*msg);
}

template <>
std::size_t CFileHelper::writeTextValue<uint32_t>(const uint32_t& val)
{
   return fprintf(m_fd,"%u",val);
}

template <>
std::size_t CFileHelper::writeTextValue<uint64_t>(const uint64_t& val)
{
   return fprintf(m_fd,"%lu",val);
}

template <>
std::size_t CFileHelper::writeTextValue<int32_t>(const int32_t& val)
{
   return fprintf(m_fd,"%d",val);
}

template <>
std::size_t CFileHelper::writeTextValue<int64_t>(const int64_t& val)
{
   return fprintf(m_fd,"%ld",val);
}

template <>
std::size_t CFileHelper::writeTextValue<float>(const float& val)
{
   return fprintf(m_fd,"%f",val);
}

