#include"fileReader.h"

int fileReader::getFileContents(){
	ifstream readFile;
	string fileName,temp;
	totalProcesses_ = 0;
	cout << "Enter the filename:";
	getline (cin, fileName);
	readFile.open(fileName.c_str());
	if(readFile.is_open()){
		while(!readFile.eof()){
			totalProcesses_++;
			getline(readFile,temp);
			fContents_.append(temp);
			fContents_.append(" ");
		}
		fContents_.append("\n");
		readFile.close();
		return 0;
	}
	else
		readFile.close();
		return -1;
}

void fileReader::contentParser(){
	int j=0;
	length_=0;
	for(int i=0;fContents_[i]!='\n';i++){
		if(fContents_[i] == ' '){
			length_++;
		}
	}
	processInfo_ = new int[length_];
	for(int i=0;j<length_;i++){
		processInfo_[j] = stoi(fContents_.substr(i,fContents_.find_first_of(" ",i)));
		i = fContents_.find_first_of(" \n",i);
		j++;
	}
}

void fileReader::processParser(){
	int j = 0;
	for(int i=0;i<totalProcesses_;i++){
		Process temp;
		temp.pid_ = processInfo_[j++];
		temp.timeIn_ = processInfo_[j++];
		temp.prio_ = processInfo_[j++];
		temp.tncpu_ = processInfo_[j++];
		for(int k=0;k<(temp.tncpu_-1);k++){
			temp.cpuBrusts_.push_back(processInfo_[j++]);
			temp.IOBrusts_.push_back(processInfo_[j++]);
		}
		temp.cpuBrusts_.push_back(processInfo_[j++]);	//Always ends with a cpu burst
		if (processes_.size() ==0)
			processes_.push_back(temp);
		else
			for (vector<Process>::iterator it = processes_.begin(); it != processes_.end(); ++it)
				if (temp.timeIn_ > it->timeIn_)
				{
					processes_.insert(it,temp);	//Put process into a vector of processes
					break;
				}
				else if (processes_.end() - 1 == it)
				{
					processes_.push_back(temp);
					break;
				}
	}
	delete processInfo_;
}
