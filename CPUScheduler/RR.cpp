#include "RR.h"

//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//

RR::RR() {
	RDY = new LinkedQueue<Process>;
}

RR::RR(int id) {
	ID = id;
	RDY = new LinkedQueue<Process>;
}

void RR::ScheduleAgo()                                      //Overloaded Scheduler Algorithem for RR processors
{
}

void RR::InserttoRDY(Process& P)
{
	RDY->Enqueue(P);
	RDYcount++;
}

void RR::PrintRDY()
{
	RDY->print();
}

bool RR::IsIdeal()
{
	if (RDY->IsEmpty()) return true;
	return false;
}

bool RR::MoveFromRDYToRUN()
{
	Process p;
	if (!RDY->IsEmpty())
	{
		RDY->Dequeue(p);
		RUN = &p;
		RDYcount--;
		return true;
	}
	return false;
}


string RR::returntypename()
{
	return "[RR  ]";
}

RR::~RR()                                                   //Default Destructor
{

}