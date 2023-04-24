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
	/*isNew = true;
	isReady = false;
	isRunning = false;
	isBlocked = false;
	isTerminated = false;
	isOrphan = false;*/
	LastOpDone = AT;
	ReqDone = 0;
	TimeInRun = 0;
}

bool Process::IsNew(int& CTS)
{
	return (AT == CTS);
}

void Process::OpIsDone(int& CTS)
{
	LastOpDone = CTS;
}

bool Process::IsOpDone(int&CTS)
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

bool Process::IsIORequested(int& CTS)
{
	if (ION == 0)
	{
		return false;
	}
	else if ((IO_R[ReqDone] >= TimeInRun) && !IsOpDone(CTS))
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

bool Process::MustbeBlocked()
{
	for (int i = 0; i < ION; i++)
	{
		if (CT == tempCT - IO_R[i])
		{
			return true;
		}
	}
	return false;
}

Process::~Process()
{
}

ostream& operator << (ostream& Out, Process* P) {
	Out << P->PID;
	return Out;
}

int Process::GetAT()
{
	return AT;
}