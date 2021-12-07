#pragma once

#include "interface.h"
#include "queue.h"
#include <vector>
#include <iostream>

const unsigned int TIME = 60;
const unsigned int AMOUNT = 100;

// represents workers that are out for delivery
struct Worker
{
	ResourceType resource;
	unsigned int time_of_arrival;

	Worker(ResourceType type, unsigned int time)
		: resource(type), time_of_arrival(time) {}
};

struct MyActionHandler : ActionHandler
{
	void onWorkerSend(int minute, ResourceType resource);
	void onWorkerBack(int minute, ResourceType resource);
	void onClientDepart(int index, int minute, int banana, int schweppes);
};

struct MyStore : Store
{
private:
	ActionHandler *actionHandler = nullptr;
	Queue store_clients;

	unsigned int total_bananas;
	unsigned int total_schweppes;
	unsigned int required_bananas;
	unsigned int required_schweppes;

	std::vector<Worker> workers_on_duty;
	unsigned int workers;

public:
	void setActionHandler(ActionHandler *handler) override;

	void init(int workerCount, int startBanana, int startSchweppes) override;

	// initalize queue with given clients
	void addClients(const Client *clients, int count) override;

	// returns the amount of banans that is ordered
	unsigned int orderedBananas();

	// returns the amount of schweppes that is ordered
	unsigned int orderedSchweppes();

	// advanceTo for individual minute
	void advanceToHelper(int minute);

	// calculates action for ever minute that is <= given minute
	void advanceTo(int minute) override;

	// returns the number of bananas that the client will take
	int calculate_banana(const Client &client);

	// returns the number of schweppes that the client will take
	int calculate_schweppes(const Client &client);

	// get all workers that return in the current minute and update the resources
	void fillStore(unsigned int minute);

	int getBanana() const;

	int getSchweppes() const;
};

Store *createStore();