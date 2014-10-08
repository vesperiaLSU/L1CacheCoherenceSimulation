#ifndef GUARD_Cache_h
#define GUARD_Cache_h

#include <list>
#include <map>
#include <mutex>
#include <vector>
#include <string>
#include <fstream>

#define cache_size 25
#define associativity 1 //full associativity
#define cache_cost 1
#define bus_cost 10
#define memory_cost 100

struct Instructions{
	int instruction_type;
	int instruction_address;
	int instruction_value;
};

struct Cache{
	Cache(): cache_status(-1) { }
	int cache_status;
	int cache_tag;
	int cache_value;
};

struct MainMemoryBlock{
	int main_address;
	int main_value;
};

class ListCache{
friend class Implementation;
private:
	int read_hit_counter;   
	int read_miss_counter;   
	int write_hit_counter;   
	int write_miss_counter;   
	int total_miss_time;
	int total_hit_time;
	int _capacity;
	bool hit_miss;  // hit = true, miss = false
	std::list<Cache> ring_cache;
	Cache cache_block;
	MainMemoryBlock main_block;
public:
	ListCache() : _capacity(cache_size), read_hit_counter(0), read_miss_counter(0),
		total_miss_time(0), write_hit_counter(0), write_miss_counter(0), hit_miss(false), total_hit_time(0) { }

	void instruct(Instructions, std::map<int, int>&, std::mutex&);
	void overwrite(Instructions, std::map<int, int>&, std::mutex&);
};

class LineUpInstruction{
public:
	LineUpInstruction() : counter(0) { }
	int counter;
	std::vector<Instructions> get_result(){
		return instruct_vec;
	}
	virtual void read() { }    //is this step necessary in the base class?
	void lineup();
protected:
	std::ifstream input;
	std::string block_info;
	std::vector<Instructions> instruct_vec;
};

class ReadCache1 : public LineUpInstruction{
public:
	ReadCache1() { read(); }
private:
	void read();
};

class ReadCache2 : public LineUpInstruction{
public:
	ReadCache2() { read(); }
private:
	void read();
};

class ReadCache3 : public LineUpInstruction{
public:
	ReadCache3() { read(); }
private:
	void read();
};

class ReadCache4 : public LineUpInstruction{
public:
	ReadCache4() { read(); }
private:
	void read();
};

class Implementation{
public:
	Implementation() : total_hit_counter(0), total_miss_counter(0), total_miss_time(0), total_hit_time(0) { }
	int total_hit_time;
	int total_miss_time;
	int total_hit_counter;
	int total_miss_counter;
	virtual void get_data() { }   //is this step necessary in the base class?
	void process_data(std::map<int, int>&, std::mutex&);
protected:
	std::vector<Instructions> cache_vec;
	ListCache cache_list;
};

class ImplementCache1 : public Implementation{
public:
	ImplementCache1() { get_data(); }
	void get_data();
private:
	ReadCache1 read1;
};

class ImplementCache2 : public Implementation{
public:
	ImplementCache2() { get_data(); }
	void get_data();
private:
	ReadCache2 read2;
};

class ImplementCache3 : public Implementation{
public:
	ImplementCache3() { get_data(); }
	void get_data();
private:
	ReadCache3 read3;
};

class ImplementCache4 : public Implementation{
public:
	ImplementCache4() { get_data(); }
	void get_data();
private:
	ReadCache4 read4;
};

std::vector<std::string> split(const std::string&);

#endif
