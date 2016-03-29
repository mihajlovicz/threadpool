#pragma once
#include <mutex>
#include <condition_variable>
#include <iostream>

class Restourant;


class BusBoy
{
public:
	std::mutex m;
	std::condition_variable condition;
	Restourant* restourant;

	BusBoy(Restourant* r) : restourant(r) {}

	void run(unsigned& barrier_count);

	~BusBoy() {}
};