#include "interface.h"
#include "implementation.h"
#include "queue.h"
#include <iostream>

int main()
{
    Store *store = createStore();
    MyActionHandler *handler = new MyActionHandler();
    store->setActionHandler(handler);

    int input;
    std::cout << "Number of workers:";
    std::cin >> input;
    store->init(input, 0, 0);

    std::cout << "Number of clients:";
    std::cin >> input;

    Client *clients = new Client[input];

    for (std::size_t i = 0; i < input; i++)
    {
        std::cout << "Client " << i + 1 << ":";
        std::cin >> clients[i].arriveMinute >> clients[i].banana >> clients[i].schweppes >> clients[i].maxWaitTime;
    }

    std::cout << std::endl
              << "Output:" << std::endl;

    store->addClients(clients, input);
    store->advanceTo(clients[input - 1].arriveMinute + clients[input - 1].maxWaitTime);

    delete store;
    delete[] clients;
    delete handler;

    return 0;
}