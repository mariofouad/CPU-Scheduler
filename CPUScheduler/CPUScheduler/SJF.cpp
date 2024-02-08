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
void SJF::ScheduleAlgo(int& CTS, int MigrationParameter, bool check)        //Overloaded Scheduler Algorithem for SJF processors
{
	Process* P=new Process;
	RDY->peek(P);
	if (!IsIdeal() && !IsBusy())
	{
		RDY->RemoveSorted(P);
		RDYcount--;
		P->SetResponceTime(CTS);
		RUN = P;
		RemTime(P); 
		if (RUN->MustbeTerminated() || RUN->MustBeBlocked(CTS))
		{
			return;
		}
		//
		//RUN->excute1TimeStep();
	}
	else if (IsBusy())
	{
		if (RUN->MustbeTerminated() || RUN->MustBeBlocked(CTS))
		{
			return;
		}
		RUN->excute1TimeStep();
		//DecrementET();
	}
}
void SJF::InserttoRDY(Process* P)                               //Function overridden to insert in RDY lists
{
	RDY->add(P,P->GetCT());
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
	if (!IsIdeal())
	{
		RDY->remove(prc);
		RUN = prc;
		RDYcount--;
		return true;
	}
	return false;
}
bool SJF::StealProcess(Processor* p)
{
	if (p == nullptr)
	{
		return false;
	}
	if (RDYcount == 1)
	{
		return false;
	}
	Process* Proc = nullptr;
	RDY->remove(Proc);
	RDYcount--;
	RemTime(Proc);
	p->InserttoRDY(Proc);
	p->AddTime(Proc);
	return true;

}
string SJF::returntypename()
{
	return "[SJF ]";
}
SJF::~SJF()
{

}
int SJF::getRDYCount()
{
	return RDYcount;
}

bool SJF::IsIdle()
{
	if (!IsBusy() && RDY->isEmpty())
		return true;
	return false;
}