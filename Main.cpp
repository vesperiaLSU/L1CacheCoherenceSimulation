/************************************************/
/*Project: Cache Coherence Design for Multicore Processor*/
/*Author: Chen Guo                                                                */
/*Date: May 2014                                                                   */
/************************************************/

#include <vector>
#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include "Header.h"

/*key parameters:
number of cores = 4;
block size = 15 bits;
cache size = 25 blocks;
associativity = 1 for simplicity reason;
cost to access cache = 1 cycle;
cost to access bus (access other caches) = 10 cycles;
cost to access write buffer (main memory) = 100 cycles;
*/

using namespace std;

void generate_instruction(){
	ofstream output_file("raw_data.txt");
	int core_num;
	int instruction_type;  //"1 = write", "0 = read", "-1 = invalid"
	int address;
	int value;
	for (int i = 0; i < 3000; i++)
	{
		core_num = rand() % 4 + 1;
		instruction_type = rand() % 2;
		address = rand() % 100 + 100;
		value = rand() % 9000 + 1000;
		if (instruction_type == 1)
		{
			//instruction type = write
			output_file<<core_num<<" "<<instruction_type<<" "<<address<<" "<<value<<endl;
		}
		else
		{
			//instruction type = read
			output_file<<core_num<<" "<<instruction_type<<" "<<address<<endl;
		}
	}
}

void split_instruction(){
	string block_info;
	ifstream input_file("raw_data.txt");
	ofstream cache1_output("cache1_instruction.txt");
	ofstream cache2_output("cache2_instruction.txt");
	ofstream cache3_output("cache3_instruction.txt");
	ofstream cache4_output("cache4_instruction.txt");
	if (!input_file.is_open())
	{
		throw exception("The raw data file doesn't exist!");
	}
	while(getline(input_file, block_info))
	{
		char c = block_info[0];
		switch (c)
		{
		case '1':
			cache1_output<<block_info<<endl;
			break;
		case '2':
			cache2_output<<block_info<<endl;
			break;
		case '3':
			cache3_output<<block_info<<endl;
			break;
		case '4':
			cache4_output<<block_info<<endl;
		default:
			break;
		}
	}
}

void main_raw_data(){
	ofstream output_file("main_memory_data.txt");
	for (int i = 100; i < 200; ++i){
		output_file<<i<<" "<<rand() % 9000 + 1000<<endl;
	}
}

bool not_space(char c){
	return !isspace(c);
}

bool space(char c){
	return isspace(c);
}

vector<string> split(const string& s){
	vector<string> information;
	string::const_iterator i = s.begin();
	while (i != s.end())
	{
		i = find_if(i, s.end(), not_space);
		string::const_iterator j = i;
		j = find_if(j, s.end(), space);
		if (i != s.end())
			information.push_back(string(i, j));
		i = j;
	}
	return information;
}

map<int, int> generate_main_map(){
	//main_raw_data();
	ifstream input_file("main_memory_data.txt");
	if (!input_file.is_open())
		throw exception("file not found");
	string main_block_info;
	map<int, int> main_dataset;   //store main memory data sets
	while (getline(input_file, main_block_info))   //fill the associative container with main memory data sets
	{
		vector<string> info_vec;
		info_vec = split(main_block_info);
		MainMemoryBlock main_block;
		main_block.main_address = stoi(info_vec[0]);
		main_block.main_value = stoi(info_vec[1]);
		main_dataset[main_block.main_address] = main_block.main_value;
	}
	return main_dataset;
}

int main(){
	generate_instruction();
	split_instruction();
	main_raw_data();
	map<int, int> main_data_map;
	main_data_map = generate_main_map();
	mutex mutex_main;  
	vector<thread> _t;
	ImplementCache1 c1;  //..........763
	ImplementCache2 c2;  //..........764
	ImplementCache3 c3;  //..........734
	ImplementCache4 c4;  //..........739

	//I can't use the following code because each thread needs to use a new instance of ImplementCache as argument
	//anyway to circumvene this?

	/*unsigned int const hardware_threads = thread::hardware_concurrency();
	cout<<"The CPU core number: " <<hardware_threads<<endl;
	for (unsigned i = 0; i<hardware_threads-1; ++i){
		_t.push_back(thread(&Implementation::process_data, &c1, ref(main_data_map), ref(mutex_main)));
	}*/

	thread t1(&ImplementCache1::process_data, &c1, ref(main_data_map), ref(mutex_main));
	if (t1.joinable()) t1.join();
	thread t2(&ImplementCache2::process_data, &c2, ref(main_data_map), ref(mutex_main));
	if (t2.joinable()) t2.join();
	thread t3(&ImplementCache3::process_data, &c3, ref(main_data_map), ref(mutex_main));
	if (t3.joinable()) t3.join();
	c4.process_data(main_data_map, mutex_main);

	//cout<<"Average hit time for this four core caches: "<<........<<endll;
	//cout<<"Average miss time for this four core caches: "<<........<<endl;

	return 0;
}
