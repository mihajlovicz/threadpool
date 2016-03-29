#pragma once
#include "t_pool.h"
#include <numeric>

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init,thread_pool& pool)
{
	unsigned long const length=std::distance(first,last);
	if(!length)
		return init;

	unsigned long const num_blocks = pool.threads_size();
	unsigned long const block_size = length/num_blocks ;

	std::vector<std::future<T>> futures(num_blocks);
	Iterator block_start=first;
	for(unsigned long i=0;i<num_blocks;++i)  
	{
		Iterator block_end=block_start;
		std::advance(block_end,block_size);
		futures[i] = pool.submit(&std::accumulate<Iterator,T>, block_start, block_end,init);
		block_start=block_end;
	}
	T last_result = std::accumulate(block_start, last, init);
	T result=init;
	for(unsigned long i=0;i<num_blocks;++i) 
	{
		result+=futures[i].get();
	}
	result += last_result;
	return result;
}