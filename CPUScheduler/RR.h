#ifndef ROUNDROBIN
#define ROUNDROBIN
#pragma once
#include "Processor.h"
#include "LinkedQueue.h"
#include <iostream>

using namespace std;
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//

class RR : public Processor
{
private:

	LinkedQueue<Process>* RDY;								 //RDY list for RR proceses

public:

	RR();                                                    //Default constructor

	RR(int id);

	void ScheduleAgo();										 //Overloaded Scheduler Algorithem for RR processors

	void InserttoRDY(Process& P);

	void PrintRDY();

	bool IsIdeal();

	bool MoveFromRDYToRUN();

	string returntypename();

	bool ProcIsFound(Process* p) { return 0; }

	~RR();                                                   //Default Destructor
};

#endif