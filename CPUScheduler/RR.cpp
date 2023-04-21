#include "RR.h"

//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//

RR::RR() {
	RDY = new LinkedQueue<Process>;
}

RR::RR(int id, int TimeSlice)
{
	ID = id;
	SetTMslice(TimeSlice);
}

RR::RR(int id) {
	ID = id;
	RDY = new LinkedQueue<Process>;
}

void RR::ScheduleAgo()                                      //Overloaded Scheduler Algorithem for RR processors
{
	Process ptorun;
	int ntimesteps = 0;
	if(!IsIdeal() && !IsBusy() )
	{
		RDY->Dequeue(ptorun);
		RDYcount--;
		RUN = &ptorun;
	}
	if (IsBusy() && TMslice != 0) 
	{
		TMslice--;
		RUN->excute1TimeStep();
	}
	if (TMslice == 0)
	{
		return;
	}
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

bool RR::ProcIsFound(Process* p)
{
	Process pfind;
	for (int i = 0; i < RDYcount; i++)
	{
		RDY->Dequeue(*p);
		RDY->Enqueue(*p);
		if (*p == pfind) return true;
	}
	return false;
}

void RR::SetTMslice(int timeslice)
{
	TMslice = timeslice;
}

RR::~RR()                                                   //Default Destructor
{

}