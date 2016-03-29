#pragma once
#include <iostream>
#include <mutex>


/*
  Synchronizes std::ostream between pool threads.
*/

class Print {


private:

	//std::mutex m;

	std::ostream&  print_one (std::ostream& os)
	{
		return os;
	}

	template <typename A0, typename ...Args>
	std::ostream&  print_one (std::ostream& os, const A0& a0, const Args& ...args)
	{
		os << a0;
		return print_one(os, args...);
	}

	template <typename ...Args>
	std::ostream&  print (std::ostream& os, const Args& ...args)
	{
		return print_one(os, args...);
	}


public:

	static std::mutex m;     // needs to be initialized in cpp file to avoid linker error LNK2005

	template <typename ...Args>
	std::ostream&  operator() (const Args& ...args)
	{
		std::lock_guard<std::mutex> lk(m);
		return print(std::cout, args...);
	}


	template <typename ...Args>
	std::ostream&  operator() (std::ostream& os, const Args& ...args)
	{
		std::lock_guard<std::mutex> lk(m);
		return print_one(os, args...);
	}

};
// std::mutex Print::m;