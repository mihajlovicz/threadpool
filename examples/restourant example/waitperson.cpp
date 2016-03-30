
#include "waitperson.h"
#include "restourant.h"


void Waitperson::run(unsigned& barrier_count) {

	//barrier
	barrier_count--;

	while (barrier_count > 0) {
		std::this_thread::yield(); 
	}

	//std::cout << "waitperson thread ..." << std::endl;

	while (!restourant->close) {

		std::unique_lock<std::mutex> lk(m);
		condition.wait(lk, [this]() {return restourant->meal != nullptr; });

		//std::cout << "WaitPerson get Meal " << restourant->meal->orderNum << std::endl;
		restourant->print("WaitPerson get Meal ", restourant->meal->orderNum, "\n");
		
		{
			std::lock_guard<std::mutex> lk2(restourant->chef->m);
			restourant->meal = nullptr;
			std::cout << "meal taken by the waitperson." << std::endl;
			{
				std::lock_guard<std::mutex> lk3(restourant->boy->m);
				std::cout << "notifying busboy to cleanup ... " << std::endl;
				restourant->boy->condition.notify_one();
			}
			restourant->chef->condition.notify_one();
		}

	}
}
