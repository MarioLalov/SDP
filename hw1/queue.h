#pragma once

#include <vector>
#include "interface.h"

struct Client_result
{
    int id;
    Client &client;
    bool in_hurry;

    //Client_result& operator=(const Client_result& other);
};

class Queue
{
private:
    static unsigned int count;

    std::vector<int> ids;
    std::size_t clients_served = 0;
    Client *client_list = nullptr;
    std::vector<Client> clients;

public:
    void initialize(const Client *in_client_list, std::size_t size);
    void clientArrive(unsigned int minute);
    const Client_result current(unsigned int minute);
    void removeClient(unsigned int id);

    unsigned int getRequiredBananas();
    unsigned int getRequiredSchweppes();
    ~Queue();
};
