#include <iostream>
#include <string.h>
#include <queue>
#include "Process.h"

using namespace std;

#ifndef algorithm_h
#define algorithm_h

class Algorithm
{

public:
  Algorithm(vector<Process> processVec, int processNum);
  void UseAlgorithm();
  vector<Process> doneProcesses_;

private:
  void ExecuteAlgorithm();
  void ExecuteFCFS();
  void ExecuteRR(int cpuTimeout);
  void ExecutePPP();
  void ExecuteIPP();
  void ExecuteNPP();
  void ExecuteSJF();
  void ExecuteSPB();

  int algorithm_;
  int totalProcesses_;
  
  Process* processInCPU_;
  
  vector<Process> inputProcesses_;
  vector<Process> readyQueue_;
  vector<Process> waitingQueue_;

};

#endif
