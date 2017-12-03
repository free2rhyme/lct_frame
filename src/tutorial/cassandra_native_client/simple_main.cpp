#include <iostream>
#include "simple_client.h"


int main(int argc, char**)
{
    using example::SimpleClient;
    SimpleClient client;
    client.connect("172.20.12.202");
    //client.connect("127.0.0.1");

    CassError err = client.createSchema();
    client.printError(err);

    err = client.querySchema();
    client.printError(err);

    //client.loadData();

    err = client.querySchema();
    client.printError(err);

    client.close();
    
    return 0;
}