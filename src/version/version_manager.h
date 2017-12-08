/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        version_manager.h
 * @version     1.0
 * @date        Oct 21, 2017 8:35:08 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_VERSION_VERSION_MANAGER_H_
#define SRC_VERSION_VERSION_MANAGER_H_

#include <string>
#include <vector>

class CVersionParser final
{
   struct CVersionInfo
   {
      CVersionInfo()
      {
      }

      CVersionInfo(const std::string& k, const int16_t v):
          Value(v),
          Key(k)
      {
      }
      int16_t      Value  = 0;
      std::string  Key;
   };

   static constexpr const char* const DEFAULT_VERSION_FILE = "./common/version.h";

public:
   bool init(const std::string& versionFile = DEFAULT_VERSION_FILE);
   bool parse();

   bool autoVersion();
   bool forceVersion(const std::string& vs);

   bool version() const;

   static CVersionParser* instance();
private:
   bool persist() const;


   CVersionParser();
   ~CVersionParser();

private:
   typedef std::vector<CVersionInfo> CVersionMapType;

   std::string     m_versionFile;
   CVersionMapType m_versionMap;
};

#define VERSION_MGR CVersionParser::instance()

#endif /* SRC_VERSION_VERSION_MANAGER_H_ */
