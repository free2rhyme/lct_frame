/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_file_util.cpp
 * @version     1.0
 * @date        Jun 9, 2017 9:15:08 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_file_util.h"

#include <sys/stat.h>

size_t FileSize(FILE *f)
{
   if (f == nullptr) {
      return 0;
   }
   int fd = fileno(f);
   struct stat64 st;
   if (fstat64(fd, &st) == 0) {
      return static_cast<size_t>(st.st_size);
   } else {
      return 0;
   }
}

bool FileExists(const std::string& fileName)
{
   struct stat buffer;
   return (stat(fileName.c_str(), &buffer) == 0);
}
