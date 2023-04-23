#ifndef SHORTJOBFIRST
#define SHORTJOBFIRST
#pragma once
#include "Processor.h"
#include "SL_PriorityQueue.h"

//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//

class SJF : public Processor
{
private:

	SL_PriorityQueue<Process>* RDY;						     //RDY list for SJF proceses

public:

	SJF();                                                   //Default constructor

	SJF(int id);

	void ScheduleAlgo(Scheduler* S, int& CTS);									 //Overloaded Scheduler Algorithem for SJF processors

	void InserttoRDY(Process& P);

	void PrintRDY();

	bool IsIdeal();

	bool MoveFromRDYToRUN(int& CTS);

	string returntypename();

	bool ProcIsFound(Process* p) { return 0; }

	~SJF();                                                  //Default Destructor
};

#endif