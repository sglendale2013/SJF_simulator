#include<iostream>	
#include<vector>
#include"process.h"
#include<deque>
#include<iomanip>
#include<fstream>
using namespace std;

deque<Process> process_sort(deque<Process>, int);
deque<Process> io_decrement(deque<Process>, string &, int &, deque<Process> &);


//Global variables
float execution_time = 0;
int p_complete = 0;
vector<Process> complete;

//Notes for shortest job first: best case scenario as CPU utilization approaches 100%:
//execution time = 739


int main()
{

	//Initialize all processes vectors 
	Process p1, p2, p3, p4, p5, p6, p7, p8;

	p1.values = { 4, 24,  5, 73, 3, 31, 5, 27, 4, 33, 6, 43, 4, 64, 5, 19, 2 };
	p2.values = { 18, 31, 19, 35, 11, 42, 18, 43, 19, 47, 18, 43, 17, 51, 19, 32, 10 };
	p3.values = { 6, 18,  4, 21, 7, 19, 4, 16, 5, 29, 7, 21, 8, 22, 6, 24, 5 };
	p4.values = { 17, 42, 19, 55, 20, 54, 17, 52, 15, 67, 12, 72, 15, 66, 14 };
	p5.values = { 5, 81, 4, 82, 5, 71, 3, 61, 5, 62, 4, 51, 3, 77, 4, 61, 3, 42, 5 };
	p6.values = { 10, 35, 12, 41, 14, 33, 11, 32, 15, 41, 13, 29, 11 };
	p7.values = { 21, 51, 23, 53, 24, 61, 22, 31, 21, 43, 20 };
	p8.values = { 11, 52, 14, 42, 15, 31, 17, 21, 16, 43, 12, 31, 13, 32, 15 };

	p1.name = "P1";
	p2.name = "P2";
	p3.name = "P3";
	p4.name = "P4";
	p5.name = "P5";
	p6.name = "P6";
	p7.name = "P7";
	p8.name = "P8";

	//Initialize t_wait = 0, state = "waiting" and index = 0
	p1.init_process();
	p2.init_process();
	p3.init_process();
	p4.init_process();
	p5.init_process();
	p6.init_process();
	p7.init_process();
	p8.init_process();

	//Construct ready_q
	deque<Process> ready_q;
	ready_q.push_back(p1);
	ready_q.push_back(p2);
	ready_q.push_back(p3);
	ready_q.push_back(p4);
	ready_q.push_back(p5);
	ready_q.push_back(p6);
	ready_q.push_back(p7);
	ready_q.push_back(p8);

	//io_q initially empty
	deque<Process> io_q;

	//Initially set to the size of the ready queue
	int p_pending = ready_q.size();
	float total_twaits = 0;
	float total_turnaround_times = 0;
	float total_response_time = 0;
	float cpu_inactive = 0;
	float utilization = 0;

	//Write to output file
	ofstream fout;
	fout.open("SJF.txt");


	fout << "COP6410\nAssignment 3: SJF\nSharon A Glendale\nZ23285369\n\n";


	while (p_pending > p_complete)
	{

		if (ready_q.size() != 0)//if items still in ready queue
		{

			//If more than one item, sort
			if (ready_q.size() > 1)
			{
				ready_q = process_sort(ready_q, ready_q.size());
			}

			fout << "Current Time:\t" << execution_time << endl;
			fout << "Now running:\t" << ready_q.at(0).name << endl << endl;

			//Initialize response time based on whether index = 0; this means this is the 
			//first time that the process is run.
			if (ready_q.at(0).index == 0)
			{
				ready_q.at(0).response_time = execution_time;
			}

			//Select process at beginning of ready_q and run
			ready_q.at(0).state = "running";
			fout << "..................................................." << endl;
			fout << "Ready Queue:\tProcess\tBurst " << endl;
			if (ready_q.size() == 1)
			{
				fout << "\t\t\t[empty]" << endl;
			}
			else
			{	//Display all values currently in ready_q
				for (int i = 1; i < ready_q.size(); i++)
				{
					fout << "\t\t\t" << ready_q.at(i).name << "\t\t" << ready_q.at(i).values[ready_q.at(i).index] << endl;
				}
			}
			fout << "..................................................." << endl;
			fout << "Now in IO:\tProcess\tRemaining I/O time " << endl;
			if (io_q.size() == 0)
			{
				fout << "\t\t\t[empty]" << endl;
			}
			else
			{
				for (int i = 0; i < io_q.size(); i++)
				{
					fout << "\t\t\t" << io_q.at(i).name << "\t\t" << io_q.at(i).io_time << endl;
				}
			}
			fout << "..................................................." << endl;

			//Adjust values in queues for the amount of the current cpu burst

			//Actual functionality begins here
			//if case addresses where more than one process in ready_q. That is, besides
			//the currently running process, there are other processes present in the queue
			if (ready_q.size() != 1)
			{
				//For the value of the current CPU burst, increment execution time
				for (int i = 0; i < ready_q.at(0).values[ready_q.at(0).index]; i++)
				{
					execution_time++;

					if (io_q.size() != 0)
					{
						string name = "";
						int counter = 1;

						//And if the io_q is not empty, decrement io_time for each value
						while (io_q.size() != 0 && name != io_q.at(0).name)//(int j = 0; j < io_q.size(); j++)
						{
							io_q = io_decrement(io_q, name, counter, ready_q);

						}
					}//if(io_q.empty() == false)

					if (ready_q.size() > 1)//If more than the currently running process
					{
						for (int k = 1; k < ready_q.size(); k++)
						{
							//increment t_waits for all processes
							ready_q.at(k).t_wait++;
						}
					}
				}//for (int i = 0; i < ready_q.at(0).values[ready_q.at(0).index]; i++)
			}
			else//Case where only one process is in ready_q; that  is, the currently running process
			{
				for (int i = 0; i < ready_q.at(0).values[ready_q.at(0).index]; i++)
				{
					execution_time++;
					string name = "";
					int counter = 1;

					//If the io_q is not empty, decrement io_time for each value
					while (io_q.size() != 0 && name != io_q.at(0).name)
					{
						io_q = io_decrement(io_q, name, counter, ready_q);

					}
				}


			}
			fout << ":::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

			//Move index forward for current process forward
			ready_q.at(0).index++;
			//If the index is the size of the process (since index' start at 0), the process is complete
			if (ready_q.at(0).index == ready_q.at(0).values.size())
			{
				ready_q.at(0).state = "completed";
				ready_q.at(0).turnaround_time = execution_time;
				p_complete++;
				complete.push_back(ready_q.at(0));
				ready_q.erase(ready_q.begin());

			}
			else//Else assume currently running process has io next
			{
				ready_q.at(0).state = "in_io";
				ready_q.at(0).io_time = ready_q.at(0).values[ready_q.at(0).index];
				io_q.push_back(ready_q.at(0));
				ready_q.erase(ready_q.begin());
			}
		}//if (ready_q.size()! =0)
		else //Address the case where everything is in io_q and nothing is in ready.
		{
			//Display everything in io_q
			fout << "Current Time:\t" << execution_time << endl;
			fout << "..................................................." << endl;
			fout << "Now in I/O:\tProcess\tRemaining I/O time" << endl;

			for (int i = 0; i < io_q.size(); i++)
			{
				fout << "\t\t\t" << io_q.at(i).name << "\t\t" << io_q.at(i).io_time << endl;
			}

			string name = "";
			int counter = 1;

			//Decrement io_time for all values in io_q and if io_time == 0
			//send to ready_q or complete queue
			cpu_inactive++;
			execution_time++;

			while (io_q.size() != 0 && name != io_q.at(0).name)
			{
				io_q = io_decrement(io_q, name, counter, ready_q);
			}


		}//Address the case where everything is in io_q and nothing is in ready.
		fout << ":::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
	}//while (p_pending != p_complete)

	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	 //Output to be displayed at program conclusion 
	fout << endl << endl << "Finished" << endl
		<< "Total time: " << execution_time << endl;
	utilization = 100 * (execution_time - cpu_inactive) / execution_time;
	fout << "CPU Utilization: " << setprecision(5) << utilization << "%" << endl;

	//Waiting Times and average
	fout << "Waiting Times:" << endl;
	for (int i = 0; i < complete.size(); i++)
	{
		fout << complete.at(i).name << "\t" << complete.at(i).t_wait << endl;
		total_twaits += complete.at(i).t_wait;
	}
	total_twaits = total_twaits / complete.size();
	fout << "Average wait time = " << total_twaits << endl << endl;

	//Turnaround time and average
	fout << "Turnaround times: " << endl;
	for (int i = 0; i < complete.size(); i++)
	{
		fout << complete.at(i).name << "\t" << complete.at(i).turnaround_time << endl;
		total_turnaround_times += complete.at(i).turnaround_time;
	}
	total_turnaround_times = total_turnaround_times / complete.size();
	fout << "Average turnaround time = " << total_turnaround_times << endl << endl;

	//Response times and average
	fout << "Response times: " << endl;
	for (int i = 0; i < complete.size(); i++)
	{
		fout << complete.at(i).name << "\t" << complete.at(i).response_time << endl;
		total_response_time += complete.at(i).response_time;
	}
	total_response_time = total_response_time / complete.size();
	fout << "Average response time = " << total_response_time << endl;
	fout.close();

	return 0;
}


