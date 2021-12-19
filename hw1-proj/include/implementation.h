#pragma once

#include "interface.h"
#include "queue.h"
#include <vector>
#include <iostream>

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
	unsigned int workers;

	unsigned int total_bananas;
	unsigned int total_schweppes;
	unsigned int required_bananas;
	unsigned int required_schweppes;

	std::vector<Worker> workers_on_duty;

public:
	void setActionHandler(ActionHandler *handler) override;

	void init(int workerCount, int startBanana, int startSchweppes) override;

	void addClients(const Client *clients, int count) override;

	unsigned int orderedBananas();

	unsigned int orderedSchweppes();

	void advanceTo(int minute) override;

	int calculate_banana(const Client &client);

	int calculate_schweppes(const Client &client);

	void fillStore(unsigned int minute);

	bool checkFor(ResourceType resource);

	int getBanana() const;

	int getSchweppes() const;
};

Store *createStore();