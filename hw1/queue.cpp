#include "queue.h"
#include <iostream>

//Client_result& Client_result::operator=(const Client& other)

unsigned int Queue::count = 0;

void Queue::initialize(const Client *in_client_list, std::size_t size)
{
    //TODO error for already initalized
    client_list = new Client[size];

    for (std::size_t i = 0; i < size; i++)
    {
        client_list[i] = in_client_list[i];
    }
}

void Queue::clientArrive(unsigned int minute)
{
    if (count < 4 && client_list[count].arriveMinute == minute)
    {
        //let client in
        clients.push_back(client_list[count]);
        ids.push_back(count);

        count++;
    }
}

const Client_result Queue::current(unsigned int minute)
{
    //let clients in if such exist
    clientArrive(minute);

    if(clients.size() < 1)
    {
        throw std::invalid_argument("No clients");
    }

    for (std::size_t i = 0; i < clients.size(); i++)
    {
        if (minute == clients[i].arriveMinute + clients[i].maxWaitTime)
        {
            return {ids[i], clients[i], true};
        }
    }

    return {ids[0], clients[0], false};
}

void Queue::removeClient(unsigned int id)
{
    for(std::size_t i = 0; i < ids.size(); i++)
    {
        if(ids[i] == id)
        {
            ids.erase(ids.begin() + i);
            clients.erase(clients.begin() + i);
        }
    }
}

unsigned int Queue::getRequiredBananas()
{
    unsigned int total = 0;

    for (std::size_t i = clients_served; i < clients.size(); i++)
    {
        total += clients[i].banana;
    }

    clients_served = clients.size();

    return total;
}

unsigned int Queue::getRequiredSchweppes()
{
    unsigned int total = 0;

    for (std::size_t i = 0; i < clients.size(); i++)
    {
        total += clients[i].schweppes;
    }

    return total;
}

Queue::~Queue()
{
    if (client_list)
    {
        delete[] client_list;
    }
}