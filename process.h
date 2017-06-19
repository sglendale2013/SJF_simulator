#pragma once
#include<vector>
#include<string>
#include"windows.h"


using namespace std;

class Process
{
public:
	//where process cpu and io bursts are stored
	vector<double> values;

	//where time waiting, state, and pointer to next process is stored.
	float t_wait;
	string state;
	int index;
	string name;
	float io_time;
	float turnaround_time;
	float response_time;


	void init_process()
	{
		t_wait = 0;
		index = 0;
		io_time = 0;
		state = "waiting";
		turnaround_time = 0;
		response_time = 0;
		return;
	}

};





