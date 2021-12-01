
#include "interface.h"
#include "queue.h"
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
	void onWorkerSend(int minute, ResourceType resource)
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

	void onWorkerBack(int minute, ResourceType resource)
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

	void onClientDepart(int index, int minute, int banana, int schweppes)
	{
		std::cout << index << " " << minute << " " << banana << " " << schweppes << std::endl;
	}
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
	void setActionHandler(ActionHandler *handler) override
	{
		actionHandler = handler;
	}

	void init(int workerCount, int startBanana, int startSchweppes) override
	{
		workers = workerCount;
		total_bananas = startBanana;
		total_schweppes = startSchweppes;
	}

	void addClients(const Client *clients, int count) override
	{
		// TODO check for nullptr
		store_clients.initialize(clients, count);
	}

	unsigned int orderedBananas()
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

	unsigned int orderedSchweppes()
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

	void advanceTo(int minute) override
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

	int calculate_banana(const Client &client)
	{
		int bananas_signed = total_bananas;
		return (bananas_signed - client.banana > 0) ? client.banana : bananas_signed;
	}

	int calculate_schweppes(const Client &client)
	{
		int schweppes_signed = total_schweppes;
		return (schweppes_signed - client.schweppes > 0) ? client.schweppes : schweppes_signed;
	}

	void fillStore(unsigned int minute)
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

	bool checkFor(ResourceType resource)
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

	int getBanana() const
	{
		return total_bananas;
	}

	int getSchweppes() const
	{
		return total_schweppes;
	}
};

/*Store *createStore()
{
	return new MyStore();
}*/