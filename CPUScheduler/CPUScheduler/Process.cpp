#pragma once
#include "Process.h"
//==================================================== CONSTRUCTORS ==============================================//
Process::Process()
{
}
Process::Process(int art, int id, int cpt)
{
	PID = id;
	AT = art;
	CT = cpt;
	tempCT = CT;
	LastOpDone = AT;
	ReqDone = 0;
	TimeInRun = 0;
	TimesForked = 0;
}
//=================================================== Process FORKING ===========================================//
void Process::SetRoot(BTree<Process*>*P)
{
	Root = P;
}
BTree<Process*>* Process::GetRoot()
{
	return Root;
}
void Process::SetChild(Process* P)
{
	if (!Child1)
		Child1 = P;
	else if (Child1 && !Child2)
		Child2 = P;
}
void Process::SetParent(Process* P)
{
	Parent = P;
}
//======================================================= PROCESS ORPHAN ============================================//
bool Process::IsParent()
{
	if (TimesForked != 0) return true;
	return false;
}
Process* Process::GetLeft()
{
	return Child1;
}
Process* Process::GetRight()
{
	return Child2;
}
void Process::SetIsOrphaned()
{
	IsOrphaned = true;
}
bool Process::GetIsOrphaned()
{
	return IsOrphaned;
}
//========================================================= IO REQUESTS ==============================================//
void Process::SetIOs(int IOR, int IOD)
{
	IOunit IOreq;
	IOreq.IO_R = IOR;
	IOreq.IO_D = IOD;
	IOs->Enqueue(IOreq);
}
bool Process::MustBeBlocked(int& CTS)
{
	IOunit temp;
	if (ION == 0)
	{
		return false;
	}
	IOs->peek(temp);
	if ((temp.IO_R <= TimeInRun) && !IOs->IsEmpty())
	{
		return true;
	}
	return false;
}
bool Process::MustRemovedFromBLK()
{
	IOunit temp;
	IOs->peek(temp);
	if (temp.IO_D <= TimeInBLK)
	{
		TotalIOD += temp.IO_D;
		ReqDone++;
		IOs->Dequeue(temp);
		TimeInBLK = 0;
		return true;
	}
	else
	{
		TimeInBLK++;
		return false;
	}
}
//===================================================================================================//
bool Process::IsForkedProc()
{
	if (this == nullptr)
	{
		return false;
	}
	return ForkedProc;
}

bool Process::IsNew(int& CTS)
{
	return (AT == CTS);
}

void Process::SetNumberOfRequests(int n)
{
	ION = n;
}
void Process::TerminationTime(int& CTS)
{
	TT = CTS;
	TRT = TT - AT;
	WT = TRT - TimeInRun;
}

void Process::SetResponceTime(int& CTS)
{
	RT = (CTS - AT);
}
void Process::excute1TimeStep()
{
	CT--;
	TimeInRun++;
}

bool Process::MustbeTerminated()
{
	if (CT <= 0) return true;
	return false;
}
void Process::CalcStatistics(int&wt,int&rt,int&tt)
{
	wt = WT;
	rt = RT;
	tt = TT;
}
Process::~Process()
{
}
int Process::GetAT()
{
	return AT;
}
int Process::GetCT()
{
	return CT;
}
void Process::SetForkedProc()
{
	ForkedProc = true;
}

bool Process::FstForkFromParent()
{
	if ((TimesForked == 0) && !ForkedProc) //Parent w ma3mlsh fork 
		return true;
	return false;
}

bool Process::ForkFromParent()
{
	if (!(TimesForked == 0) && !ForkedProc)
		return true;
	return false;
}

void Process::ForkOpIsDone()
{
	TimesForked++;
}

bool Process::ProcessCanFork(int CTS) const
{
	return ((TimesForked < 2));
}

int Process::ID()
{
	return PID;
}

int Process::proTRT()
{
	return TRT;
}

void Process::Increment1WT()
{
	WT++;
}
void Process::IncrementRR_RUN()
{
	NoofRR_RUN++;
}

bool Process::RR_RUN1st()
{
	return (NoofRR_RUN == 0);
}
bool Process::MustMigrateToSJF(int RTF)
{
	if (CT < RTF && !(this->IsForkedProc())) return true;
	return false;
}

bool Process::MustMigrateToRR(int MaxW)
{
	if (WT > MaxW && !(this->IsForkedProc()))  return true;
	return false;
}
ostream& operator << (ostream& Out, Process* P) {
	Out << P->PID;
	return Out;
}

ostream& operator <= (ostream& Out, Process* P)
{
	Out << P->TT << "\t" << P->PID << "\t" << P->AT << "\t" << P->tempCT << "\t" << P->TotalIOD << "\t" << P->WT << "\t" << P->RT << "\t" << P->TRT;
	return Out;
}
bool Process::operator==(const Process& p)
{
	if (PID == p.PID) return true;
	return false;
}