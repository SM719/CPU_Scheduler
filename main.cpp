#include<iostream>
#include"fileReader.h"
#include"algorithm.h"

using namespace std;

int main(void){
	ifstream fd;
	fileReader *fr = new fileReader();
	if(fr->getFileContents() == -1)
		cout << "Failed to open file" << endl;
	else{
		fr->contentParser();
		fr->processParser();
		Algorithm *algorithm = new Algorithm(fr->processes_, fr->totalProcesses_);
		algorithm->UseAlgorithm();
	
		for(unsigned int i = 0; i < algorithm->doneProcesses_.size();i++){
			cout << "Process " << algorithm->doneProcesses_.at(i).pid_ << " Total execution time: "<< algorithm->doneProcesses_.at(i).totalExecutionTime_ << endl;
			cout << "Process " << algorithm->doneProcesses_.at(i).pid_ << " Total waiting time: "<< algorithm->doneProcesses_.at(i).totalWaitingTime_<< endl;
		}
	}
	//system("PAUSE");
}
