/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_string_util.cpp
 * @version      1.0
 * @date         Jun 9, 2017 9:12:42 AM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_COMMON_CLCTSTRINGUTIL_CPP_
#define SRC_COMMON_CLCTSTRINGUTIL_CPP_

#include "lct_string_util.h"

std::vector<std::string> Split(const std::string& str, const char* delim)
{
    std::vector<std::string> parsed;
    std::string::size_type pos = 0;
    while (true){
        const std::string::size_type colon = str.find(delim, pos);
        if (colon == std::string::npos){
            parsed.push_back(str.substr(pos));
            break;
        } else {
            parsed.push_back(str.substr(pos, colon - pos));
            pos = colon + 1;
        }
    }
    return std::move(parsed);
}

std::string Trim(const std::string& str, const std::string& delc)
{
    const size_t pos = str.find_first_not_of(delc);
    if (std::string::npos == pos){
        return "";
    }

    return str.substr(pos, str.find_last_not_of(delc) - pos + 1);
}


std::string Replace(const std::string& str, const char pattern, const char replacer)
{
    std::string result(str);
    size_t pos = str.find_first_of(pattern);
    while(str.npos != pos){
        result.at(pos) = replacer;
        pos = str.find_first_of(pattern, pos + 1);
    }
    return result;
}

#endif /* SRC_COMMON_CLCTSTRINGUTIL_CPP_ */
