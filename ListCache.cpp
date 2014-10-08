#include "Header.h"
#include <map>
#include <mutex>

using namespace std;

void ListCache::instruct(Instructions value, map<int, int>& main_memory_data, mutex& m){
	hit_miss = false;
	if (value.instruction_type == 1)  //instruction_type == write
	{
		if (!ring_cache.empty())
		{
			for (auto ite = ring_cache.begin(); ite != ring_cache.end(); ++ite){
				if (ite->cache_tag == value.instruction_address){
					if (ite->cache_status == 0)
						write_miss_counter++;
					else{
						write_hit_counter++;
						total_hit_time+=cache_cost;
					}
					ite->cache_tag == value.instruction_address;
					ite->cache_status = value.instruction_type;
					ite->cache_value = value.instruction_value;                                          
					hit_miss = true;
				}
			}
			if (hit_miss == false){   //write-back 1st stage, only writes to the cache, and doens't assume cost to main memory
				write_miss_counter++;
				cache_block.cache_status = value.instruction_type;
				cache_block.cache_tag = value.instruction_address;
				cache_block.cache_value = value.instruction_value;
				ring_cache.push_back(cache_block);
			 }
		}
		else  // ring_cache is empty
		{
			write_miss_counter++;
			cache_block.cache_status = value.instruction_type;
			cache_block.cache_tag = value.instruction_address;
			cache_block.cache_value = value.instruction_value;
			ring_cache.push_back(cache_block);
		}
	}
	else //value.instruction_type == read
	{
		if (!ring_cache.empty())
		{
			for (auto ite = ring_cache.begin(); ite != ring_cache.end(); ++ite){
				if (ite->cache_tag == value.instruction_address){
					total_hit_time+=cache_cost;
					read_hit_counter++;
					hit_miss = true;
				}
			}
			if (hit_miss == false){   //write-back 1st stage, only writes to the cache, and doens't assume cost to main memory
				read_miss_counter++;
				cache_block.cache_status = value.instruction_type;
				cache_block.cache_tag = value.instruction_address;
				unique_lock<mutex> my_lock(m);
				cache_block.cache_value = main_memory_data[value.instruction_address];
				my_lock.unlock();
				total_miss_time += memory_cost;
				ring_cache.push_back(cache_block);
			}
		}
		else  //ring_cache is empty
		{
			read_miss_counter++;
			cache_block.cache_status = value.instruction_type;
			cache_block.cache_tag = value.instruction_address;
			unique_lock<mutex> my_lock(m);
			cache_block.cache_value = main_memory_data[value.instruction_address];
			my_lock.unlock();
			total_miss_time += memory_cost;
			ring_cache.push_back(cache_block);
		}
	}
}

 void ListCache::overwrite(Instructions value, map<int, int>& main_memory_data, mutex& m){  //use overwrite 
	instruct(value, main_memory_data, m);
    if (ring_cache.size() > _capacity)
    {
        Cache discard = ring_cache.front();
		if (discard.cache_status == 1){
			unique_lock<mutex> my_lock(m);
			main_memory_data[discard.cache_tag] = discard.cache_value;
			my_lock.unlock();
			total_miss_time+=memory_cost;
		}
		ring_cache.pop_front();
    }
}
