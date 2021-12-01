#include "implementation.cpp"
#include "queue.h"
#include <iostream>

int main()
{
 
    MyStore store;
    MyActionHandler* handler = new MyActionHandler();
    store.setActionHandler(handler);
    store.init(5, 0, 0);

    Client* clients = new Client[3];

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

    store.addClients(clients, 4);
    
    for(int i = 0; i < 202; i++)
    {
        if(i == 107)
        {
        //  std::cout << "a";
        }
        store.advanceTo(i);
    }

    delete[] clients;
    delete handler;

    return 0;
}