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
	int tempCT = 0;				//A temporary variable to save the CT time
	int TT = 0;					//Termination time, when the process finish exectuting 
	int TRT = 0;				//Turnaround duration, the total time the process spends in the system from its arrival to termination
	int WT = 0; 				//Waiting time, total time wait by the process in the system before being executed
	int ION = 0;				//Number of I/O requests
	int* IO_R;					//I|O Request time
	int* IO_D;					//I|O Duration time
	int Randomization = 0;		//Returns a number from 1-100 used for managing what to do with the process
	friend ostream& operator << (ostream& Out, Process* p);
	friend ostream& operator <= (ostream& Out, Process* P);
	int LastOpDone=0;
	int ReqDone = 0;
	int TimeInRun = 0;
	int TotalIOD=0;

public:

	Process();

	Process(int art, int id, int cpt);

	bool IsNew(int& CTS);

	void OpIsDone(int& CTS); //me7tageen neshoof law el op 7asal 3aleeha 7aga han3raf lama ne update el time el operation 7asal feeh bel CTS

	bool IsOpDone(int&CTS);

	void SetNumberOfRequests(int n);

	void ExcutionTimeNeeded(int& timeleft);

	bool MustBeBlocked(int& CTS);

	void TerminationTime(int& CTS);

	void SetResponceTime(int& CTS);

	void SetInputData(int ir, int id, int i);

	bool operator == (const Process& p);

	bool operator<(const Process& other) const;

	int GetAT();

	void excute1TimeStep();								//Let the process excute 1 time step only by decreasing 1 timestep from CT
	
	bool MustbeTerminated();							//Returns true if the process is ready to be Terminated		 

	bool MustMigrateToSJF(int RTF);						//Returns True if CT < RTF , ready to be migrated from RR to SJF

	bool MustMigrateToRR(int MaxW);					    //Returns TRue if WT > MaxW , ready to be migrated from FCFS to RR
	
	void CalcStatistics(int& wt, int& rt, int& tt);

	int ID();

	int GetCT();

	int proTRT();

	~Process();
};
#endif