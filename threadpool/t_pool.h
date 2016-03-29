#pragma once
#include <thread>
#include <atomic>
#include <vector>
#include <future>
#include <type_traits>
#include <utility>
#include <algorithm>
#include "t_s_queue.h"
#include "join_threads.h"
#include "function_wrapper.h"


class thread_pool
{
	std::atomic<bool> done;
	std::vector<std::thread> threads;
	threadsafe_queue<function_wrapper> work_queue;
	join_threads joiner;
	unsigned MIN_THREAD_NUMBER = 3; // I arbitrarily chosed 3 to fit one of examples

    void worker_thread()
	{
		while(!done)
		{
			function_wrapper task;  
			if(work_queue.try_pop(task))
			{
				//std::cout << "task poped " << std::endl;
				task();

			}
			else
			{
				std::this_thread::yield();
			}
		}
	}
public:

	thread_pool():
		done(false),joiner(threads)
	{		
		unsigned const thread_count = std::max(MIN_THREAD_NUMBER, std::thread::hardware_concurrency());
		try
		{
			for(unsigned i=0;i<thread_count;++i)
			{
				threads.push_back(
					std::thread(&thread_pool::worker_thread,this));
			}
		}
		catch(...)
		{
			done=true;
			throw;
		}
	}



	/*
		Generalized submit function
	*/

		template<
			 typename F, typename... Args,
			 typename = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type
			>
			auto submit(F&& f, Args&&... args) //-> decltype(f(std::forward<Args>(args)...))  //c++11
		{
			//std::cout << "submit 1 " << std::endl;
			typedef typename std::result_of<F(Args&&... args)>::type result_type;
			std::packaged_task<result_type()> task(std::bind(std::move(f), std::forward<Args>(args)...));
			std::future<result_type> res(task.get_future());
			work_queue.push(function_wrapper(std::move(task)));
			return res;
		}

		// const member function
		template<typename R, typename C, typename... Args>
		auto submit(R(C::*f)() const, const C& c, Args&&... args) //-> R
		{
			//std::cout << "submit 2 " << std::endl;
			std::packaged_task<R()> task(std::bind(std::move(f), c, std::forward<Args>(args)...));
			std::future<R> res(task.get_future());
			work_queue.push(function_wrapper(std::move(task)));
			return res;
		}

		template<typename R, typename C, typename... Args>
		auto submit(R(C::*f)() const, C& c, Args&&... args)// -> R
		{
			//std::cout << "submit 3 " << std::endl;
			std::packaged_task<R()> task(std::bind(std::move(f), c, std::forward<Args>(args)...));
			std::future<R> res(task.get_future());
			work_queue.push(function_wrapper(std::move(task)));
			return res;
		}

		// non-const member function
		template<typename R, typename C, typename... Args>
		auto submit(R(C::*f)(Args... args), C& c, Args&&... args)// -> R
		{
			//std::cout << "submit 4 " << std::endl;
			std::packaged_task<R()> task(std::bind(std::move(f),c, std::forward<Args>(args)...));
			std::future<R> res(task.get_future());
			work_queue.push(function_wrapper(std::move(task)));
			return res;

		}

		
		/*
		  if only typename... Args was used, instead of both Args and Tn, call to this overload in examples
		  would not compile.
		*/

		template<typename R, typename C, typename... Args, typename... Tn>
		auto submit(R(C::*f)(Args... args), std::reference_wrapper<C> c, Tn&&... arg)// -> R   
		{
			//std::cout << "submit 5 " << std::endl;
			std::packaged_task<R()> task(std::bind(std::move(f), c, std::forward<Tn>(arg)...));
			std::future<R> res(task.get_future());
			work_queue.push(function_wrapper(std::move(task)));
			return res;
		}



		size_t threads_size()
		{
			return threads.size();
		}

		~thread_pool()
		{
			done = true;
		}

};