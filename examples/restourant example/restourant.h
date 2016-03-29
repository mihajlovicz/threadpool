#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "busboy.h"
#include "chef.h"
#include "waitperson.h"
#include "meal.h"
#include "t_pool.h"
#include "print.h"


class Waitperson;
class Chef;
class BusBoy;
class Meal;


class Restourant
{
	unsigned barrier_count = 3;

public:
	std::unique_ptr<Meal> meal = nullptr;  
	std::unique_ptr<Waitperson> waitperson = std::make_unique<Waitperson>(this);
	std::unique_ptr<Chef> chef = std::make_unique<Chef>(this);
	std::unique_ptr<BusBoy> boy = std::make_unique<BusBoy>(this);
	bool close = false;
	thread_pool* pool;
	std::mutex m;
	Print print;

	Restourant(thread_pool* p):pool(p)	{ }


	void start() {

		std::future<void> f1 = pool->submit(&Chef::run, std::ref(*chef),std::ref(barrier_count));
		std::future<void> f2 = pool->submit(&Waitperson::run, std::ref(*waitperson), std::ref(barrier_count));
		std::future<void> f3 = pool->submit(&BusBoy::run, std::ref(*boy), std::ref(barrier_count));
		
		/*
		  If there are not enough free threads ,as in this example, we can't call get() for futures because we
		  would block threads that have already started.
		*/

		/*f1.get();
		f2.get();
		f3.get();*/
	}


	~Restourant() {}

};


