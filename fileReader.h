#include<iostream>
#include<string>
#include<fstream>
#include <vector>
#include"Process.h"

using namespace std;

#ifndef FILEREADER_H
#define FILEREADER_H

class fileReader
{
public:
	int getFileContents();
	void contentParser();
	void processParser();
	string fContents_;
	int *processInfo_;
	int length_,totalProcesses_;
	vector<Process> processes_;
};

#endif