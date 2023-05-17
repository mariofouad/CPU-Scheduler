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
//====================================================================================================================//
bool Process::IsNew(int& CTS)
{
	return (AT == CTS);
}

void Process::OpIsDone(int& CTS)
{
	LastOpDone = CTS;
}

bool Process::IsOpDone(int CTS) const
{
	if (LastOpDone==CTS)
	{
		return true;
	}
	return false;
}

void Process::SetNumberOfRequests(int n)
{
	ION = n;
	IO_R = new int[n];
	IO_D = new int[n];
}

bool Process::MustBeBlocked(int& CTS)
{
	if (ION == 0)
	{
		return false;
	}
	if ((IO_R[ReqDone] <= TimeInRun) && !IsOpDone(CTS))
	{
		ReqDone++;
		TimeInRun = 0;
		return true;
	}
	return false;
}

void Process::TerminationTime(int& CTS)
{
	TT = CTS;
	TRT = TT - AT;
	WT = TRT - tempCT;
}

void Process::SetResponceTime(int& CTS)
{
	RT = (CTS - AT);
}

void Process::SetInputData(int ir, int id, int i)
{
	IO_R[i] = ir;
	IO_D[i] = id;
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

bool Process::IsForkedProc()
{
	return ForkedProc;
}
bool Process::ProcessCanFork(int CTS) const
{
	return ((TimesForked < 2) && (!IsOpDone(CTS)));
}

int Process::ID()
{
	return PID;
}

int Process::proTRT()
{
	return TRT;
}

bool Process::MustMigrateToSJF(int RTF)
{
	if (CT < RTF) return true;
	return false;
}

bool Process::MustMigrateToRR(int MaxW)
{
	if (WT > MaxW) return true;
	return false;
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