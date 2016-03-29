#pragma once
#include "chef.h"
#include "restourant.h"


void Chef::run(unsigned& barrier_count)
{
	//barrier
	barrier_count--;

	while (barrier_count > 0) {
		std::this_thread::yield();
	}

	//std::cout << "chef thread..." << std::endl;

	while (!restourant->close) {

		//std::cout << "Order up!" << std::endl;
		restourant->print("Order up!", "\n");
		

		{
			std::lock_guard<std::mutex> lk2(restourant->waitperson->m);
			restourant->meal = std::make_unique<Meal>(count);
			restourant->waitperson->condition.notify_one();
		}

		if (++count == 10) {

			restourant->close = true;
		}

		std::unique_lock<std::mutex> lk(m);
		condition.wait(lk, [this]() {return restourant->meal == nullptr; });

	}
}

