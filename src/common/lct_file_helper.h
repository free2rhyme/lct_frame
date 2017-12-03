/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_file_helper.h
 * @version     1.0
 * @date        Jun 9, 2017 9:28:24 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_FILE_HELPER_H_
#define SRC_COMMON_LCT_FILE_HELPER_H_

#include "lct_common_define.h"
#include "lct_file_util.h"

class CFileHelper final
{
public:
   explicit CFileHelper(const bool isBinary = true);

   ~CFileHelper();

   LCT_ERR_CODE open(const std::string& fname, bool truncate = false);
   LCT_ERR_CODE reopen(bool truncate);
   LCT_ERR_CODE flush();
   LCT_ERR_CODE close();

   template <typename T>
   LCT_ERR_CODE write(const T& val);

   size_t size() const;

   const std::string& fileName() const;

private:
   template <typename T>
   std::size_t writeTextValue(const T& val);

private:
   DISALLOW_COPY_MOVE_OR_ASSIGN(CFileHelper);

private:
   bool          m_isBinary;
   FILE*         m_fd;
   std::string   m_fileName;
};

#include "detail/lct_file_helper.hpp"

#endif /* SRC_COMMON_LCT_FILE_HELPER_H_ */
