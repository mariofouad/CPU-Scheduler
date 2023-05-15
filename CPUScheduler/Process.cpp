#pragma once
#include "Process.h"

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

void Process::SetRoot(BTree<Process*>*P)
{
	Root = P;
}

BTree<Process*>* Process::GetRoot()
{
	return Root;
}

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

void Process::ExcutionTimeNeeded(int& timeleft)
{
	CT--;
	TimeInRun++;
	timeleft = CT;
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

bool Process::operator==(const Process& p)
{
	if (PID == p.PID) return true;
	return false;
}

bool Process::operator<(const Process& other) const
{
	return CT > other.CT;
}

void Process::excute1TimeStep()
{
	CT--;
}

bool Process::MustbeTerminated()
{
	if (CT <= 0) return true;
	return false;
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

void Process::CalcStatistics(int&wt,int&rt,int&tt)
{
	wt = WT;
	rt = RT;
	tt = TT;
}
Process::~Process()
{
}

ostream& operator << (ostream& Out, Process* P) {
	Out << P->PID;
	return Out;
}

ostream& operator <= (ostream& Out, Process* P)
{
	Out << P->TT << "\t" << P->PID << "\t" << P->AT << "\t" << P->CT << "\t" << P->TotalIOD << "\t" << P->WT << "\t" << P->RT << "\t" << P->TRT;
	return Out;
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

bool Process::CreateForkList()
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