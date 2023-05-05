#include "Processor.h"
//====================================================================================================================//
//======================================================== CLASS IMPLEMENTATION ======================================//
//====================================================================================================================//
Processor::Processor()                                       //Default constructor
{
	ID = 0;
	RDYcount = 0;
	RUN = nullptr;
}

Processor::Processor(int id)                                 //Non-Default constructor
{
	ID = id;
	RDYcount = 0;
	RUN = nullptr;
}

void ScheduleAlgo(int& CTS)                                //Virtual function to be overloaded in each child processor
{
}

bool Processor::operator<(const Processor& P)
{
	if (ExpectedTime < P.ExpectedTime) return true;
	return false;
}


Process* Processor::GetRUN()
{
	return RUN;
}

void Processor::PrintRUN()
{
	cout << RUN << "(P" << ID << ")";
}



bool Processor::IsBusy()
{
	if (RUN)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//bool Processor::IsIdeal()
//{
//	if (R)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

void Processor::KillRUN()
{
	RUN = nullptr;
}

Processor::~Processor()                                      //Default Destructor
{
}

ostream& operator<<(ostream& Out, Processor& P)
{
	Processor* P1 = &P;
	Out << "Processor " << P1->ID << " " << P1->returntypename() << ": " << P1->RDYcount << " RDY: ";
	P1->PrintRDY();
	return Out;
}

void Processor::AddTime(Process* p)
{
	ExpectedTime = ExpectedTime + p->GetCT();
}

int Processor::TotalTime()
{
	return ExpectedTime;
}

void Processor::RemTime(Process* p)
{
	ExpectedTime = ExpectedTime - p->GetCT();
}
