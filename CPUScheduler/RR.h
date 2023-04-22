#ifndef ROUNDROBIN
#define ROUNDROBIN
#pragma once
#include "Processor.h"
#include "LinkedQueue.h"
#include <iostream>

using namespace std;
//====================================================================================================================//
//=======================================================  CLASS FUNCTIONS ===========================================//
//====================================================================================================================//

class RR : public Processor
{
private:

	LinkedQueue<Process>* RDY;								 //RDY list for RR proceses
	int TMslice = 0;										 //Time Slice data will be set by the scheduler after reading the input file

public:

	RR();                                                    //Default constructor

	RR(int id);												 //ID initializer

	RR(int id , int TimeSlice);								 //ID , Time slice initializer 

	void ScheduleAgo();										 //Overloaded Scheduler Algorithem for RR processors

	void InserttoRDY(Process& P);							 //This function will insert a Process into that processor RDY list

	void PrintRDY();										 //Prints the data in the RDY list 

	bool IsIdeal();											 //Returns true if the RDY list is empty

	bool MoveFromRDYToRUN(int &CTS);								 //Moves the first process in The RDY queue to RUN state

	string returntypename();								 //Returna a string with the type name which is RR

	bool ProcIsFound(Process* p);							 //Returns true if a process is found in the RDY list

	void SetTMslice(int timeslice);							 //Sets the time slice of RR processors

	~RR();                                                   //Default Destructor
};

#endif