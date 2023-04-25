#include "RR.h"

//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//

RR::RR() {
	RDY = new LinkedQueue<Process*>;
}

RR::RR(int id, int TimeSlice)
{
	RDY = new LinkedQueue<Process*>;
	ID = id;
	SetTMslice(TimeSlice);
	tempSlice = TimeSlice;
}

RR::RR(int id) {
	ID = id;
	RDY = new LinkedQueue<Process*>;
}

void RR::ScheduleAlgo(int& CTS)                                      //Overloaded Scheduler Algorithem for RR processors
{
	Process *ptorun;
	RDY->peek(ptorun);
	if (!RDY->IsEmpty() && !IsBusy() && TMslice != 0)
	{
		if (!(ptorun->IsOpDone(CTS))) 
		{
			RDY->Dequeue(ptorun);
			RDYcount--;
			RUN = ptorun;
			ptorun->excute1TimeStep();
			TMslice--;
			ptorun->OpIsDone(CTS);
			/*ptorun->SetResponceTime(CTS);*/
		}
	}
	else if (IsBusy() && TMslice != 0)
	{
		RUN->excute1TimeStep();
		TMslice--;
	}
	else if (IsBusy() && TMslice == 0)
	{
		Process* p = RUN;
		RDY->Enqueue(RUN);
		RDYcount++;
		KillRUN();
		TMslice = tempSlice;
	}
}

void RR::InserttoRDY(Process* P)
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
	Process *p;
	RDY->peek(p);
	if (!RDY->IsEmpty()&& !p->IsOpDone(CTS))
	{
		RDY->Dequeue(p);
		p->OpIsDone(CTS);
		RUN = p;
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
	Process* pfind = nullptr;
	for (int i = 0; i < RDYcount; i++)
	{
		RDY->Dequeue(pfind);
		RDY->Enqueue(pfind);
		if (p == pfind) return true;
	}
	return false;
}

void RR::SetTMslice(int timeslice)
{
	TMslice = timeslice;
}

bool RR::ProcessMigration(SJF* receiver, int RTF)
{
	Process* p = nullptr;
	for (int i = 0; i < RDYcount; i++)
	{
		RDY->peek(p);
		if (p->MustMigrateToSJF(RTF))
		{
			RDY->Dequeue(p);
			receiver->InserttoRDY(p);
			RDYcount--;
			return true;
		}
	}
	return false;
}

RR::~RR()                                                   //Default Destructor
{

}