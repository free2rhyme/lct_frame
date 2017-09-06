/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_network_util.cpp
 * @version     1.0
 * @date        Aug 18, 2017 12:05:35 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include <vector>
#include <string>

#include "lct_network_util.h"
#include "lct_string_util.h"

static constexpr int32_t LCT_IP_PART1 = 0xff000000;
static constexpr int32_t LCT_IP_PART2 = 0xff0000;
static constexpr int32_t LCT_IP_PART3 = 0xff00;
static constexpr int32_t LCT_IP_PART4 = 0xff;

std::string IpAddr2String(const int64_t ip)
{
    std::string ipStr = std::to_string((ip & LCT_IP_PART1) >> 24);
    ipStr += "." + std::to_string((ip & LCT_IP_PART2) >> 16);
    ipStr += "." + std::to_string((ip & LCT_IP_PART3) >> 8);
    ipStr += "." + std::to_string(ip & LCT_IP_PART4);

    return ipStr;
}

int64_t IpAddr2long(const std::string& ipStr)
{
    std::vector<std::string> values = Split(ipStr, ".");
    if (values.size() != 4) {
        return LCT_INVALID_IP;
    }
    int64_t result = LCT_INVALID_IP;
    try {
        int32_t part = std::stoi(values.at(0));
        int64_t ip = part << 24;
        part = std::stoi(values.at(1));
        ip = ip | (part << 16);
        part = std::stoi(values.at(2));
        ip = ip | (part << 8);
        part = std::stoi(values.at(3));
        ip = ip | (part);

        result = ip;
    } catch (const std::exception& e) {
        return LCT_INVALID_IP;
    } catch (...) {
        return LCT_INVALID_IP;
    }
    return result;
}

LCT_ERR_CODE AdapterAddr2Ip(const std::string& addr, std::string& ip, int16_t& port)
{
    std::vector<std::string> values = Split(addr, ":");
    if (values.size() != 2) {
        return LCT_INVALID_PARAM;
    }
    try {
        port = std::stoi(values.at(1));
        ip = values.at(0);
    } catch (const std::exception& e) {
        return LCT_INVALID_PARAM;
    } catch (...) {
        return LCT_INVALID_PARAM;
    }
    return LCT_SUCCESS;
}

std::string IpPort2Adapter(const std::string& ip, const int16_t port)
{
    return ip + ":" + std::to_string(port);
}
