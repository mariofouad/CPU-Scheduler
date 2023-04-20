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
	isNew = true;
	isReady = false;
	isRunning = false;
	isBlocked = false;
	isTerminated = false;
	isOrphan = false;
}

void Process::SetNumberOfRequests(int n)
{
	ION = n;
	IO_R = new int[n];
	IO_D = new int[n];
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


Process::~Process()
{
}

ostream& operator << (ostream& Out, const Process& P) {
	Out << P.PID;
	return Out;
}


int Process::GetAT()
{
	return AT;


}

void Process::SetHamada(bool l)
{
	hamada = l;
}

bool Process::GetHamada()
{
	return hamada;
}