//End of output//////////////////////////////////////////////////////////////////////////////////////////////////////////

deque<Process> process_sort(deque<Process> readyq, int n)
{
	bool swapped = true;
	int s = 0;
	Process tmp;
	while (swapped)
	{
		swapped = false;
		s++;
		for (int y = 0; y < n - s; y++)
		{
			if (readyq.at(y).values[readyq.at(y).index] > readyq.at(y + 1).values[readyq.at(y + 1).index])
			{
				tmp = readyq.at(y);
				readyq.at(y) = readyq.at(y + 1);
				readyq.at(y + 1) = tmp;
				swapped = true;
			}
		}
	}
	return readyq;
}


//This function needed to be used 3 times:
// 1.) When the ready queue has only one process (considered as the currently running process)
//and the io_q is not empty
// 2.) When the ready queue has more than one process and the io queue is not empty
// And 3.) when all remaining processes are in io and nothing is in the ready_q
deque<Process> io_decrement(deque<Process> ioq, string &name, int &counter, deque<Process> &ready_q)
{
	//decrement io time for all values in io
	ioq.at(0).io_time--;
	//Do only for first iteration
	if (counter == 1)
	{
		name = ioq.at(0).name;
	}
	counter++;

	//if any should complete io, send to ready_q or complete queue
	if (ioq.at(0).io_time == 0)
	{
		ioq.at(0).index++;
		if (ioq.at(0).index == ioq.at(0).values.size())
		{
			ioq.at(0).state = "completed";
			ioq.at(0).turnaround_time = execution_time;
			p_complete++;
			complete.push_back(ioq.at(0));
			ioq.erase(ioq.begin());

		}
		else//else, send to ready_q and allow to wait
		{
			ioq.at(0).state = "waiting";
			ready_q.push_back(ioq.at(0));
			ioq.erase(ioq.begin());
		}
	}

	if (ioq.size() > 1)
	{
		Process temp = ioq.at(ioq.size() - 1);
		ioq.at(ioq.size() - 1) = ioq.at(0);
		ioq.at(0) = temp;
	}
	return ioq;

}

