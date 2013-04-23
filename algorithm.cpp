#include "algorithm.h"


using namespace std;

Algorithm::Algorithm(vector<Process> processVec, int processNum)
	: processInCPU_(NULL)
	, inputProcesses_(processVec)
	, totalProcesses_(processNum)
{}

void Algorithm::UseAlgorithm()
{
  cout << "Which algorithm would you like to use?" << endl;
  cout << "1. First Come First Serve" << endl;
  cout << "2. Round Robin" << endl;
  cout << "3. Polite Preemptive Priority" << endl;
  cout << "4. Impatient Preemptive Priority" << endl;
  cout << "5. Non-Primitive Priority" << endl;
  cout << "6. Shortest Job First" << endl;
  cout << "7. Shortest Previous CPU Burst" << endl;
  cout << "- ";
  cin >> algorithm_;
  ExecuteAlgorithm();
}

void Algorithm::ExecuteAlgorithm()
{
  switch(algorithm_)
  {
  int temp;
    case 1:
      cout << "Executing algorithm First Come First Serve..." << endl;
	  ExecuteFCFS();
      break;
    case 2:
      cout << "Round Robin: How long should the CPU timeout be?" << endl;
      cin >> temp;
      cout << "Executing algorithm Round Robin..." << endl;
	  ExecuteRR(temp);
      break;
	case 3:
	  cout << "Polite Preemptive Priority: How long should the CPU timeout be?" << endl;
      cin >> temp;
	  cout << "Executing algorithm Polite Preemptive Priority..." << endl;
	  ExecutePPP(temp);
	  break;
	case 4:
	  cout << "Executing algorithm Impatient Preemptive Priority..." << endl;
	  ExecuteIPP();
	  break;
	case 5:
	  cout << "Executing algorithm Non-Primitive Priority.." << endl;
	  ExecuteNPP();
	  break;
	case 6:
	  cout << "Executing algorithm Shortest Job First..." << endl;
	  ExecuteSJF();
	  break;
	case 7:
	  cout << "Shortest Previous Burst: What should the weight coefficient be?" << endl;
      cin >> temp;
	  cout << "Executing algorithm Shortest Previous CPU Burst..." << endl;
	  ExecuteSPB(temp);
	  break;
    default:
      cout << "Woops" << endl;
  }
}

//Expected Outcomes with Inputs
//1 3 2 1 7
//2 0 7 2 5 4 2
//3 1 1 2 2 3 3
//P1:	4,14
//P2: 5,16
//P3: 10,19

void Algorithm::ExecuteFCFS(){
	for(int curTime=0;;curTime++){
		
			//decrements all waitingQueue_'s leading IOBursts
		if (waitingQueue_.size() > 0)
			for (vector<Process>::iterator it = waitingQueue_.begin(); it < waitingQueue_.end(); ++it)
				--(it->IOBrusts_.front());

			//increments all readyQueue_'s wait times		
		if (readyQueue_.size() > 0)
			for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); ++it)
				++(it->totalWaitingTime_);

			//adding processes as they get read in
		while((inputProcesses_.size() > 0) && (curTime == inputProcesses_.back().timeIn_))
		{
			readyQueue_.push_back(inputProcesses_.back());
			inputProcesses_.pop_back();
		}
		
			//moves waitingQueue_'s finished IOBursts processes to ready
		if (waitingQueue_.size() > 0)
		{
			for (vector<Process>::iterator it = waitingQueue_.begin(); it < waitingQueue_.end();)
				if (!it->IOBrusts_.front())
				{
					it->IOBrusts_.erase(it->IOBrusts_.begin());
					readyQueue_.push_back(*it);
					it = waitingQueue_.erase(it);
				}
				else
					++it;
		}
			//check for cpu being free
		if (processInCPU_) 
		{
			if (!--(*processInCPU_->cpuBrusts_.begin()))
			{
				processInCPU_->cpuBrusts_.erase(processInCPU_->cpuBrusts_.begin());
				if (processInCPU_->IOBrusts_.size() > 0) 
					waitingQueue_.push_back(*processInCPU_);
				else //process done!
				{
					processInCPU_->totalExecutionTime_ = curTime;
					doneProcesses_.push_back(*processInCPU_);
					if (doneProcesses_.size() == totalProcesses_)
					{
						delete processInCPU_;
						processInCPU_ = NULL;
						return;
					}
				}	
				delete processInCPU_;
				processInCPU_ = NULL;
			}
		}
			//"run" next readyQueue_ process
		if ((!processInCPU_) && readyQueue_.size() > 0)
		{
			processInCPU_ = new Process(readyQueue_.front());
			readyQueue_.erase(readyQueue_.begin());
		}
	}
	
}

