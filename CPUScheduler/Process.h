#ifndef PROCESS
#define PROCESS
#pragma once
#include"BTree.h"
#include <iostream>
#include "LinkedQueue.h"

using namespace std;
// Struct containing the IO Request Time and IO Duration
struct IOunit {
	int IO_R;
	int IO_D;
};

class Process
{
	//====================================================================================================================//
	//======================================================== CLASS DATA MEMBERS  ===========================================//
	//====================================================================================================================//
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
	friend ostream& operator << (ostream& Out, Process* p);  //Operator overloading for Cout in Console
	friend ostream& operator <= (ostream& Out, Process* P); // Operator overloading for Cout in file
	int LastOpDone=0;			//Sets the timestep of last operation done
	int ReqDone = 0;			//Sets the timestep that the process need IO
	int TimeInRun = 0;			//Sets the total time the process is in RUN
	int TotalIOD=0;				//Total time the process is handed IO
	int TimesForked=0;			//number of times the process forks
	bool ForkedProc = false;    //Checks if a process  forked
	LinkedQueue<IOunit>* IOrequests;	//A queue of IO requests blocks
	int NoofRR_RUN = 0;		            //Number of timeslices for RR processor given to a process

	BTree<Process*>* Root = nullptr;	//Binary tree of child processes

	//====================================================================================================================//
	//======================================================== CLASS FUNCTIONS ===========================================//
	//====================================================================================================================//
public:
	Process();		//default constructor

	Process(int art, int id, int cpt);		//Parameterized constructor

	void SetRoot(BTree<Process*>* P);		//Setter of binary root

	BTree<Process*>* GetRoot();				//Getter of binary root

	bool IsNew(int& CTS);					//Checks if process came from new

	void OpIsDone(int& CTS);				//Sets Lastest opeation with current AT

	bool IsOpDone(int CTS) const;			//checks if operation done

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

	
	
	void IncrementRR_RUN();

	bool RR_RUN1st();

	void CalcStatistics(int& wt, int& rt, int& tt);

	int ID();

	int GetCT();

	void SetForkedProc();

	bool CreateForkList();

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