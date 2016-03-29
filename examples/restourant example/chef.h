#pragma once
#include <mutex>
#include <condition_variable>
#include <iostream>


class Restourant;

class Chef
{
public:
	std::mutex m;
	std::condition_variable condition;
	Restourant* restourant;
	int count = 0;

	Chef(Restourant* r) :restourant(r) {}

	void run(unsigned& barrier_count);

	~Chef(){}
};
