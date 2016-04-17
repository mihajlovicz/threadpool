#include <iostream>
#include <chrono>
#include <future>
#include <vector>
#include "t_pool.h"
#include "accumulate.h"
#include "A.h"
#include "restourant.h"
#include "exception_thread.h"
#include "print.h"
#include "int_str.h"





int main(){

	thread_pool pool;
	Restourant r(&pool);
	std::exception_ptr ex_ptr;
	Print print;
	A a1(33);	
	std::vector<int> v1 = {1,22,3,2,73,15,88,243,1,5,9,44,62,78,112,41,92,37,96,97,46,32,111};
	MyInt myint(5);
	MyString mystr("4");



	/*
	  function f_ex throws exeption from pool thread,exeption is caught later in the main thread, 
	  uses first submit overload.
	*/

	std::future<void> f_ex = pool.submit(fnc_throw, std::ref(ex_ptr));


	/*
	  function &A::get_a is nonconst member function, uses fourth submit overload.
	  function &A::get_a2 is const member function, uses third submit overload.
	*/

	std::cout << pool.submit(&A::get_a, a1,2).get()<<std::endl;	
	std::cout << pool.submit(&A::get_a2, a1).get()<<std::endl;


	/*
	  function parallel_accumulate uses all threads in the pool, every thread calls std::accumulate for a part of vector
	  and partial results are added at the end in the main thread. Pool threads don't exchange data between themselves.
	  Uses first submit overload.
	*/

	std::cout <<"parallel_accumulate "<<parallel_accumulate(v1.begin(), v1.end(), 0, pool) << std::endl;


	/*
	  Restourant::start needs 3 available threads in thread pool to work properly because threads wait on each other
	  through conditional variables. Restourant::start thread starts another 3 threads for Chef::run,Waitperson::run and 
	  Busboy::run functions.
	  In all of this cases fifth submit overload is used. 
	  If 3 threads are not available, which is the case in this example, these functions use std::this_thread::yield 
	  to wait until some of occupated threads is released. 
	  In this case threads wait until std::future f_ex calls get() function.
	  If the pool had more threads available Restourant r would have finished before f_ex.
	*/

	pool.submit(&Restourant::start, std::ref(r)).get();
	


	/*
	  Catch exeption from first example.
	*/

	f_ex.get();
	if (ex_ptr) {
		try {
			std::rethrow_exception(ex_ptr);
		}
		
		catch (const std::runtime_error& ex)
		{
			//std::cerr << "Thread exited with exception: " << ex.what() << "\n";

			/*
			  print is function object that synchronizes std::std::ostream between threads.
			*/
			print(std::cerr, "Thread exited with exception: ", ex.what(), "\n");

		}
		catch (...)
		{
			std::cout << "Thread finished with an error " ;
		}
	}

	/*
	  template functions
	*/

	// template parameter in submit call defines return type of the passed function ,in this case DoAdd's return type
	// depends on the order of input arguments

	auto c1 = pool.submit<MyInt>(DoAdd, myint, mystr).get();  //uses 7th submit overload
	auto c2 = pool.submit<MyString>(DoAdd, mystr, myint).get();

	Base* c_ptr;

	c_ptr= &c1;
	pool.submit<void>(print_base,c_ptr).get(); //uses 8th submit overload

    c_ptr = &c2;
	pool.submit<void>(print_base, c_ptr).get();

	std::getchar();
}


