#pragma once
#include "Processor.h"
#include "LinkedList.h"

//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//

class FCFS : public Processor
{
private:

	LinkedList<Process>* RDY;

public:

	FCFS();											  //Default constructor

	FCFS(int id);

	void ScheduleAlgo(int& CTS);        //Overloaded Scheduler Algorithem for FCFS processors

	void InserttoRDY(Process& P);					  //Virtual function responsible for inserting a Process to RDY list

	bool MoveFromRDYToRUN(int& CTS);

	void MoveFromBLKToRUN(Process& P);                //Virtual function responsible for moving a Process from BLK to RDY list

	void PrintRDY();

	bool IsIdeal();

	string returntypename();

	bool ProcIsFound(Process* p);

	~FCFS();                                                 //Default Destructor
};
