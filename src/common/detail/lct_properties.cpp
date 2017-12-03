/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_properties.cpp
 * @version     1.0
 * @date        Jun 8, 2017 3:49:27 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_properties.h"

#include <fstream>
#include <sstream>

#include "lct_log.h"
#include "lct_common_util.h"
#include "lct_string_util.h"

CLctProperties::CLctProperties(const std::string& configFilePath):
   m_configFilePath(configFilePath)
{
}

CLctProperties::~CLctProperties()
{
}

LCT_ERR_CODE CLctProperties::parse()
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;
   std::ifstream ifs;
   do {
      try {
         ifs.open(m_configFilePath);

         if (!ifs.good()) {
            errCode = LCT_FAIL;
            break;
         }

         char buf[512] = {0};
         while (ifs.getline(buf, sizeof(buf))) {
            std::istringstream line(Trim(buf));
            std::string key;
            if (std::getline(line, key, '=')) {
               std::string value;
               if (std::getline(line, value)) {
                  m_properties[Trim(key)] = Trim(value);
               }
            }
         }
      } catch (std::exception& e) {
         LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
         errCode = LCT_UNEXPECTED;
      } catch (...) {
         LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
         errCode = LCT_UNEXPECTED;
      }
   }while(0);

   if (ifs.is_open()) {
      ifs.close();
   }

   return errCode;
}


std::string CLctProperties::getString(const std::string& key, const std::string& defaultValue /* = "" */) const
{
   std::string value;
   if (LCT_SUCCESS == getValue(key, value)) {
      return value;
   } else {
      return defaultValue;
   }
}

bool CLctProperties::getBool(const std::string& key, const bool defaultValue /* = false */) const
{
   std::string value;
   if (LCT_SUCCESS == getValue(key, value)) {
      if (value == "TRUE" || value == "true" || value == "True") {
         return true;
      } else if (value == "FALSE" || value == "false" || value == "False") {
         return false;
      }
   }
   return defaultValue;
}

int32_t CLctProperties::getInt32(const std::string& key, const int32_t defaultValue /* = 0 */) const
{
   std::string value;
   if (LCT_SUCCESS == getValue(key, value)) {
      try {
         return std::stoi(value);
      } catch (std::exception& e) {
         LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
         return defaultValue;
      } catch (...) {
         LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
         return defaultValue;
      }
   } else {
      return defaultValue;
   }
}

int64_t CLctProperties::getInt64(const std::string& key, const int64_t defaultValue /* = 0 */) const
{
   std::string value;
   if (LCT_SUCCESS == getValue(key, value)) {
      try {
         return std::stol(value);
      } catch (std::exception& e) {
         LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
         return defaultValue;
      } catch (...) {
         LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
         return defaultValue;
      }
   } else {
      return defaultValue;
   }
}

float CLctProperties::getFloat(const std::string& key, const double defaultValue /* = 0.0f */) const
{
   std::string value;
   if (LCT_SUCCESS == getValue(key, value)) {
      try{
         return std::stof(value);
      } catch (std::exception& e) {
         LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
         return defaultValue;
      } catch (...) {
         LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
         return defaultValue;
      }
   } else {
      return defaultValue;
   }
}

double CLctProperties::getDouble(const std::string& key, const double defaultValue /* = 0.0d */) const
{
   std::string value;
   if (LCT_SUCCESS == getValue(key, value)) {
      try {
         return std::stod(value);
      } catch (std::exception& e) {
         LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED) << " e.what(" << e.what() << ")";
         return defaultValue;
      } catch (...) {
         LOG_ERROR << ErrCodeFormat(LCT_UNEXPECTED);
         return defaultValue;
      }
   } else {
      return defaultValue;
   }
}

LCT_ERR_CODE CLctProperties::getValue(const std::string& key, std::string& value) const{
   CPropertisMap::const_iterator itRe = m_properties.find(key);
   if (itRe != m_properties.end()) {
      value = itRe->second;
      return LCT_SUCCESS;
   } else {
      return LCT_INVALID_PARAM;
   }
}
