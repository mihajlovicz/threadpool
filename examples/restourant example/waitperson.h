#pragma once
#include <mutex>
#include <condition_variable>
#include <iostream>


class Restourant;

class Waitperson
{
public:
	std::mutex m;
	std::condition_variable condition;
	Restourant* restourant;

	Waitperson(Restourant* r) :restourant(r) {}

	void run(unsigned& barrier_count);

	~Waitperson() {}
};