//Expected inputs for:
//1 3 2 1 7
//2 0 7 2 5 4 2
//3 1 1 2 2 3 3
//Quantum time: 3
//P1: 9, 19
//P2: 7, 18
//P3: 4, 13

void Algorithm::ExecuteRR(int cpuTimeout){
	int timeOutLimit = 0;
	for(int curTime=0;;curTime++){

			//decrements all waitingQueue_'s leading IOBursts
		if (waitingQueue_.size() > 0)
			for (vector<Process>::iterator it = waitingQueue_.begin(); it != waitingQueue_.end(); ++it)
				--(it->IOBrusts_.front());
		
			//increments all readyQueue_'s wait times		
		if (readyQueue_.size() > 0)
			for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); ++it)
				++(it->totalWaitingTime_);

			//adding processes as they get read in
		while((inputProcesses_.size() > 0) && (curTime == inputProcesses_.back().timeIn_))
		{
			readyQueue_.push_back(inputProcesses_.back());
			inputProcesses_.pop_back();
		}
		
			//decrements all waitingQueue_'s leading IOBursts, transfers to readyQueue_ if done
		if (waitingQueue_.size() > 0)
		{
			for (vector<Process>::iterator it = waitingQueue_.begin(); it < waitingQueue_.end();)
				if (!(it->IOBrusts_.front()))
				{
					it->IOBrusts_.erase(it->IOBrusts_.begin());
					readyQueue_.push_back(*it);
					it = waitingQueue_.erase(it);
				}
				else
					++it;
		}

			//check for cpu being free
		if (processInCPU_) 
		{
			if ( (!--(processInCPU_->cpuBrusts_.front())) || (timeOutLimit == curTime) )
			{
				if (processInCPU_->cpuBrusts_.front() > 0)
				{
					readyQueue_.push_back(*processInCPU_);
				}
				else
				{
					processInCPU_->cpuBrusts_.erase(processInCPU_->cpuBrusts_.begin());
					if (processInCPU_->IOBrusts_.size() > 0) 
						waitingQueue_.push_back(*processInCPU_);
					else //process done!
					{
						processInCPU_->totalExecutionTime_ = curTime;
						doneProcesses_.push_back(*processInCPU_);
						if (doneProcesses_.size() == totalProcesses_)
						{
							delete processInCPU_;
							processInCPU_ = NULL;
							return;
						}
					}
				}	
				delete processInCPU_;
				processInCPU_ = NULL;
			}
		}
		
			//"run" next readyQueue_ process
		if ((!processInCPU_) && (readyQueue_.size() > 0))
		{
			processInCPU_ = new Process(readyQueue_.front());
			readyQueue_.erase(readyQueue_.begin());
			timeOutLimit = curTime + cpuTimeout;
		}
	}
}

//Expected Outputs for Inputs:
//1 4 1 1 4
//2 0 7 2 5 4 2
//3 1 2 2 2 3 3
//Quantum Time: 3
//P1: 1,9
//P3: 3,12
//P2: 9,20

