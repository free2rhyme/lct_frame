/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_properties.h
 * @version     1.0
 * @date        Jun 8, 2017 3:49:13 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_PROPERTIES_H_
#define SRC_COMMON_LCT_PROPERTIES_H_

#include <map>
#include <string>
#include "lct_common_define.h"

class CLctProperties final
{
   static constexpr const char* const DEFAULT_CONFIG_FILE = "./config.properties";

public:
   explicit CLctProperties(const std::string& configFilePath = DEFAULT_CONFIG_FILE);

   ~CLctProperties();

   LCT_ERR_CODE parse();

   std::string getString(const std::string& key, const std::string& defaultValue = "") const;

   bool getBool(const std::string& key, const bool defaultValue = false) const;

   int32_t getInt32(const std::string& key, const int32_t defaultValue = 0) const;

   int64_t getInt64(const std::string& key, const int64_t defaultValue = 0) const;

   float getFloat(const std::string& key, const double defaultValue = 0.0f) const;

   double getDouble(const std::string& key, const double defaultValue = 0.0d) const;

private:
   LCT_ERR_CODE getValue(const std::string& key, std::string& value) const;

   DISALLOW_COPY_MOVE_OR_ASSIGN(CLctProperties);

private:
   const std::string   m_configFilePath;

   typedef std::map<std::string, std::string>   CPropertisMap;
   CPropertisMap       m_properties;
};



#endif /* SRC_COMMON_LCT_PROPERTIES_H_ */
