#include "interface.h"
#include "implementation.h"
#include "queue.h"
#include <iostream>

int main()
{
    Store *store = createStore();
    MyActionHandler *handler = new MyActionHandler();
    store->setActionHandler(handler);
    store->init(5, 0, 0);

    Client *clients = new Client[4];

    clients[0].arriveMinute = 0;
    clients[0].maxWaitTime = 10;
    clients[0].banana = 10;
    clients[0].schweppes = 0;

    clients[1].arriveMinute = 45;
    clients[1].maxWaitTime = 30;
    clients[1].banana = 35;
    clients[1].schweppes = 0;

    clients[2].arriveMinute = 46;
    clients[2].maxWaitTime = 100;
    clients[2].banana = 30;
    clients[2].schweppes = 20;

    clients[3].arriveMinute = 200;
    clients[3].maxWaitTime = 1;
    clients[3].banana = 10;
    clients[3].schweppes = 10;

     store->addClients(clients, 4);
    // Client test = Client{0, 1, 1, 1};
    // store->addClients(&test, 1);
    // store->advanceTo(1);

    //Client test1 = Client{0, 0, 10, 0};
   // Client test2 = Client{1, 20, 0, 5};
  //  Client cli[2] = {test1, test2};
   // store->addClients(cli, 2);

    //store->advanceTo(3);
    //store->advanceTo(0);

    /*for(int i = 0; i < 202; i++)
    {
        if(i == 10)
        {
        //  std::cout << std::endl;
        }
        store->advanceTo(i);
    }*/
     store->advanceTo(2002);

    delete store;

    //delete[] clients;
    delete handler;

    return 0;
}