void Algorithm::ExecutePPP(int cpuTimeout){
    int timeOutLimit = 0;
	for(int curTime=0;;curTime++){

		//decrements all waitingQueue_'s leading IOBursts
		if (waitingQueue_.size() > 0)
			for (vector<Process>::iterator it = waitingQueue_.begin(); it != waitingQueue_.end(); ++it)
				--(it->IOBrusts_.front());
		
		//increments all readyQueue_'s wait times		
		if (readyQueue_.size() > 0)
			for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); ++it)
				++(it->totalWaitingTime_);

		//adding processes as they get read in
        while((inputProcesses_.size() > 0) && (curTime == inputProcesses_.back().timeIn_))
		{
            if (readyQueue_.size() > 0)
            {
                for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); it++ )
                {
					if(inputProcesses_.back().prio_ < it->prio_)
                    {
                        readyQueue_.insert(it, inputProcesses_.back());
                        //Insert the process if the priority is less
                        inputProcesses_.pop_back();
                        break;   
                    }
					else if (readyQueue_.end() - 1 == it)
					{
						readyQueue_.push_back(inputProcesses_.back());
						inputProcesses_.pop_back();
						break;
					}
                }
            }
            else
            {
                readyQueue_.push_back(inputProcesses_.back());
                inputProcesses_.pop_back();
            }
		}
		
		// transfers to readyQueue_ if done
		if (waitingQueue_.size() > 0)
		{
			for (vector<Process>::iterator itWaiting = waitingQueue_.begin(); itWaiting < waitingQueue_.end();)
				if (!(itWaiting->IOBrusts_.front()))
				{
					itWaiting->IOBrusts_.erase(itWaiting->IOBrusts_.begin());
					if(readyQueue_.size() > 0)
					{
						//Input process from waiting queue to ready queue sorted by cpu burst
						for(vector<Process>::iterator itReady = readyQueue_.begin(); itReady < readyQueue_.end(); ++itReady)
						{
							if(itWaiting->prio_ < itReady->prio_)
							{
								readyQueue_.insert(itReady, *itWaiting);
								//Insert the CPU brust here if it comes before
								itWaiting = waitingQueue_.erase(itWaiting);
								break;   
							}
							else if (readyQueue_.end() - 1 == itReady)
							{
								readyQueue_.push_back(*itWaiting);
								itWaiting = waitingQueue_.erase(itWaiting);
								break;
							}
						}
					}
					else
					{
						readyQueue_.push_back(*itWaiting);
						itWaiting = waitingQueue_.erase(itWaiting);
					}
				}
				else
					++itWaiting;
		}

			//check for cpu being free
		if (processInCPU_) 
		{
			if ( (!--(processInCPU_->cpuBrusts_.front())) || (timeOutLimit == curTime) )
			{
				if (processInCPU_->cpuBrusts_.front() > 0)
				{
					//Sort by priority
					if (readyQueue_.size() > 0)
					{
						for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); it++ )
						{
							if(readyQueue_.back().prio_ > it->prio_)
							{
								readyQueue_.insert(it, *processInCPU_);
								//Insert the process if the priority is less
								break;   
							}
							else if (readyQueue_.end() - 1 == it)
							{
								readyQueue_.push_back(*processInCPU_);
								break;
							}
						}
					}
					else
					{
						readyQueue_.push_back(*processInCPU_);
					}
				}
				else
				{
					processInCPU_->cpuBrusts_.erase(processInCPU_->cpuBrusts_.begin());
					if (processInCPU_->IOBrusts_.size() > 0) 
						waitingQueue_.push_back(*processInCPU_);
					else //process done!
					{
						processInCPU_->totalExecutionTime_ = curTime;
						doneProcesses_.push_back(*processInCPU_);
						if (doneProcesses_.size() == totalProcesses_)
						{
							delete processInCPU_;
							processInCPU_ = NULL;
							return;
						}
					}
				}	
				delete processInCPU_;
				processInCPU_ = NULL;
			}
		}
		
			//"run" next readyQueue_ process
		if ((!processInCPU_) && (readyQueue_.size() > 0))
		{
			processInCPU_ = new Process(readyQueue_.front());
			readyQueue_.erase(readyQueue_.begin());
			timeOutLimit = curTime + cpuTimeout;
		}
	}
}

void Algorithm::ExecuteIPP(){

}

//Expected Output for inputs:
//1 3 1 1 7
//2 0 7 2 5 4 2
//3 1 2 2 2 3 3
//P1: 2, 12
//P2: 5, 16
//P3: 11, 20

