#ifndef PROCESSOR_H
#define PROCESSOR_H
#pragma once
//====================================================================================================================//
#include "Process.h"
#include <iostream>
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//
class Processor
{
protected:

	int ID;							                        //Processor identifiers
	int RDYcount;					                        //number of Ready processes
	Process* RUN;					                        //that should contain only process I think because every process can only excute one
	int ExpectedTime = 0;									//Summation of CPUT of its RDY Processes
	int BusyTime = 0;										//Total busy time
	int IdleTime = 0;
public:

	Processor();                                             //Default constructor

	Processor(int ID);                                       //Non-Default constructor

	virtual void ScheduleAlgo(int& CTS, int MigrationParameter, bool check) = 0;                 //Virtual function to be overloaded in each derived processor

	virtual void InserttoRDY(Process* P) = 0;				 //Virtual function responsible for inserting a Process to RDY list

	friend ostream& operator << (ostream& Out, Processor& P);

	bool operator<(const Processor& P);						//Used to compare Processors with each other according to their ExpectedTime value

	virtual void PrintRDY() = 0;

	Process* GetRUN();

	void PrintRUN();

	virtual bool IsBusy();

	virtual bool StealProcess(Processor* p) = 0;

	void KillRUN();

	virtual int getRDYCount() = 0;

	virtual string returntypename() = 0;

	void AddTime(Process* p);

	int TotalTime();

	void RemTime(Process* p);

	void CalcBusyTime();

	int GetBusyTime();

	int GetIdleTime();

	void CalcIdleTime();

	virtual bool IsIdle() = 0;

	~Processor();                                            //Default Destructor
};
#endif 
