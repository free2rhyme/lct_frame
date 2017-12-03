/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_string_util.h
 * @version     1.0
 * @date        Jun 9, 2017 9:12:02 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_STRING_UTIL_H_
#define SRC_COMMON_LCT_STRING_UTIL_H_

#include <string>
#include <vector>

std::vector<std::string> Split(const std::string& str, const char* delim = " ");

std::string Trim(const std::string& str, const std::string& delc = " \t\r\n");

std::string Replace(const std::string& str, const char pattern, const char replacer);

#endif /* SRC_COMMON_LCT_STRING_UTIL_H_ */