void Algorithm::ExecuteNPP(){
	for(int curTime=0;;curTime++){

		//decrements all waitingQueue_'s leading IOBursts
		if (waitingQueue_.size() > 0)
			for (vector<Process>::iterator it = waitingQueue_.begin(); it != waitingQueue_.end(); ++it)
				--(it->IOBrusts_.front());
		
		//increments all readyQueue_'s wait times		
		if (readyQueue_.size() > 0)
			for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); ++it)
				++(it->totalWaitingTime_);

        //adding processes as they get read in
        while((inputProcesses_.size() > 0) && (curTime == inputProcesses_.back().timeIn_))
		{
            if (readyQueue_.size() > 0)
            {
                for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); it++ )
                {
					if(inputProcesses_.back().prio_ < it->prio_)
                    {
                        readyQueue_.insert(it, inputProcesses_.back());
                        //Insert the process if the priority is less
                        inputProcesses_.pop_back();
                        break;   
                    }
					else if (readyQueue_.end() - 1 == it)
					{
						readyQueue_.push_back(inputProcesses_.back());
						inputProcesses_.pop_back();
						break;
					}
                }
            }
            else
            {
                readyQueue_.push_back(inputProcesses_.back());
                inputProcesses_.pop_back();
            }
		}

		//decrements all waitingQueue_'s leading IOBursts, transfers to readyQueue_ if done
		if (waitingQueue_.size() > 0)
		{
			for (vector<Process>::iterator itWaiting = waitingQueue_.begin(); itWaiting < waitingQueue_.end();)
				if (!(itWaiting->IOBrusts_.front()))
				{
					itWaiting->IOBrusts_.erase(itWaiting->IOBrusts_.begin());
					if(readyQueue_.size() > 0)
					{
						//Input process from waiting queue to ready queue sorted by cpu burst
						for(vector<Process>::iterator itReady = readyQueue_.begin(); itReady < readyQueue_.end(); ++itReady)
						{
							if(itWaiting->prio_ < itReady->prio_)
							{
								readyQueue_.insert(itReady, *itWaiting);
								//Insert the CPU brust here if it comes before
								itWaiting = waitingQueue_.erase(itWaiting);
								break;   
							}
							else if (readyQueue_.end() - 1 == itReady)
							{
								readyQueue_.push_back(*itWaiting);
								itWaiting = waitingQueue_.erase(itWaiting);
								break;
							}
						}
					}
					else
					{
						readyQueue_.push_back(*itWaiting);
						itWaiting = waitingQueue_.erase(itWaiting);
					}
				}
				else
					++itWaiting;
		}
        
        //check for cpu being free
		if (processInCPU_)
		{
			if (!--(*processInCPU_->cpuBrusts_.begin()))
			{
				processInCPU_->cpuBrusts_.erase(processInCPU_->cpuBrusts_.begin());
				if (processInCPU_->IOBrusts_.size() > 0)
					waitingQueue_.push_back(*processInCPU_);
				else //process done!
				{
					processInCPU_->totalExecutionTime_ = curTime;
					doneProcesses_.push_back(*processInCPU_);
					if (doneProcesses_.size() == totalProcesses_)
					{
						delete processInCPU_;
						processInCPU_ = NULL;
						return;
					}
				}
				delete processInCPU_;
				processInCPU_ = NULL;
			}
		}

        if ((!processInCPU_) && (readyQueue_.size() > 0))
		{
			processInCPU_ = new Process(readyQueue_.front());
			readyQueue_.erase(readyQueue_.begin());
		}
        
        
	}
}

//Expected Outputs for Inputs:
//1 3 1 1 1
//2 0 7 2 5 4 2
//3 1 2 2 2 3 3
//P1: 2,6
//P2: 0,11
//P3: 5,14

