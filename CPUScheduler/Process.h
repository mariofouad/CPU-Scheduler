#ifndef PROCESS
#define PROCESS
#pragma once
#include <iostream>

using namespace std;

class Process
{
private:

	int PID = 0;				//Process Id
	int AT = 0;					//Arrival time, when the process is ready to be scheduled
	int RT = 0;					//Response time, the diff. between the arrival time and the time the process reach the cpu for the first time
	int CT = 0;					//CPU time, the total time needed to run the process
	int TT = 0;					//Termination time, when the process finish exectuting 
	int TRT = 0;				//Turnaround duration, the total time the process spends in the system from its arrival to termination
	int WT = 0;					//Waiting time, total time wait by the process in the system before being executed
	int ION = 0;				//Number of I/O requests
	bool isNew;					//Checks if a process is New
	bool isReady;				//Checks if a process is Ready
	bool isRunning;				//Checks if a process is Running
	bool isBlocked;				//Checks if a process is Blocked
	bool isTerminated;			//Checks if a process is Terminated
	bool isOrphan;				//Checks is a process is Orphan
	int* IO_R;					//I|O Request time
	int* IO_D;					//I|O Duration time
	int Randomization = 0;		//Returns a number from 1-100 used for managing what to do with the process
	friend ostream& operator << (ostream& Out, const Process& p);

public:

	Process();
	Process(int art, int id, int cpt);
	void SetNumberOfRequests(int n);
	void SetInputData(int ir, int id, int i);
	bool operator == (const Process& p);
	bool operator<(const Process& other) const;
	int GetAT();
	void excute1TimeStep();								//Let the process excute 1 time step only 
	bool MustbeTerminated();							//Returns true if the process is ready to be Terminated
	~Process();
};
#endif