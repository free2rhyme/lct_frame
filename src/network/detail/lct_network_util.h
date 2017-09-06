/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_network_util.h
 * @version     1.0
 * @date        Aug 18, 2017 12:05:24 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NETWORK_UTIL_H_
#define SRC_NETWORK_LCT_NETWORK_UTIL_H_

#include "lct_network_common_define.h"

LCT_ERR_CODE AdapterAddr2Ip(const std::string& addr, std::string& ip, int16_t& port);
std::string  IpPort2Adapter(const std::string& ip, const int16_t port);
std::string  IpAddr2String(const int64_t ip);
int64_t      IpAddr2long(const std::string& ipStr);


#endif /* SRC_NETWORK_LCT_NETWORK_UTIL_H_ */