void Algorithm::ExecuteSJF(){
	for(int curTime=0;;curTime++){
		//decrements all waitingQueue_'s leading IOBursts
		if (waitingQueue_.size() > 0)
			for (vector<Process>::iterator it = waitingQueue_.begin(); it != waitingQueue_.end(); ++it)
				--(it->IOBrusts_.front());
		
		//increments all readyQueue_'s wait times		
		if (readyQueue_.size() > 0)
			for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); ++it)
				++(it->totalWaitingTime_);

        //adding processes as they get read in
		
        while((inputProcesses_.size() > 0) && (curTime == inputProcesses_.back().timeIn_))
		{
            if (readyQueue_.size() > 0)
            {
                for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); it++ )
                {
                    if(inputProcesses_.back().cpuBrusts_.front() < it->cpuBrusts_.front())
                    {
                        readyQueue_.insert(it, inputProcesses_.back());
                        inputProcesses_.pop_back();
                        break;   
                    }
					else if (readyQueue_.end() - 1 == it)
					{
						readyQueue_.push_back(inputProcesses_.back());
						inputProcesses_.pop_back();
						break;
					}
                }
            }
            else
            {
                readyQueue_.push_back(inputProcesses_.back());
                inputProcesses_.pop_back();
            }
		}
        
		//decrements all waitingQueue_'s leading IOBursts, transfers to readyQueue_ if done
		if (waitingQueue_.size() > 0)
		{
			for (vector<Process>::iterator itWaiting = waitingQueue_.begin(); itWaiting < waitingQueue_.end();)
				if (!(itWaiting->IOBrusts_.front()))
				{
					itWaiting->IOBrusts_.erase(itWaiting->IOBrusts_.begin());
					if(readyQueue_.size() > 0)
					{
						//Input process from waiting queue to ready queue sorted by cpu burst
						for(vector<Process>::iterator itReady = readyQueue_.begin(); itReady < readyQueue_.end(); ++itReady)
						{
							if(itWaiting->cpuBrusts_.front() < itReady->cpuBrusts_.front())
							{
								readyQueue_.insert(itReady, *itWaiting);
								//Insert the CPU brust here if it comes before
								itWaiting = waitingQueue_.erase(itWaiting);
								break;   
							}
							else if (readyQueue_.end() - 1 == itReady)
							{
								readyQueue_.push_back(*itWaiting);
								itWaiting = waitingQueue_.erase(itWaiting);
								break;
							}
						}
					}
					else
					{
						readyQueue_.push_back(*itWaiting);
						itWaiting = waitingQueue_.erase(itWaiting);
					}
				}
				else
					++itWaiting;
		}

        //check for cpu being free
		if (processInCPU_)
		{
			if (!--(*processInCPU_->cpuBrusts_.begin()))
			{
				processInCPU_->cpuBrusts_.erase(processInCPU_->cpuBrusts_.begin());
				if (processInCPU_->IOBrusts_.size() > 0)
					waitingQueue_.push_back(*processInCPU_);
				else //process done!
				{
					processInCPU_->totalExecutionTime_ = curTime;
					doneProcesses_.push_back(*processInCPU_);
					if (doneProcesses_.size() == totalProcesses_)
					{
						delete processInCPU_;
						processInCPU_ = NULL;
						return;
					}
				}
				delete processInCPU_;
				processInCPU_ = NULL;
			}
		}

		//Add next processes (ready queue) to cpu
        if ((!processInCPU_) && (readyQueue_.size() > 0))
		{
			processInCPU_ = new Process(readyQueue_.front());
			readyQueue_.erase(readyQueue_.begin());
		}    
	}
}

//Expected Outputs for Inputs:
//1 0 0 3 4 1 2 3 1
//2 3 0 2 2 5 4
//3 4 0 3 6 2 4 1 1
//Weighted Coefficient: 0.5
//P2: 4,18
//P1: 8,19
//P3: 7,25

