#ifndef __CPU_Scheduler__Process__
#define __CPU_Scheduler__Process__

#include <iostream>
#include <string.h>
#include <deque>

using namespace std;

class Process
{
public:
    int pid_;	//Process id
    int timeIn_;	//Time of arrival into ready queue
    int prio_;	//Priority code
    int tncpu_;	//Total number of CPU Bursts
	int totalWaitingTime_;
	int totalExecutionTime_;
    deque<int> cpuBrusts_;
    deque<int> IOBrusts_;
	
	Process(int pid, int timeIn, int prio, int tncpu) : 
		pid_(pid), timeIn_(timeIn), prio_(prio), tncpu_(tncpu),
		totalWaitingTime_(0), totalExecutionTime_(0)
	{};
	Process() : 
		totalWaitingTime_(0), totalExecutionTime_(0)
	{};
};

#endif
