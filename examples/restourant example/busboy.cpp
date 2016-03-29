#include "busboy.h"
#include "restourant.h"

void BusBoy::run(unsigned& barrier_count) {

	//barrier
	barrier_count--;

	while (barrier_count > 0) {
		std::this_thread::yield();
	}

	//std::cout << "busboy thread ..." << std::endl;

	while (!restourant->close) {
		std::unique_lock<std::mutex> lk(m);
		condition.wait(lk);
		std::cout << "BusBoy is clearing up.\n";
	}
	std::cout << "out of food, closing..." << std::endl;
}