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
///The idea of RR schedule algo is to control the Processes in RUN state by decrementing the TMslice untill it is equal to zero
///Then the timeslice will be reset and process in RUN will go back to RDY and so on
void RR::ScheduleAlgo(int& CTS, int MigrationParameter)
{
	Process* ptorun = nullptr;
	RDY->peek(ptorun);
	if (!RDY->IsEmpty() && !IsBusy())
	{
		if (!(ptorun->IsOpDone(CTS)))
		{
			RDY->Dequeue(ptorun);
			RDYcount--;
			RUN = ptorun;
			RemTime(ptorun);
			if (RUN->MustMigrateToSJF(MigrationParameter) || RUN->MustBeBlocked(CTS) || RUN->MustbeTerminated()) return;
			ptorun->excute1TimeStep();
			//DecrementET();
			TMslice--;
			ptorun->OpIsDone(CTS);
			if (ptorun->RR_RUN1st())
			{
				ptorun->SetResponceTime(CTS);
			}
			ptorun->IncrementRR_RUN();
		}
	}
	else if (IsBusy() && TMslice > 0)
	{
		RUN->excute1TimeStep();
		//DecrementET();
		TMslice--;
		if (RUN->MustbeTerminated() || RUN->MustBeBlocked(CTS)) ResetTMslice();
	}
	else if (IsBusy() && TMslice <= 0)
	{
		AddTime(RUN);
		RDY->Enqueue(RUN);
		RDYcount++;
		KillRUN();
		ResetTMslice();
	}
}

bool RR::ProcessMigrationToSJF(Processor* receiver, int RTF, int slice)
{
	Process* p = nullptr;
	if (!receiver || !RUN || receiver == this)
	{
		return false;
	}
	else
	{
		if (RUN->MustMigrateToSJF(RTF) && TMslice == slice)
		{
			receiver->InserttoRDY(RUN);
			KillRUN();
			ResetTMslice();
			return true;
		}
		else
		{
			return false;
		}
	}
}
void RR::InserttoRDY(Process* P)                      //Function overridden to insert in RDY lists
{
	RDY->Enqueue(P);
	RDYcount++;
}

void RR::PrintRDY()
{
	RDY->print();
}
string RR::returntypename()
{
	return "[RR  ]";
}
void RR::ResetTMslice()
{
	TMslice = tempSlice;
}
void RR::SetTMslice(int timeslice)
{
	TMslice = timeslice;
}
void RR::StealProcess(Processor* p)
{
	if (p == nullptr)
	{
		return;
	}
	Process* Proc = nullptr;
	RDY->Dequeue(Proc);
	RDYcount--;
	RemTime(Proc);
	p->InserttoRDY(Proc);
	p->AddTime(Proc);
}
bool RR::IsIdle()
{
	if (!IsBusy() && RDY->IsEmpty())
		return true;
	return false;
}
RR::~RR()                                                   //Default Destructor
{

}