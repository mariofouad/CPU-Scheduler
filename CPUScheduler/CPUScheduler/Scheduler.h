
#pragma once
//====================================================================================================================//
//======================================================== CLASS INCLUDES ============================================//
//====================================================================================================================//
#include"LinkedList.h"
#include"LinkedQueue.h"                                    //DATA_STRUCTURES INCLUDES
#include"SL_PriorityQueue.h"
#include"BTree.h"
//====================================================================================================================//
#include <fstream>                           
#include <iostream>                                         //READ_FILE INCLUDES
#include <sstream>
#include <string>
using namespace std;
//====================================================================================================================//
#include <cstdlib>
#include <ctime>                                            //RANDOM_NUMBER_GENERATOR_INCLUDES
//====================================================================================================================//
#include "UI.h"
#include "Processor.h"                                      //HEADERS_INCLUDES
#include "Process.h"
#include "FCFS.h"
#include "SJF.h"
#include "RR.h"
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//
class Scheduler 
{
private:
	UI* UserInterface;				                         //An object from UI so we can make scheduler call it and do the output and input
	LinkedQueue<Process*>* NEW = nullptr;				     //A container for all the processes just before their arrival to CPU
	int Proc_count = 0;				                         //Total number of processes in NEW before anything occurs 
	int tempProc_count = 0;
	LinkedQueue<Process*>* BLK = nullptr;				     //A container for all processses in blocked state
	int BLK_count = 0;
	LinkedQueue<Process*>* TRM = nullptr;				     //A container for all terminated processes
	int TRM_count = 0;
	LinkedQueue<Process*>* ActualRUN = nullptr;
	int ActualRUNcount = 0;
	string FileName;										 //Input file name msh mot2ked han7tago wala laa
	int CurrentTimestep = 1;			                     //a variable that returns the current timestep        
	LinkedList<FCFS*>* FCFS_Processors = nullptr;			 //Container of First Come First Serve Processors  
	int FCFS_Count = 0;				                         //Number of FCFS processors
	LinkedList<SJF*>* SJF_Processors = nullptr;				 //Container of Short Job First Processors
	int SJF_Count = 0;				                         //Number of SJB processors
	LinkedList<RR*>* RR_Processors = nullptr;				 //Container of Round Robin Processors
	int RR_Count = 0;					                     //Number of RR_count
	int MaxW = 0;						                     //Data needed to do the process migration would be provided from the input file
	int RTF = 0;						                     //Data needed to do the process migration would be provided from the input file
	Processor** AllProcessors;
	int TimeSliceRR = 0;
	int STL = 0;
	int ForkProb = 0;
	string OutputFileName;									 //The Output file name will be set by the user from the UI
	int *KillPID ;
	int *KillTime ;
	int killindex = 0;
	int Processor_count = 0;
	int ProcessorI = 0;
	int* tempcurrenttimestep = &CurrentTimestep;
	int AllProcessesTRT = 0;
	int* ProcessorLoad;
	LinkedList<BTree<Process*>*>* Treeptrs = nullptr;
	int TreeCount=0;
	int* ProcessorUti;
	double RTFpercentage = 0;
	double MaxWpercentage = 0;
	int ShortestIndex = 0;
	int LongestIndex = 0;
	SJF* ShortestSJF = nullptr;
	RR* ShortestRR = nullptr;
	int ShortestFCFS = 0;
	int RTFcount = 0;
	int maxWcount = 0;
	int ForkedProcID = 0;
	int ForkedCount = 0;
	int STL_Perc = 0;
	int STL_Count = 0;
	int AvgWT = 0;
	int AvgRT = 0;
	int AvgTT = 0;
	int TotalWT = 0;
	int TotalRT = 0;
	int TotalTT = 0;
	int ForkedPerc = 0;
	int KillCount = 0;
	int KillPerc = 0;
	bool CheckForRR = false;
	bool CheckForSJF = false;

public:

	Scheduler();

	UI* GetInterface();										 //returns a pointer form UI 

	void setFileName(string& fname);					     //used to set the inputfile name so that scheduler go opens it and reads the data from it

	bool FileisFound();										 //Returns true if file is found

	void SetOutputFName(string name);

	void OutputFile();

	void ReadFile();										 //responsible for reading the data needed from the input file

	void PrintWindow();										 //this function would be called from the UI using a pointer from scheduler because Scheduler is the only class that should had access over data needed

	void SIMULATOR();										 //The real simulator that will do real scheduling						

	void Work_stealing();									 //will manage the work stealing

	bool WorkisDone();                                       //returns true if all processes are terminated		

	void MoveFromNewToRdy();								 //This function is responsible for distributimg NEW processes into RDY lists of each processor

	bool MovetoTRM(Process* p);							     //This function will terminate a process

	bool MoveToRDY(Process* p);								 //This function will move a process into RDY state in a processor

	bool MoveToBlk(Process* p);								 //This function will move a process to the BLK state

	void MoveFromBLKToRDY();

	//Process Forking
	void ProcessForking(FCFS* P);

	//Process Orphan
	void ProcessOrphan(Process* P);

	void Load();

	void Uti();

	int AvgUti();

	bool MoveToShFCFS(Process* p);

	void KillSig();

	int ShortestQueue();
	int LongestQueue();

	bool ShortestSteal();
	bool LongestSteal();

	void SetShortestSJF();
	void SetShortestRR();

	~Scheduler();
};