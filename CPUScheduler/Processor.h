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

public:

	Processor();                                             //Default constructor

	Processor(int ID);                                       //Non-Default constructor

	virtual void ScheduleAlgo(int& CTS) = 0;                 //Virtual function to be overloaded in each derived processor

	virtual void InserttoRDY(Process* P) = 0;				 //Virtual function responsible for inserting a Process to RDY list

	friend ostream& operator << (ostream& Out, Processor& P);

	bool operator<(const Processor& P);						//Used to compare Processors with each other according to their ExpectedTime value

	virtual void PrintRDY() = 0;

	Process* GetRUN();

	void PrintRUN();

	virtual bool MoveFromRDYToRUN(int& CTS) = 0;

	virtual bool IsBusy();

	virtual bool IsIdeal() = 0;

	virtual bool ProcIsFound(Process* p) = 0;

	void KillRUN();

	virtual string returntypename() = 0;

	void AddTime(Process* p);

	int TotalTime();

	void RemTime(Process* p);

	void CalcBusyTime();

	int GetBusyTime();

	~Processor();                                            //Default Destructor
};
#endif 
