#ifndef SHORTJOBFIRST
#define SHORTJOBFIRST
//====================================================================================================================//
//======================================================== CLASS INCLUDES ============================================//
//====================================================================================================================//
#pragma once
#include "Processor.h"
#include "SL_PriorityQueue.h"
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//
class SJF : public Processor
{
private:
	SL_PriorityQueue<Process*>* RDY;						     //RDY list for SJF proceses
public:
	SJF();                                                   //Default constructor

	SJF(int id);

	void ScheduleAlgo(int& CTS, int MigrationParameter, bool check);									 //Overloaded Scheduler Algorithem for SJF processors

	void InserttoRDY(Process* P);

	void PrintRDY();

	bool IsIdeal();
	bool StealProcess(Processor* p);
	
	bool MoveFromRDYToRUN(int& CTS);

	string returntypename();

	bool ProcIsFound(Process* p) { return 0; }

	int getRDYCount();

	~SJF();                                                  //Default Destructor
	bool IsIdle();
};
#endif