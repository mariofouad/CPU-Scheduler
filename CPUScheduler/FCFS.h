#pragma once
#include "Processor.h"
#include "LinkedList.h"
#include <iostream>
#include <cstdlib> // required for rand() function
#include <ctime>   // required for time() function
using namespace std;
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//

class FCFS : public Processor
{
private:

	LinkedList<Process*>* RDY;

public:

	FCFS();											  //Default constructor

	FCFS(int id);

	void ScheduleAlgo(int& CTS);        //Overloaded Scheduler Algorithem for FCFS processors

	void InserttoRDY(Process* P);					  //Virtual function responsible for inserting a Process to RDY list

	bool MoveFromRDYToRUN(int& CTS);

	void MoveFromBLKToRUN(Process* P);                //Virtual function responsible for moving a Process from BLK to RDY list

	void PrintRDY();

	bool IsIdeal();

	string returntypename();

	int GenerateRandom();

	bool ProcessorCanFork(Process*P, int CTS, int ForkProb);

	bool ProcIsFound(Process* p);

	bool SearchForProcess(int id, Process*& p);

	~FCFS();                                                 //Default Destructor
};
