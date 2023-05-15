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



void SJF::ScheduleAlgo(int& CTS, int MigrationParameter)                                     //Overloaded Scheduler Algorithem for SJF processors
{
	Process* P=new Process;
	RDY->peek(P);
	if (!IsIdeal() && !P->IsOpDone(CTS) && !IsBusy())
	{
		RDY->RemoveSorted(P);
		RDYcount--;
		P->SetResponceTime(CTS);
		P->OpIsDone(CTS);
		RUN = P;
		if (RUN->MustbeTerminated() || RUN->MustBeBlocked(CTS))
		{
			return;
		}
		RUN->excute1TimeStep();
		DecrementET();
	}
	else if (IsBusy())
	{
		if (RUN->MustbeTerminated() || RUN->MustBeBlocked(CTS))
		{
			return;
		}
		RUN->excute1TimeStep();
		
	}
}

void SJF::InserttoRDY(Process* P)
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