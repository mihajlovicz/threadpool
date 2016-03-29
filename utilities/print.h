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

	template <class A0, class ...Args>
	std::ostream&  print_one (std::ostream& os, const A0& a0, const Args& ...args)
	{
		os << a0;
		return print_one(os, args...);
	}

	template <class ...Args>
	std::ostream&  print (std::ostream& os, const Args& ...args)
	{
		return print_one(os, args...);
	}


public:

	static std::mutex m;     // needs to be initialized in cpp file to avoid linker error LNK2005

	template <class ...Args>
	std::ostream&  operator() (const Args& ...args)
	{
		std::lock_guard<std::mutex> lk(m);
		return print(std::cout, args...);
	}


	template <class ...Args>
	std::ostream&  operator() (std::ostream& os, const Args& ...args)
	{
		std::lock_guard<std::mutex> lk(m);
		return print_one(os, args...);
	}

};
// std::mutex Print::m;