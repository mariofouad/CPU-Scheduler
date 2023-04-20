#include "SJF.h"

//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//

SJF::SJF() {
	RDY = new SL_PriorityQueue<Process>;
}

SJF::SJF(int id) {
	ID = id;
	RDY = new SL_PriorityQueue<Process>;
}


void SJF::ScheduleAgo()                                     //Overloaded Scheduler Algorithem for SJF processors
{
}

void SJF::InserttoRDY(Process& P)
{
	RDY->add(P);
	RDYcount++;
}

void SJF::PrintRDY()
{
	RDY->print();
}

bool SJF::IsIdeal()
{
	if (RDY->isEmpty()) return true;
	return false;
}


bool SJF::MoveFromRDYToRUN()
{

	Process prc;
	if (!IsIdeal())
	{
		RDY->remove(prc);
		RUN = new Process(prc);
		RDYcount--;
		return true;
	}
	return false;
}

string SJF::returntypename()
{
	return "[SJF ]";
}

SJF::~SJF()
{

}