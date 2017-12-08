#ifndef __SIMPLE_CLIENT_H__
#define __SIMPLE_CLIENT_H__

#include "cassandra.h"
#include <string>
#include <iostream>

namespace example
{
   class SimpleClient
   {
   private:
      CassCluster* cluster    = nullptr;
      CassSession* session    = nullptr;
      inline CassError execute(std::string cqlStatement, const CassResult** results = NULL);

   public:
      inline CassSession* getSession()
      {
         return session;
      }

      inline CassError printError(const CassError error)
		{
		   std::cout << cass_error_desc(error) << "\n";
		   return error;
		}

      CassError connect(const std::string nodes);
      CassError createSchema();
      virtual CassError loadData();
      CassError querySchema();
      void close();

      SimpleClient()
      {
      }
      virtual ~SimpleClient()
      {
      }
   };
} // end namespace

#endif
