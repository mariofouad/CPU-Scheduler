#ifndef PROCESS
#define PROCESS
#pragma once
#include"BTree.h"
#include <iostream>
#include "LinkedQueue.h"

using namespace std;
struct IOunit {
	int IO_R;
	int IO_D;
};
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
	LinkedQueue<IOunit>* IOs = new LinkedQueue<IOunit>;
	int Randomization = 0;		//Returns a number from 1-100 used for managing what to do with the process
	friend ostream& operator << (ostream& Out, Process* p);
	friend ostream& operator <= (ostream& Out, Process* P);
	int LastOpDone=0;
	int ReqDone = 0;
	int TimeInRun = 0;
	int TimeInBLK = 0;
	int TotalIOD=0;
	int TimesForked=0;
	bool ForkedProc = false;
	LinkedQueue<IOunit>* IOrequests;
	int NoofRR_RUN = 0;
	BTree<Process*>* Root = nullptr;
	Process* Parent = nullptr;
	Process* Child1 = nullptr;
	Process* Child2 = nullptr;
	bool IsOrphaned=false;

public:
	Process();

	Process(int art, int id, int cpt);

	void SetRoot(BTree<Process*>* P);

	BTree<Process*>* GetRoot();

	void SetChild(Process* P);

	void SetParent(Process* P);

	bool IsParent();

	Process* GetLeft();

	Process* GetRight();

	void SetIsOrphaned();

	bool GetIsOrphaned();

	void SetIOs(int IOR, int IOD);

	bool IsForkedProc();

	bool IsNew(int& CTS);

	void SetNumberOfRequests(int n);

	bool MustBeBlocked(int& CTS);

	bool MustRemovedFromBLK();

	void TerminationTime(int& CTS);

	void SetResponceTime(int& CTS);

	bool operator == (const Process& p);

	int GetAT();

	void excute1TimeStep();								//Let the process excute 1 time step only by decreasing 1 timestep from CT
	
	bool MustbeTerminated();							//Returns true if the process is ready to be Terminated		 
	
	void IncrementRR_RUN();

	bool RR_RUN1st();

	void CalcStatistics(int& wt, int& rt, int& tt);

	int ID();

	int GetCT();

	void SetForkedProc();

	bool FstForkFromParent();

	bool ForkFromParent();

	void ForkOpIsDone();

	bool ProcessCanFork(int CTS) const;

	int proTRT();

	bool MustMigrateToSJF(int RTF);						//Returns True if CT < RTF , ready to be migrated from RR to SJF

	bool MustMigrateToRR(int MaxW);					    //Returns TRue if WT > MaxW , ready to be migrated from FCFS to RR

	void Increment1WT();								//Increases the Waiting Time by 1 timestep

	~Process();
};
#endif