/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_cassandra_helper.hpp
 * @version     1.0
 * @date        Sep 26, 2017 11:22:10 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#ifndef SRC_WRAPPER_CASSANDRA_LCT_CASSANDRA_HELPER_HPP_
#define SRC_WRAPPER_CASSANDRA_LCT_CASSANDRA_HELPER_HPP_

#include "lct_log.h"
#include "lct_cass_conn_pool.h"
#include "lct_cass_access_restrain.h"

template <typename Func>
LCT_ERR_CODE CLctCassandraHelper::query(
      const std::string& keyspace,
      const std::string& cqlStatement,
      Func fn)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   LctCassResultType cassResult(nullptr);

   const CassResult* cassResultPtr = cassResult.get();
   errCode = execute(keyspace, cqlStatement, &cassResultPtr);
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to query statement(" << cqlStatement << ")";
      return errCode;
   }

   LctCassIteratorType rows(cass_iterator_from_result(cassResultPtr));
   while (cass_iterator_next(rows.get())) {
      const CassRow* row = cass_iterator_get_row(rows.get());
      fn(row);
   }

   return errCode;
}

template <typename Func>
LCT_ERR_CODE CLctCassandraHelper::pageQuery(
      const std::string& keyspace,
      const std::string& cqlStatement,
      const bool isFirst,
      const int32_t pageSize,
      Func fn)
{
   if (isFirst) {
      return queryFirstPage(keyspace, cqlStatement, pageSize, fn);
   } else {
      return queryRestPage(keyspace, cqlStatement, pageSize, fn);
   }
}

template <typename Func>
LCT_ERR_CODE CLctCassandraHelper::multiQuery(
      const std::string& keyspace,
      const std::vector<std::string>& cqlStatements,
      Func fn)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   LctCassFutureShpContainerType futureShpContainer;

   errCode = execute(keyspace, cqlStatements, futureShpContainer);
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to perform multi query";
      return errCode;
   }

   while (!futureShpContainer.empty()) {
      auto it = futureShpContainer.begin();
      bool isAnyReady = false;
      while (it != futureShpContainer.end()) {
         LctCassFutureShpType& futRef = *it;
         if (cass_future_wait_timed(futRef.get(), cass_duration_t(0))) {
            isAnyReady = true;
            CassError csErr = cass_future_error_code(futRef.get());
            if (CASS_OK != csErr) {
               LOG_ERROR << "Failed to query error(" << cassErrDesc(csErr) << ")";
            } else {
               LctCassResultType cassResult(cass_future_get_result(futRef.get()));
               LctCassIteratorType rows(cass_iterator_from_result(cassResult.get()));
               while (cass_iterator_next(rows.get())) {
                  const CassRow* row = cass_iterator_get_row(rows.get());
                  fn(row);
               }
            }
            it = futureShpContainer.erase(it);
         } else {
            ++it;
         }

         if (!isAnyReady) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
         }
      }
   }
   return errCode;
}

template <typename Func>
LCT_ERR_CODE CLctCassandraHelper::queryFirstPage(
      const std::string& keyspace,
      const std::string& cqlStatement,
      const int32_t pageSize,
      Func fn)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   if (!m_ready.load()) {
      LOG_ERROR << "CassandraHelper is not ready yet";
      return LCT_NOT_INITIATED_YET;
   }

   LOG_DEBUG << "CassandraHelper is executing:" << cqlStatement;

   LctCassStatementShpType statement(cass_statement_new(cqlStatement.c_str(), 0), LctCassStatementRelease);

   CassError csErr = cass_statement_set_paging_size(statement.get(), pageSize);
   if (CASS_OK != csErr) {
      LOG_ERROR << "Failed to set page size for statement(" << cqlStatement << ") error(" << cassErrDesc(csErr) << ")";
      return errCode;
   }

   CassAutoFlowControl();

   LctCassSessionType cassSession;
   errCode = m_connPool->session(keyspace, cassSession);
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to get a session for keyspace(" << keyspace << ")";
      return errCode;
   }

   LctCassFutureType resultFuture(cass_session_execute(cassSession.get(), statement.get()));
   cass_future_wait(resultFuture.get());

   csErr = cass_future_error_code(resultFuture.get());
   if (CASS_OK != csErr) {
      LOG_ERROR << "Failed to execute statement(" << cqlStatement << ") error(" << cassErrDesc(csErr) << ")";
      return LCT_FAIL;
   }

   LctCassResultShpType cassResult(cass_future_get_result(resultFuture.get()), LctCassResultRelease);

   LctCassIteratorType rows(cass_iterator_from_result(cassResult.get()));
   while (cass_iterator_next(rows.get())) {
      const CassRow* row = cass_iterator_get_row(rows.get());
      errCode = fn(row);
      if (LCT_SUCCESS != errCode) {
         LOG_ERROR << "Failed to handle row result for cql(" << statement << ")";
         return errCode;
      }
   }

   cass_bool_t hasMorePages = cass_result_has_more_pages(cassResult.get());
   if (cass_false != hasMorePages) {
      errCode = m_pageStateVessel.save(cqlStatement, LctCassPageInfo(statement, cassResult));
      if (LCT_SUCCESS != errCode) {
         LOG_ERROR << "Failed to save cql(" << statement << ") into paging state vessel";
         return errCode;
      }
   }

   return errCode;
}


