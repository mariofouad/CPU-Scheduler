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
	tempSlice = TimeSlice;
}

RR::RR(int id) {
	ID = id;
	RDY = new LinkedQueue<Process>;
}

void RR::ScheduleAlgo(int& CTS)                                      //Overloaded Scheduler Algorithem for RR processors
{
	Process ptorun;
	if (!IsIdeal() && !IsBusy() && TMslice != 0)
	{
		RDY->Dequeue(ptorun);
		RUN = &ptorun;
		ptorun.excute1TimeStep();
		TMslice--;
	}
	if (IsBusy() && TMslice != 0)
	{
		RUN->excute1TimeStep();
		TMslice--;
	}
	if (IsBusy() && TMslice == 0)
	{
		RDY->Enqueue(*RUN);
		KillRUN();
		TMslice = tempSlice;
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

bool RR::MoveFromRDYToRUN(int& CTS)
{
	Process p;
	RDY->peek(p);
	if (!RDY->IsEmpty()&& !p.IsOpDone(CTS))
	{
		RDY->Dequeue(p);
		p.OpIsDone(CTS);
		RUN = new Process(p);
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