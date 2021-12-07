#include "queue.h"
#include <iostream>

void Queue::initialize(const Client *in_client_list, std::size_t size)
{
    // clear all previous data
    count = 0;
    client_list.clear();
    clients.clear();
    ids.clear();

    for (std::size_t i = 0; i < size; i++)
    {
        client_list.push_back(in_client_list[i]);
    }
}

void Queue::clientArrive(unsigned int minute)
{
    if (client_list.size() > count && client_list[count].arriveMinute == minute)
    {
        // let client in
        clients.push_back(client_list[count]);
        // assign id
        ids.push_back(count);

        // count clients that have arrived
        clients_served_banana++;
        clients_served_schweppes++;

        count++;
    }
}

const Client_result Queue::current(unsigned int minute)
{
    // let clients in if such exist
    clientArrive(minute);

    // throw if there is no client in the queue
    if (clients.size() < 1)
    {
        throw std::invalid_argument("No clients");
    }

    // return current client whether he is in a hurry or not
    for (std::size_t i = 0; i < clients.size(); i++)
    {
        if (minute == clients[i].arriveMinute + clients[i].maxWaitTime)
        {
            // client must leave
            return {ids[i], clients[i], true};
        }
    }

    // client on turn
    return {ids[0], clients[0], false};
}

void Queue::removeClient(unsigned int id)
{
    for (std::size_t i = 0; i < ids.size(); i++)
    {
        if (ids[i] == id)
        {
            ids.erase(ids.begin() + i);
            clients.erase(clients.begin() + i);
        }
    }
}

unsigned int Queue::getRequiredBananas()
{
    unsigned int total = 0;

    for (std::size_t i = clients.size() - 1, j = clients_served_banana; j > 0; i--, j--)
    {
        total += clients[i].banana;
    }

    clients_served_banana = 0;

    return total;
}

unsigned int Queue::getRequiredSchweppes()
{
    unsigned int total = 0;

    for (std::size_t i = clients.size() - 1, j = clients_served_schweppes; j > 0; i--, j--)
    {
        total += clients[i].schweppes;
    }

    clients_served_schweppes = 0;

    return total;
}