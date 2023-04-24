#include "SJF.h"

//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//

SJF::SJF() {
	RDY = new SL_PriorityQueue<Process*>;
}

SJF::SJF(int id) {
	ID = id;
	RDY = new SL_PriorityQueue<Process*>;
}



void SJF::ScheduleAlgo(int& CTS)                                     //Overloaded Scheduler Algorithem for SJF processors
{
}

void SJF::InserttoRDY(Process* P)
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


bool SJF::MoveFromRDYToRUN(int& CTS)
{
	Process *prc;
	RDY->peek(prc);
	if (!IsIdeal() && !prc->IsOpDone(CTS))
	{
		RDY->remove(prc);
		prc->OpIsDone(CTS);
		RUN = prc;
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