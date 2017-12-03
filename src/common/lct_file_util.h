/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_file_util.h
 * @version     1.0
 * @date        Jun 9, 2017 9:13:08 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_FILE_UTIL_H_
#define SRC_COMMON_LCT_FILE_UTIL_H_

#include <cstddef>
#include <cstdio>
#include <string>

size_t FileSize(FILE *f);

bool FileExists(const std::string& fileName);


#endif /* SRC_COMMON_LCT_FILE_UTIL_H_ */
