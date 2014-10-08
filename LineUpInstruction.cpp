#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Header.h"

using namespace std;

void LineUpInstruction::lineup(){
	if (!input.is_open()) 
		throw exception("File does not exist!");
	   while (getline(input, block_info))
	   {
		counter++;
	   	Instructions instruct;
	   	vector<string> info_vec;
	   	info_vec = split(block_info);
	   	instruct.instruction_type = stoi(info_vec[1]);
	   	instruct.instruction_address = stoi(info_vec[2]);
	   	if (info_vec.size() > 3)
	   	{
	   		instruct.instruction_value = stoi(info_vec[3]);
	   	}
	   	else
	   	{
	   		instruct.instruction_value = 0;
	   	}
	   	instruct_vec.push_back(instruct);
	   }
	cout<<"instruction line: "<<counter<<endl;
}

void ReadCache1::read(){
	ifstream input_file("cache1_instruction.txt");
	input = move(input_file);
}

void ReadCache2::read(){
	ifstream input_file("cache2_instruction.txt");
	input = move(input_file);
}

void ReadCache3::read(){
	ifstream input_file("cache3_instruction.txt");
	input = move(input_file);
}

void ReadCache4::read(){
	ifstream input_file("cache4_instruction.txt");
	input = move(input_file);
}
