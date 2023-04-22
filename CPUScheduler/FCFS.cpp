#include <iostream>
#include "FCFS.h"
#include "LinkedList.h"
using namespace std;

//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//

FCFS::FCFS() {
	RDY = new LinkedList<Process>;
}

FCFS::FCFS(int id) {
	ID = id;
	RDY = new LinkedList<Process>;
}

void FCFS::ScheduleAgo()                                    //Overloaded Scheduler Algorithem for FCFS processors
{
}

void FCFS::InserttoRDY(Process& P)
{
	RDY->InsertEnd(P);
	RDYcount++;
}

bool FCFS::MoveFromRDYToRUN(int& CTS)
{
	Process P;
	RDY->peek(P);
	if (!IsIdeal() && !P.IsOpDone(CTS))
	{
		RDY->DeleteFirst(P);
		P.OpIsDone(CTS);
		RUN = new Process(P);
		RDYcount--;
		return true;
	}
	return false;
}

void FCFS::MoveFromBLKToRUN(Process& P)                      //Virtual function responsible for moving a Process from BLK to RDY list
{
	RDY->InsertEnd(P);
	RDYcount++;
}

void FCFS::PrintRDY()
{
	RDY->PrintList();
}

bool FCFS::IsIdeal()
{
	if (RDY->IsEmpty()) return true;
	return false;
}

string FCFS::returntypename()
{
	return "[FCFS]";
}

bool FCFS::ProcIsFound(Process* p)
{
	for (int i = 0; i < RDYcount; i++)
	{
		Process ptemp;
		RDY->Traversal(ptemp, i);
		if (ptemp == *p)
		{
			return true;
		}
	}
	return false;
}


FCFS::~FCFS()                                               //Default Destructor
{
	RDY->~LinkedList();
	delete RDY;
}