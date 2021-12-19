#pragma once

#include <vector>
#include "interface.h"

struct QueueEmpty : public std::exception
{
	const char * what () const throw ()
    {
    	return "No client in queue!";
    }
};

// return type with specific info about the client
struct Client_result
{
    int id;
    Client &client;
    bool in_hurry;
};

class Queue
{
private:
    // keeps track of ids
    unsigned int count;

    // store client ids
    std::vector<int> ids;
    // full client list
    std::vector<Client> client_list;
    // clients waiting in queue
    std::vector<Client> clients;

    // clients whose requests have been ordered
    std::size_t clients_served_banana = 0;
    std::size_t clients_served_schweppes = 0;

public:
    void initialize(const Client *in_client_list, std::size_t size);
    // adds clients that arrive in the current minute
    void clientArrive(unsigned int minute);
    // retruns the current client
    const Client_result current(unsigned int minute);
    // remove client by id
    void removeClient(unsigned int id);

    // get the amount that needs to be ordered
    unsigned int getRequiredBananas();
    unsigned int getRequiredSchweppes();
};
