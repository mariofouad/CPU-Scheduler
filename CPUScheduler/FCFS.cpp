#include "FCFS.h"

//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//

//========================================================== CONSTRUCTORS ============================================//
FCFS::FCFS()
{
	RDY = new LinkedList<Process*>;
}

FCFS::FCFS(int id)
{
	ID = id;
	RDY = new LinkedList<Process*>;
}
//========================================================== SCHEDULER ALGORITHM ============================================//
void FCFS::ScheduleAlgo(int& CTS, int MigrationParameter)                             //Overloaded Scheduler Algorithem for FCFS processors
{
	Process *P;
	RDY->peek(P);
	if (!IsIdeal() && !P->IsOpDone(CTS) && !IsBusy())
	{
		RDY->DeleteFirst(P);
		RDYcount--;
		P->SetResponceTime(CTS);
		P->OpIsDone(CTS);
		RUN = P;
	}
}
//==========================================================================================================================//
void FCFS::InserttoRDY(Process* P)
{
	RDY->InsertEnd(P);
	RDYcount++;
}

bool FCFS::MoveFromRDYToRUN(int& CTS)
{
	Process *P;
	RDY->peek(P);
	if (!IsIdeal() && !P->IsOpDone(CTS) && !IsBusy())
	{
		RDY->DeleteFirst(P);
		P->OpIsDone(CTS);
		RUN = P;
		RDYcount--;
		return true;
	}
	return false;
}

void FCFS::MoveFromBLKToRUN(Process* P)                      //Virtual function responsible for moving a Process from BLK to RDY list
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

int FCFS::GenerateRandom()
{
	// seed the random number generator
	srand(time(0));

	// generate a random number between 1 and 100
	return (rand() % 100 + 1);
}


bool FCFS::ProcessorCanFork(Process* P, int CTS,int ForkProb)
{
	if (P == nullptr) return false;
	return((P->ProcessCanFork(CTS)) && (IsBusy()) && (GenerateRandom() <= 100));
}

bool FCFS::ProcIsFound(Process* p)
{
	for (int i = 0; i < RDYcount; i++)
	{
		Process *ptemp;
		RDY->Traversal(ptemp, i);
		if (ptemp == p)
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

bool FCFS::SearchForProcess(int id, Process*& p)
{
	if (RUN)
	{
		if (RUN->ID() == id)
		{
			p = RUN;
			return true;
		}
		for (int i = 0; i < RDYcount; i++)
		{
			Process* ptemp;
			RDY->DeleteFirst(ptemp);
			if (ptemp->ID() == id)
			{
				p = ptemp;
				RDYcount--;
				return true;
			}
			else
			{
				RDY->InsertEnd(ptemp);
				return false;
			}
		}
	}
	return false;
}

int FCFS::KillSignalID(int curr)
{
	KillList klist;
	list->peek(klist);
	if (klist.killtime == curr)
	{
		return klist.killid;

	}
}

bool FCFS::KillAgain(int curr)
{
	KillList klist;
	list->Dequeue(klist);
	KillList newklist;
	list->peek(newklist);
	if (newklist.killtime == curr)
	{
		return false;
	}
	return true;
}

void FCFS::SetKillList(int id, int k)
{
	KillList klist;
	klist.killid = id;
	klist.killtime = k;
	list->Enqueue(klist);
}

bool FCFS::KillSignal(int curr)
{
	KillList klist;
	list->peek(klist);
	if (klist.killtime == curr)
	{
		return true;
	}
	return false;
}
void FCFS::UpdateWT_RDY()
{
	Process* p = nullptr;
	for (int i = 0; i < RDYcount; i++)
	{
		RDY->DeleteFirst(p);
		p->Increment1WT();
		RDY->InsertEnd(p);
	}
}
bool FCFS::ProcessMigratonToRR(RR* receiver, int MaxW)
{
	if (!receiver || !IsBusy())
	{
		return false;
	}
	else
	{
		if (RUN->MustMigrateToRR(MaxW))
		{
			receiver->InserttoRDY(RUN);
			KillRUN();
			return true;
		}
		return false;
	}
}