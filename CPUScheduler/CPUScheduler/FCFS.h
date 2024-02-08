#ifndef FCFS_H
#define FCFS_H
//====================================================================================================================//
//======================================================== CLASS INCLUDES ============================================//
//====================================================================================================================//
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

//Struct for IOs
struct KillList                     
{
	int killtime;
	int killid;
};
static LinkedList<KillList>* list = new LinkedList<KillList>;

//start of the class
class FCFS : public Processor
{
private:
	LinkedList<Process*>* RDY;

public:

	//Constructors
	FCFS();											            //Default constructor
	FCFS(int id);                                               //Non-Default constructor
	~FCFS();                                                    //Default Destructor

	//Scheduler Algo
	void ScheduleAlgo(int& CTS, int MigrationParameter, bool checkforRR);        //Overloaded Scheduler Algorithem for FCFS processors

	//Process Migration
	bool ProcessMigratonToRR(RR* receiver, int MaxW);    //Will migrate one process from RDY of FCFS to RDY of RR if that process has WT > MaxW

	//Process Forking 
	int GenerateRandom();                                       //Generate radom number from 1 to 100
	bool ProcessorCanFork(Process* P, int CTS, int ForkProb);   //Boolean to ask the scheduler to create the forked process of true

	//Process Orphan
	bool GenerateKillSigToChild(Process* P);

	//Process killing
	bool SearchForProcess(int id, Process*& p, int Curr);        
	int KillSignalID(int curr);
	bool KillAgain(int curr);
	void SetKillList(int id, int t);
	bool KillSignal(int curr);
	bool ProcIsRun(Process* p);
	void KillIsDone();

	//Work stealing
	bool StealProcess(Processor* p);
	void UpdateWT_RDY();

	//Rest of functions
	void InserttoRDY(Process* P);					            //Virtual function responsible for inserting a Process to RDY list
	void MoveFromBLKToRUN(Process* P);                          //Virtual function responsible for moving a Process from BLK to RDY list
	void PrintRDY();
	int getRDYCount();
	string returntypename();                                
	void SetKillListOrphan(int id, int k);
	bool IsIdle();                                                //Virtual overriden idle function
};
#endif