template <typename Func>
LCT_ERR_CODE CLctCassandraHelper::queryRestPage(
      const std::string& keyspace,
      const std::string& cqlStatement,
      const int32_t pageSize,
      Func fn)
{
   LCT_ERR_CODE errCode = LCT_SUCCESS;

   if (!m_ready.load()) {
      LOG_ERROR << "CassandraHelper is not ready yet";
      return LCT_NOT_INITIATED_YET;
   }

   LOG_DEBUG << "CassandraHelper is executing:" << cqlStatement;

   LctCassPageInfo pageInfo;
   errCode = m_pageStateVessel.retrieve(cqlStatement, pageInfo);
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to get page info for cql-statement(" << cqlStatement << ")";
      return errCode;
   }
   LctCassStatementShpType statement  = pageInfo.CassStatement;
   LctCassResultShpType   cassResult = pageInfo.CassResult;

   CassError csErr = cass_statement_set_paging_state(statement.get(), cassResult.get());
   if (CASS_OK != csErr) {
      LOG_ERROR << "Failed to set paging state for statement(" << cqlStatement << ") error(" << cassErrDesc(csErr) << ")";
      return LCT_FAIL;
   }

   CassAutoFlowControl();

   LctCassSessionType cassSession;
   errCode = m_connPool->session(keyspace, cassSession);
   if (LCT_SUCCESS != errCode) {
      LOG_ERROR << "Failed to get a session for keyspace(" << keyspace << ")";
      return errCode;
   }

   LctCassFutureType resultFuture(cass_session_execute(cassSession.get(), statement.get()));
   cass_future_wait(resultFuture.get());

   csErr = cass_future_error_code(resultFuture.get());
   if (CASS_OK != csErr) {
      LOG_ERROR << "Failed to execute statement(" << cqlStatement << ") error(" << cassErrDesc(csErr) << ")";
      return LCT_FAIL;
   }

   cassResult.reset(cass_future_get_result(resultFuture.get()), LctCassResultRelease);

   LctCassIteratorType rows(cass_iterator_from_result(cassResult.get()));
   while (cass_iterator_next(rows.get())) {
      const CassRow* row = cass_iterator_get_row(rows.get());
      errCode = fn(row);
      if (LCT_SUCCESS != errCode) {
         LOG_ERROR << "Failed to handle row result for cql(" << statement << ")";
         return errCode;
      }
   }

   cass_bool_t hasMorePages = cass_result_has_more_pages(cassResult.get());
   if (cass_false != hasMorePages) {
      errCode = m_pageStateVessel.save(cqlStatement, LctCassPageInfo(statement, cassResult));
      if (LCT_SUCCESS != errCode) {
         LOG_ERROR << "Failed to save cql(" << statement << ") into paging state vessel";
         return errCode;
      }
   } else {
      errCode = m_pageStateVessel.remove(cqlStatement);
      if (LCT_SUCCESS != errCode) {
         LOG_ERROR << "Failed to remove cql(" << statement << ") from paging state vessel";
         return errCode;
      }
   }

   return errCode;
}

#endif /* SRC_WRAPPER_CASSANDRA_LCT_CASSANDRA_HELPER_HPP_ */

