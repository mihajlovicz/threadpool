#pragma once
#include<thread>
#include<exception>
#include<stdexcept>


void fnc_throw(std::exception_ptr& teptr)
{
	try
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		throw std::runtime_error("To be passed between threads");
	}
	catch (...)
	{
		teptr = std::current_exception();
	}
}