#include "Header.h"
#include <map>
#include <mutex>
#include <vector>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void Implementation::process_data(map<int, int>& data_map, mutex& m){
	for (vector<Instructions>::const_iterator ite = cache_vec.begin(); ite != cache_vec.end(); ite++){
		cache_list.overwrite(*ite, data_map, m);
	}
	total_hit_counter = cache_list.read_hit_counter+cache_list.write_hit_counter;
	total_miss_counter = cache_list.read_miss_counter+cache_list.write_miss_counter;
	total_hit_time = cache_list.total_hit_time;
	total_miss_time = cache_list.total_miss_time;
	cout<<total_hit_counter<<" "<<total_miss_counter<<" "<<total_hit_time<<" "<<total_miss_time<<endl;
}

void ImplementCache1::get_data(){
	read1.lineup();
	cache_vec = read1.get_result();
}

void ImplementCache2::get_data(){
	read2.lineup();
	cache_vec = read2.get_result();
}

void ImplementCache3::get_data(){
	read3.lineup();
	cache_vec = read3.get_result();
}

void ImplementCache4::get_data(){
	read4.lineup();
	cache_vec = read4.get_result();
}
