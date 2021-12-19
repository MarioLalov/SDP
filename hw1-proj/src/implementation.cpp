#include "../include/implementation.h"

void MyActionHandler::onWorkerSend(int minute, ResourceType resource)
{
	std::cout << "W " << minute << " ";
	switch (resource)
	{
	case ResourceType::banana:
		std::cout << "banana";
		break;
	case ResourceType::schweppes:
		std::cout << "schweppes";
		break;
	}

	std::cout << std::endl;
}

void MyActionHandler::onWorkerBack(int minute, ResourceType resource)
{
	std::cout << "D " << minute << " ";
	switch (resource)
	{
	case ResourceType::banana:
		std::cout << "banana";
		break;
	case ResourceType::schweppes:
		std::cout << "schweppes";
		break;
	}

	std::cout << std::endl;
}

void MyActionHandler::onClientDepart(int index, int minute, int banana, int schweppes)
{
	std::cout << index << " " << minute << " " << banana << " " << schweppes << std::endl;
}

void MyStore::setActionHandler(ActionHandler *handler)
{
	actionHandler = handler;
}

void MyStore::init(int workerCount, int startBanana, int startSchweppes)
{
	workers = workerCount;
	total_bananas = startBanana;
	total_schweppes = startSchweppes;
}

void MyStore::addClients(const Client *clients, int count)
{
	if(!clients)
	{
		throw std::invalid_argument("Empty client list!");
	}

	store_clients.initialize(clients, count);
}

unsigned int MyStore::orderedBananas()
{
	unsigned int total = 0;

	for (std::size_t i = 0; i < workers_on_duty.size(); i++)
	{
		if (workers_on_duty[i].resource == ResourceType::banana)
		{
			total += 100;
		}
	}

	return total;
}

unsigned int MyStore::orderedSchweppes()
{
	unsigned int total = 0;

	for (std::size_t i = 0; i < workers_on_duty.size(); i++)
	{
		if (workers_on_duty[i].resource == ResourceType::schweppes)
		{
			total += 100;
		}
	}

	return total;
}

void MyStore::advanceTo(int minute)
{
	fillStore(minute);

	while (true)
	{
		// try to serve client
		try
		{
			Client_result cur = store_clients.current(minute);
		}
		catch (const std::invalid_argument &err)
		{
			return;
		}

		Client_result cur = store_clients.current(minute);

		// client that needs to leave
		if (cur.in_hurry)
		{
			int bananas_taken = calculate_banana(cur.client);
			total_bananas -= bananas_taken;
			int schweppes_taken = calculate_schweppes(cur.client);
			total_schweppes -= schweppes_taken;

			actionHandler->onClientDepart(cur.id, minute, bananas_taken, schweppes_taken);

			store_clients.removeClient(cur.id);
		}
		// try to serve current client
		try
		{
			Client_result cur = store_clients.current(minute);
		}
		catch (const std::invalid_argument &err)
		{
			return;
		}

		Client_result cur1 = store_clients.current(minute);

		if (cur1.client.banana <= total_bananas && cur1.client.schweppes <= total_schweppes)
		{
			int bananas_taken = calculate_banana(cur1.client);
			total_bananas -= bananas_taken;
			int schweppes_taken = calculate_schweppes(cur1.client);
			total_schweppes -= schweppes_taken;

			actionHandler->onClientDepart(cur1.id, minute, bananas_taken, schweppes_taken);

			store_clients.removeClient(cur1.id);
			continue;
		}

		break;
	}

	// order products
	unsigned int required_bananas = store_clients.getRequiredBananas();
	unsigned int required_schweppes = store_clients.getRequiredSchweppes();

	if (required_bananas > orderedBananas() + total_bananas)
	{
		if (workers != 0)
		{
			Worker worker_to_be_sent(ResourceType::banana, minute + 60);
			workers--;
			workers_on_duty.push_back(Worker(ResourceType::banana, minute + 60));

			actionHandler->onWorkerSend(minute, ResourceType::banana);
		}
	}

	if (required_schweppes > orderedSchweppes() + total_schweppes)
	{
		if (workers != 0)
		{
			Worker worker_to_be_sent(ResourceType::schweppes, minute + 60);
			workers--;
			workers_on_duty.push_back(Worker(ResourceType::schweppes, minute + 60));

			actionHandler->onWorkerSend(minute, ResourceType::schweppes);
		}
	}
}

int MyStore::calculate_banana(const Client &client)
{
	int bananas_signed = total_bananas;
	return (bananas_signed - client.banana > 0) ? client.banana : bananas_signed;
}

int MyStore::calculate_schweppes(const Client &client)
{
	int schweppes_signed = total_schweppes;
	return (schweppes_signed - client.schweppes > 0) ? client.schweppes : schweppes_signed;
}

void MyStore::fillStore(unsigned int minute)
{
	for (std::size_t i = 0; i < workers_on_duty.size(); i++)
	{
		if (workers_on_duty[i].time_of_arrival == minute)
		{
			switch (workers_on_duty[i].resource)
			{
			case ResourceType::banana:
				total_bananas += 100;
				break;
			case ResourceType::schweppes:
				total_schweppes += 100;
				break;
			}

			actionHandler->onWorkerBack(workers_on_duty[i].time_of_arrival, workers_on_duty[i].resource);

			workers_on_duty.erase(workers_on_duty.begin() + i);

			workers++;
		}
	}
}

bool MyStore::checkFor(ResourceType resource)
{
	for (std::size_t i = 0; i < workers_on_duty.size(); i++)
	{
		if (workers_on_duty[i].resource == resource)
		{
			return true;
		}
	}

	return false;
}

int MyStore::getBanana() const
{
	return total_bananas;
}

int MyStore::getSchweppes() const
{
	return total_schweppes;
}

Store *createStore()
{
	return new MyStore;
}