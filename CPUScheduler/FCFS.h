#pragma once
#include "Processor.h"
#include "LinkedList.h"
#include <iostream>
#include <cstdlib> // required for rand() function
#include <ctime>   // required for time() function
#include "LinkedQueue.h"
#include "RR.h"
using namespace std;
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//


struct KillList
{
	int killtime;
	int killid;
};

static LinkedQueue<KillList>* list = new LinkedQueue<KillList>;


class FCFS : public Processor
{
private:

	LinkedList<Process*>* RDY;

public:

	FCFS();											  //Default constructor

	FCFS(int id);

	void ScheduleAlgo(int& CTS, int MigrationParameter);        //Overloaded Scheduler Algorithem for FCFS processors

	void InserttoRDY(Process* P);					  //Virtual function responsible for inserting a Process to RDY list

	bool MoveFromRDYToRUN(int& CTS);

	void MoveFromBLKToRUN(Process* P);                //Virtual function responsible for moving a Process from BLK to RDY list

	void PrintRDY();

	bool IsIdeal();

	string returntypename();

	int GenerateRandom();

	bool ProcessorCanFork(Process*P, int CTS, int ForkProb);

	bool ProcIsFound(Process* p);

	bool SearchForProcess(int id, Process*& p, int Curr);

	int KillSignalID(int curr);

	bool KillAgain(int curr);

	void SetKillList(int id, int t);

	bool KillSignal(int curr);

	void UpdateWT_RDY();
	bool ProcessMigratonToRR(RR* receiver, int MaxW);  //Will migrate one process from RDY of FCFS to RDY of RR if that process has WT > MaxW

	bool ProcIsRun(Process* p);

	void KillIsDone();


	~FCFS();                                                 //Default Destructor
};
