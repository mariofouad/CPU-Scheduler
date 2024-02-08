#ifndef ROUNDROBIN
#define ROUNDROBIN
//====================================================================================================================//
//======================================================== CLASS INCLUDES ============================================//
//====================================================================================================================//
#pragma once
#include "Processor.h"
#include "SJF.h"
#include "LinkedQueue.h"
#include <iostream>
using namespace std;
//====================================================================================================================//
//======================================================= CLASS FUNCTIONS ============================================//
//====================================================================================================================//
class RR : public Processor
{
private:
	LinkedQueue<Process*>* RDY;								            //RDY list for RR proceses
	int TMslice = 0;										            //Time Slice data will be set by the scheduler after reading the input file
	int tempSlice;											            //A variable that saves a temporary integer with the value og the TMslice
public:

	//Constructors
	RR();                                                                //Default constructor
	RR(int id);												             //ID initializer
	RR(int id , int TimeSlice);							              	 //ID , Time slice initializer 
	~RR();                                                               //Default Destructor

	//Scheduler Algo
	void ScheduleAlgo(int& CTS, int MigrationParameter, bool checkforSJF); //Overloaded Scheduler Algorithem for RR processors
	void SetTMslice(int timeslice);							             //Sets the time slice of RR processors
	void ResetTMslice();									             //A function resets TMslice  

	//Process Migration
	bool ProcessMigrationToSJF(Processor* reciever, int RTF, int slice); //Will migrate 1 process from the RDY queue of RR to SJF

	//Work Stealing
	bool StealProcess(Processor* p);                                      //Rest of functions
	bool IsIdle();
	int getRDYCount();
	void InserttoRDY(Process* P);							              //This function will insert a Process into that processor RDY list
	void PrintRDY();										              //Prints the data in the RDY list 
	string returntypename();	                                          //Returna a string with the type name which is RR
};
#endif