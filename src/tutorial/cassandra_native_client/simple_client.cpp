#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "simple_client.h"

namespace example {
   using namespace std;
   const std::string TAGS_COLUMN("tags");
   const std::string TITLE_COLUMN("title");
   const std::string ARTIST_COLUMN("artist");
   const std::string ALBUM_COLUMN("album");

   // auxiliary functions
   inline CassError printError(CassError error)
   {
      cout << cass_error_desc(error) << "\n";
      return error;
   }

   inline CassError SimpleClient::execute(string cqlStatement,
      const CassResult** results /* = NULL */)
   {
      CassError rc = CASS_OK;
      CassFuture* result_future = NULL;

      cout << "Executing " << cqlStatement << "\n";

      CassStatement* statement = cass_statement_new(cqlStatement.c_str(), 0);
      result_future = cass_session_execute(session, statement);
      cass_future_wait(result_future);
      rc = cass_future_error_code(result_future);
      if (rc == CASS_OK) {
         cout << "Statement " << cqlStatement << " executed successully." << "\n";
         if ( results != NULL ) {
            *results = cass_future_get_result(result_future);
         }
      } else {
         return printError(rc);
      }
      cass_statement_free(statement);
      cass_future_free(result_future);
      return rc;
   }

   CassError SimpleClient::connect(const string nodes)
   {
      CassError rc = CASS_OK;
      cout << "Connecting to " << nodes << "\n";

      cluster = cass_cluster_new();
      session = cass_session_new();
      CassFuture* connect_future = NULL;
      cass_cluster_set_contact_points(cluster, nodes.c_str());
      connect_future = cass_session_connect(session, cluster);
      cass_future_wait(connect_future);
      rc = cass_future_error_code(connect_future);
      if ( rc == CASS_OK ) {
         cout << "Connected." << "\n";
      } else {
         return printError(rc);
      }
      cass_future_free(connect_future);
      return rc;
   }

   CassError SimpleClient::createSchema()
   {
      CassError rc = CASS_OK;
      cout << "Creating simplex keyspace." << endl;

      rc = execute(string("CREATE KEYSPACE IF NOT EXISTS simplex ") +
         "WITH replication = {'class':'SimpleStrategy', 'replication_factor':1};");

      rc = execute(
         string("CREATE TABLE simplex.songs (") +
            "id uuid PRIMARY KEY," +
            "title text," +
            "album text," +
            "artist text," +
            "tags set<text>," +
            "data blob" +
         ");");

      rc = execute(
         string("CREATE TABLE simplex.playlists (") + "id uuid," + "title text," +
            "album text, " +
            "artist text," +
            "song_id uuid," +
            "PRIMARY KEY (id, title, album, artist)" +
         ");");
      return rc;
   }

   CassError SimpleClient::loadData()
   {
      CassError rc = CASS_OK;
      cout << "Loading data into simplex keyspace." << endl;
      rc = execute(
         string("INSERT INTO simplex.songs (id, title, album, artist, tags)") +
            "VALUES (" +
               "756716f7-2e54-4715-9f00-91dcbea6cf50," +
               "'La Petite Tonkinoise'," +
               "'Bye Bye Blackbird'," +
               "'Joséphine Baker'," +
               "{'jazz', '2013'})" +
         ";");

      rc = execute(
         string("INSERT INTO simplex.songs (id, title, album, artist, tags)") +
            "VALUES (" + "f6071e72-48ec-4fcb-bf3e-379c8a696488," +
               "'Die Mösch'," +
               "'In Gold'," +
               "'Willi Ostermann'," +
               "{'kölsch', '1996', 'birds'}" +
         ");");

      rc = execute(
         string("INSERT INTO simplex.songs (id, title, album, artist, tags)") +
            "VALUES (" + "fbdf82ed-0063-4796-9c7c-a3d4f47b4b25," +
               "'Memo From Turner'," +
               "'Performance'," +
               "'Mick Jager'," +
               "{'soundtrack', '1991'}" +
         ");");

      rc = execute(
        string("INSERT INTO simplex.playlists (id, song_id, title, album, artist) ") +
            "VALUES (" +
               "2cc9ccb7-6221-4ccb-8387-f22b6a1b354d," +
               "756716f7-2e54-4715-9f00-91dcbea6cf50," +
               "'La Petite Tonkinoise'," +
               "'Bye Bye Blackbird'," +
               "'Joséphine Baker'" +
         ");");

     rc = execute(
        string("INSERT INTO simplex.playlists (id, song_id, title, album, artist) ") +
            "VALUES (" +
               "2cc9ccb7-6221-4ccb-8387-f22b6a1b354d," +
               "f6071e72-48ec-4fcb-bf3e-379c8a696488," +
               "'Die Mösch'," + "'In Gold'," +
               "'Willi Ostermann'" +
         ");");

      rc = execute(
         string("INSERT INTO simplex.playlists (id, song_id, title, album, artist) ") +
            "VALUES (" +
               "3fd2bedf-a8c8-455a-a462-0cd3a4353c54," +
               "fbdf82ed-0063-4796-9c7c-a3d4f47b4b25," +
               "'Memo From Turner'," +
               "'Performance'," +
               "'Mick Jager'" +
         ");");

      return rc;
   }

   CassError SimpleClient::querySchema()
   {
      CassError rc = CASS_OK;
      const CassResult* results = nullptr;
      cout << "Querying the simplex.playlists table." << endl;

      rc = execute(
         string("SELECT title, artist, album FROM simplex.playlists ") +
            "WHERE id = 2cc9ccb7-6221-4ccb-8387-f22b6a1b354d;",
         &results);

      const auto deleter = [](CassIterator* rows) {
         std::cout << "Going to release rows" << std::endl;
         cass_iterator_free(rows);
      };

      std::unique_ptr<CassIterator, decltype(deleter)> rows(cass_iterator_from_result(results), deleter);

      if ( rc == CASS_OK ) {
         std::cout << "\n" << "Iterating select-results from querySchema......" << "\n";
         while ( cass_iterator_next(rows.get())) {
            const CassRow* row = cass_iterator_get_row(rows.get());
            const char* title;
            const char* artist;
            const char* album;
            size_t key_length;
            cass_value_get_string(cass_row_get_column(row, 0), &title, &key_length);
            cass_value_get_string(cass_row_get_column(row, 1), &artist, &key_length);
            cass_value_get_string(cass_row_get_column(row, 2), &album, &key_length);
            cout << "title: " << title << ", artist: " << artist << ", album: " << album << "\n";
         }
         std::cout << "\n";
         cass_result_free(results);
         //cass_iterator_free(rows);
      }
      return rc;
   }

   void SimpleClient::close()
   {
      cout << "\nClosing down cluster connection." << "\n";
      std::this_thread::sleep_for(std::chrono::seconds(30));

      cass_session_close(session);

      cass_cluster_free(cluster);
   }

} // end namespace
