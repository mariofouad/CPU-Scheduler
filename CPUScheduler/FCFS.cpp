#include "FCFS.h"
//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//

//========================================================== CONSTRUCTORS ============================================//
FCFS::FCFS()                                   //First come first serve processor constructor
{   //create the FCFS RDY list 
	RDY = new LinkedList<Process*>;
}

FCFS::FCFS(int id)                             //First come first serve processor constructor
{	//create the FCFS RDY list and set the ID
	ID = id;
	RDY = new LinkedList<Process*>;
}

FCFS::~FCFS()                                               //Default Destructor
{
	RDY->~LinkedList();
	delete RDY;
}
//======================================================= SCHEDULER ALGORITHM =========================================//
void FCFS::ScheduleAlgo(int& CTS, int MigrationParameter)            //Overloaded Scheduler Algorithem for FCFS processors
{
	Process *P;
	RDY->peek(P); //Check first on the last process
	//Do the scheduling criteria according to FCFS
	if (!RDY->IsEmpty() && !P->IsOpDone(CTS) && !IsBusy())
	{
		RDY->DeleteFirst(P);
		RDYcount--;
		RemTime(P);
		P->SetResponceTime(CTS);
		P->OpIsDone(CTS);
		RUN = P;
		RUN->excute1TimeStep();
	}
	//increment the time step if busy
	else if (IsBusy())
	{
		
		RUN->excute1TimeStep();
	}
}
//========================================================= PROCESS FORKING ===========================================//
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
	return((P->ProcessCanFork(CTS)) && (IsBusy()) && (GenerateRandom() <= 25));
}
//========================================================== PROCESS ORPHAN ===========================================//
bool FCFS::GenerateKillSigToChild(Process* P)
{
	if (P->IsParent()) return true;
	return false;
}
//============================================================ KILL PROCESS ===========================================//
bool FCFS::SearchForProcess(int id, Process*& p, int Curr)
{
	bool found = false;
	if (RUN)
	{
		if (RUN->ID() == id)
		{
			p = RUN;
			return !found;
		}
	}
	for (int i = 0; i < RDYcount; i++)
	{
		Process* ptemp;
		RDY->DeleteFirst(ptemp);
		if ((ptemp->ID() == id) && !(ptemp->IsOpDone(Curr)))
		{
			p = ptemp;
			found = true;
		}
		else
		{
			RDY->InsertEnd(ptemp);
		}
	}
	if (found)
	{
		RDYcount--;
		return true;
	}
	else
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

bool FCFS::ProcIsRun(Process* p)
{
	if (p == RUN)
	{
		return true;
	}
	else
		return false;
}

void FCFS::KillIsDone()
{
	KillList klist;
	list->Dequeue(klist);
}
bool FCFS::IsIdle()
{
	if (!IsBusy() && RDY->IsEmpty()) 
		return true;
	return false;
}
//============================================================ WORK STEALING ======================================//
void FCFS::StealProcess(Processor* p)                                 //Work stealing function
{
	if (p == nullptr)
	{
		return;
	}
	Process* Proc = nullptr;
	RDY->DeleteFirst(Proc);
	RDYcount--;
	RemTime(Proc);
	p->InserttoRDY(Proc);
	p->AddTime(Proc);
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
//=============================================================== PROCESS MIGRATION ===================================//
bool FCFS::ProcessMigratonToRR(Processor* receiver, int MaxW)
{
	if (!receiver || !IsBusy() || receiver == this)
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
//============================================================ REST TO FUNCTIONS ==================================//
void FCFS::InserttoRDY(Process* P)                                    //Function overridden to insert in RDY lists
{
	RDY->InsertEnd(P);
	RDYcount++;
}
string FCFS::returntypename()
{
	return "[FCFS]";
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