void Algorithm::ExecuteSPB(float weightC){
	for(int curTime=0;;curTime++){

		//decrements all waitingQueue_'s leading IOBursts
		if (waitingQueue_.size() > 0)
			for (vector<Process>::iterator it = waitingQueue_.begin(); it != waitingQueue_.end(); ++it)
				--(it->IOBrusts_.front());
		
		//increments all readyQueue_'s wait times		
		if (readyQueue_.size() > 0)
			for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); ++it)
				++(it->totalWaitingTime_);

        //adding processes as they get read in
        while((inputProcesses_.size() > 0) && (curTime == inputProcesses_.back().timeIn_))
		{
			//Case for when its the CPUs first burst so it doesn't have a previousCPUburst
            if(inputProcesses_.back().previousCPUBurst == 0)
            {
                inputProcesses_.back().previousCPUBurst = inputProcesses_.back().cpuBrusts_.front();
                inputProcesses_.back().previousCPUBurstAvg = inputProcesses_.back().previousCPUBurst;  
            }

			inputProcesses_.back().prio_ = inputProcesses_.back().cpuBrusts_.front();

            if (readyQueue_.size() > 0)
            {
                for (vector<Process>::iterator it = readyQueue_.begin(); it != readyQueue_.end(); it++ )
                {
					if(inputProcesses_.back().prio_ < it->prio_)
                    {
                        readyQueue_.insert(it, inputProcesses_.back());
                        //Insert the process if the priority is less
                        inputProcesses_.pop_back();
                        break;   
                    }
					else if (readyQueue_.end() - 1 == it)
					{
						readyQueue_.push_back(inputProcesses_.back());
						inputProcesses_.pop_back();
						break;
					}
                }
            }
            else
            {
                readyQueue_.push_back(inputProcesses_.back());
                inputProcesses_.pop_back();
            }
		}

		//transfers to readyQueue_ if done
		if (waitingQueue_.size() > 0)
		{
			for (vector<Process>::iterator itWaiting = waitingQueue_.begin(); itWaiting < waitingQueue_.end();)
				if (!(itWaiting->IOBrusts_.front()))
				{
					itWaiting->IOBrusts_.erase(itWaiting->IOBrusts_.begin());

					//Calculate the average and previous CPU burst
					itWaiting->previousCPUBurstAvg = (weightC * itWaiting->previousCPUBurst) + ((1-weightC)*  itWaiting->previousCPUBurst );
					itWaiting->prio_ = itWaiting->previousCPUBurstAvg;
					itWaiting->previousCPUBurst = itWaiting->cpuBrusts_.front();

					if(readyQueue_.size() > 0)
					{
						//Input process from waiting queue to ready queue sorted by cpu burst
						for(vector<Process>::iterator itReady = readyQueue_.begin(); itReady < readyQueue_.end(); ++itReady)
						{
							if(itWaiting->prio_ < itReady->prio_)
							{
								readyQueue_.insert(itReady, *itWaiting);
								//Insert the CPU brust here if it comes before
								itWaiting = waitingQueue_.erase(itWaiting);
								break;   
							}
							else if (readyQueue_.end() - 1 == itReady)
							{
								readyQueue_.push_back(*itWaiting);
								itWaiting = waitingQueue_.erase(itWaiting);
								break;
							}
						}
					}
					else
					{
						readyQueue_.push_back(*itWaiting);
						itWaiting = waitingQueue_.erase(itWaiting);
					}
				}
				else
					++itWaiting;
		}
        
        //check for cpu being free
		if (processInCPU_)
		{
			if (!--(*processInCPU_->cpuBrusts_.begin()))
			{
				processInCPU_->cpuBrusts_.erase(processInCPU_->cpuBrusts_.begin());
				if (processInCPU_->IOBrusts_.size() > 0)
					waitingQueue_.push_back(*processInCPU_);
				else //process done!
				{
					processInCPU_->totalExecutionTime_ = curTime;
					doneProcesses_.push_back(*processInCPU_);
					if (doneProcesses_.size() == totalProcesses_)
					{
						delete processInCPU_;
						processInCPU_ = NULL;
						return;
					}
				}
				delete processInCPU_;
				processInCPU_ = NULL;
			}
		}

        if ((!processInCPU_) && (readyQueue_.size() > 0))
		{
			processInCPU_ = new Process(readyQueue_.front());
			readyQueue_.erase(readyQueue_.begin());
		}
        
        
	}
}
