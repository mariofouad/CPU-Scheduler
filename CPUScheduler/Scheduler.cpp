#include "Scheduler.h"
//===================================================================================================================================//
//=========================================================== CLASS IMPLIMENTATION ==================================================//
//===================================================================================================================================//
Scheduler::Scheduler() {
	UserInterface = new UI(this);
	NEW = new LinkedQueue<Process*>;
	BLK = new LinkedQueue<Process*>;
	TRM = new LinkedQueue<Process*>;
	ActualRUN = new LinkedQueue<Process*>;
	FCFS_Processors = new LinkedList<FCFS*>;
	SJF_Processors = new LinkedList<SJF*>;
	RR_Processors = new LinkedList<RR*>;
}

Scheduler::~Scheduler() {
	delete NEW, BLK, TRM, FCFS_Processors, RR_Processors, SJF_Processors, AllProcessors, ActualRUN, UserInterface;
}
//===================================================================================================================================//
//================================================================= PHASE-2 =========================================================//
//===================================================================================================================================//
void Scheduler::SIMULATOR()
{

	//=================================== READ FILE ==================================//
	int id = 1;
	ReadFile();
	for (int i = 0; i < FCFS_Count; i++)
	{
		FCFS* P = new FCFS(id++);
		Processor* Pproc = P;
		FCFS_Processors->InsertEnd(P);
		AllProcessors[i] = P;
	}
	for (int i = 0; i < SJF_Count; i++)
	{
		SJF* P = new SJF(id++);
		Processor* Pproc = P;
		SJF_Processors->InsertEnd(P);
		AllProcessors[FCFS_Count+i] = P;
	}
	for (int i = 0; i < RR_Count; i++)
	{
		RR* P = new RR(id++, TimeSliceRR);
		Processor* Pproc = P;
		RR_Processors->InsertEnd(P);
		AllProcessors[FCFS_Count + SJF_Count + i] = P;
	}
	//===========================================================================================================================//
	while (!WorkisDone()) 
	{
		MoveFromNewToRdy();
		//=================================== HANDLING FCFS ==================================//
		for (int i = 0; i < FCFS_Count; i++)
		{   /////////////////////////////////////////////
			/////////// Calling Schedule algo ///////////
			/////////////////////////////////////////////                                      
			FCFS* P = nullptr;
			Process* proc = nullptr;
			FCFS_Processors->Traversal(P, i);
			P->ScheduleAlgo(CurrentTimestep);         
			proc = P->GetRUN();
			if (P->IsBusy())
			{
				ActualRUN->Enqueue(proc);
				ActualRUNcount++;
			}
			if (P->IsBusy())                                                    //Busy? yes -> make checks needed : No-> nothing to be done
			{
				int timeleft = 0;                                               //there is requests to be done? true : No-> nothimg to be done          
				if (proc->MustBeBlocked(CurrentTimestep))                       //true-> (first IOreq time >= time in RUN? && no op done )-> req is done time in run=0
				{
					MoveToBlk(proc);
					proc->ExcutionTimeNeeded(timeleft);
					P->KillRUN();                                               //mmken gedan ba3d ma tefred eno hai3mel el req yetrefed bara 3ashan 
					continue;
				}	                                                            //Iorequested? true -> move to BLK : false ->nothing to be done
				proc->ExcutionTimeNeeded(timeleft);
				if (timeleft <= 0 && MovetoTRM(proc))
				{
					P->KillRUN();
				}
			}
			/////////////////////////////////////////////
			////////// Processes Migration 2 ////////////
			/////////////////////////////////////////////    

			
		}
		//====================================== HANDLING SJF PROCESSORS ===================================//	
		for (int i = 0; i < SJF_Count; i++)
		{
			SJF* P = nullptr;
			SJF_Processors->Traversal(P, i);
			P->ScheduleAlgo(CurrentTimestep);

		}
		//======================================= HANDLING RR PROCESSORS ===================================//	
		for (int i = 0; i < RR_Count; i++)
		{
			/////////////////////////////////////////////
			/////////// Calling Schedule algo ///////////
			/////////////////////////////////////////////
			RR* R = nullptr;
			Process* p;
			RR_Processors->Traversal(R, i);
			R->ScheduleAlgo(CurrentTimestep);
			p = R->GetRUN();
			if (R->IsBusy())
			{
				if (p->MustBeBlocked(CurrentTimestep))
				{
					MoveToBlk(p);
					R->KillRUN();
				}
				else if (p->MustbeTerminated() && MovetoTRM(p))
				{
					R->KillRUN();
				}
			}
			/////////////////////////////////////////////
			////////// Processes Migration  /////////////
			/////////////////////////////////////////////
			/*SJF* firstSJF = nullptr;
			int a = 0;
			SJF_Processors->Traversal(firstSJF, a);
			if (R->ProcessMigration(firstSJF, RTF))
			{
				RTFcount++;
			}*/
		}
		
		//============================== HANDLING BLK list ============================//
		//me7tageen ne move men blk lel rdy lists law 5alst el IOduration beta3etha 
		//w da hait3mel b enna kol time step ne check eza kan ai process men el fel list 5alst el io duration
		//+ conditon en maikonsh 7asal 3aleeha ai operation tani f same current time step fa IsOpDone teb2a false 
		//law kolo true han move lel rdy list el expexted to finish eariler w call OpIsDone(CurrentTimeStep)
		//=============================================================================//
		/*Process* ptemp = nullptr;
		int temp = ActualRUNcount;
		for (int i = 0; i < temp; i++)
		{
			ActualRUN->Dequeue(ptemp);
			if (ptemp->MustbeBlocked())
			{
				MoveToBlk(ptemp);
				ActualRUNcount++;
			}
			else if (ptemp->MustbeTerminated())
			{
				MovetoTRM(ptemp);
				ActualRUNcount++;
			}
			else 
			{
				ActualRUN->Enqueue(ptemp);
				ActualRUNcount++;
			}
			
		}*/
		GetInterface()->UpdateInterface();
		CurrentTimestep++;
	}
	//=================================================================== END OF SCHEDULING ==========================================//
}
	
//================================================================ USED FUNCTIONS IN PHASE2 ======================================//
bool Scheduler::MoveToBlk(Process* p)
{
	if (p == nullptr || p->IsOpDone(CurrentTimestep))
	{
		return false;
	}
	else
	{
		p->OpIsDone(CurrentTimestep);
		BLK->Enqueue(p);
		BLK_count++;
		return true;
	}
} 

bool Scheduler::MovetoTRM(Process* p)
{
	if (p == nullptr || p->IsOpDone(CurrentTimestep))
	{
		return false;
	}
	else
	{
		p->OpIsDone(CurrentTimestep);
		p->TerminationTime(CurrentTimestep);
		TRM->Enqueue(p);
		TRM_count++;
		return true;
	}
}

void Scheduler::MoveFromNewToRdy()        //Need to be editted enna newadi lel rdy el expected to finish earliear
{
	Process* ptemp = nullptr;
	FCFS* Ftemp = nullptr;
	SJF* Stemp = nullptr;
	RR* Rtemp = nullptr;
	while (!(NEW->IsEmpty()))
	{
		if (ProcessorI == (FCFS_Count + SJF_Count + RR_Count)) { ProcessorI = 0; }
		NEW->peek(ptemp);
		if (ptemp->GetAT() == CurrentTimestep)
		{
			NEW->Dequeue(ptemp);
			Proc_count--;

			if (ProcessorI < FCFS_Count)
			{
				FCFS_Processors->Traversal(Ftemp, ProcessorI);
				Ftemp->InserttoRDY(ptemp);
			}

			if (ProcessorI >= (FCFS_Count) && ProcessorI < (FCFS_Count + SJF_Count))
			{
				int is = ProcessorI - FCFS_Count;
				SJF_Processors->Traversal(Stemp, is);
				Stemp->InserttoRDY(ptemp);

			}

			if (ProcessorI >= (SJF_Count + FCFS_Count) && ProcessorI < (FCFS_Count + SJF_Count + RR_Count))
			{
				int ir = ProcessorI - SJF_Count - FCFS_Count;
				RR_Processors->Traversal(Rtemp, ir);
				Rtemp->InserttoRDY(ptemp);
			}
			ProcessorI++;
		}
		else
		{
			break;
		}
	}
}

bool Scheduler::MoveFromBLKToRDY()                                  //Me7taga tet3adel 3ashan newadi be condition el expected to finish
{
	srand(time(0));                                                 // seed the random number generator with current time
	int random_num1 = rand() % 100 + 1;                              // generate a random number between 1 and 100
	Process* p;
	BLK->peek(p);
	if (random_num1 < 10 && !BLK->IsEmpty() && !p->IsOpDone(CurrentTimestep))
	{
		BLK->Dequeue(p);
		BLK_count--;
		p->OpIsDone(CurrentTimestep);
		MoveToRDY(p);
		return true;
	}
	return false;
}

bool Scheduler::MoveToRDY(Process* p)                       // me7taga tet3adel law hanmove men el blk bel shortest list(expected to finish earlier)
{
	if (p == nullptr) return false;
	srand(time(0));   // seed the random number generator with current time
	int ProcessorI = (rand() % Processor_count) + 1;
	int c = 0;
	FCFS* Ftemp = nullptr;
	SJF* Stemp = nullptr;
	RR* Rtemp = nullptr;
	if (ProcessorI < FCFS_Count)
	{
		FCFS_Processors->Traversal(Ftemp, ProcessorI);
		Ftemp->InserttoRDY(p);
		c++;
	}
	else if (ProcessorI >= (FCFS_Count) && ProcessorI < (FCFS_Count + SJF_Count))
	{
		int is = ProcessorI - FCFS_Count;
		SJF_Processors->Traversal(Stemp, is);
		Stemp->InserttoRDY(p);
		c++;
	}
	else if (ProcessorI >= (SJF_Count + FCFS_Count) && ProcessorI < (FCFS_Count + SJF_Count + RR_Count))
	{
		int ir = ProcessorI - SJF_Count - FCFS_Count;
		RR_Processors->Traversal(Rtemp, ir);
		Rtemp->InserttoRDY(p);
		c++;
	}
	return (c != 0);
}

//void Scheduler::Process_Migartion(Processor* p1, Processor* p2) {}

void Scheduler::Work_stealing() {}

bool Scheduler::WorkisDone()
{
	if (NEW->IsEmpty() && BLK->IsEmpty() && TRM_count == tempProc_count) return true;
	return false;
}

UI* Scheduler::GetInterface() {
	return UserInterface;
}

void Scheduler::setFileName(string& fname) {
	FileName = fname;
}

bool Scheduler::FileisFound() {
	ifstream inputfile(FileName);
	if (inputfile.is_open()) {
		return true;
	}
	else {
		return false;
	}
}

string Scheduler::OutputFileName() //here make the user enters the name of the file and I will call the function in output file function
{
	return "TestOutputFile.txt";
}

void Scheduler::OutputFile()
{	
	std::ofstream outputfile(OutputFileName());
	if (outputfile.is_open())
	{
		Process* P = new Process;
		int wt = 0, rt = 0, tt = 0;
		int AvgWT = 0, AvgRT = 0, AvgTT = 0;
		outputfile << "TT" << "\t" << "PID" << "\t" << "AT" << "\t" << "CT" << "\t" << "IO_D" << "\t" << "WT" << "\t" << "RT" << "\t" << "TRT" << std::endl;
		while (TRM->Dequeue(P))
		{
			int wtp = 0, rtp = 0, ttp = 0;
			outputfile <= P;
			outputfile << std::endl;
			P->CalcStatistics(wtp, rtp, ttp);
			wt += wtp;
			rt += rtp;
			tt += ttp;
		}
		outputfile << std::endl;
		outputfile << "Processes: " << tempProc_count << endl;
		outputfile << "Avg WT = " << (AvgWT = wt / tempProc_count) << "," << "\t" << "Avg RT = " << (AvgRT = rt / tempProc_count) << "," << "\t" << "Avg TT = " << (AvgTT = tt / tempProc_count) << std::endl;
		outputfile << "Migration %: " << "RTF = " << RTFcount << "%," << "\t" << "MaxW = " << MaxW << "%" << std::endl; //need to be handeled
		outputfile << "Work Steal %:" << "%" << std::endl;//need to be handeled
		outputfile << "Forked Process:" << "%" << std::endl;//need to be handeled
		outputfile << "Killed Process:" << "%" << std::endl;//need to be handeled
		outputfile << std::endl;
		outputfile << "Processors: " << Processor_count << " [" << FCFS_Count << " FCFS, " << SJF_Count << " SJF, " << RR_Count << " RR" << "]" << std::endl;
		outputfile << "Processors Load:" << std::endl;//need to be handeled
		outputfile << "Processors Utiliz:" << std::endl;//need to be handeled
		outputfile << "Avg Utilization = " << "%" << std::endl;//need to be handeled
		outputfile.close();
	}
	else 
	{
		return;
	}
}

void Scheduler::ReadFile()
{
	std::string line;
	std::ifstream inputfile(FileName);
	if (inputfile.is_open())
	{
		//Fist line in text file
		std::getline(inputfile, line);
		std::stringstream firstline(line);
		firstline >> FCFS_Count >> SJF_Count >> RR_Count;
		Processor_count = FCFS_Count + SJF_Count + RR_Count;
		AllProcessors = new Processor*[Processor_count];

		//Second line in text file
		std::getline(inputfile, line);
		std::stringstream secondline(line);
		secondline >> TimeSliceRR;

		//Third line in text file
		std::getline(inputfile, line);
		stringstream thirdline(line);
		thirdline >> RTF >> MaxW >> STL >> ForkProb;

		//Fourth line in text file
		std::getline(inputfile, line);
		std::stringstream fourthline(line);
		fourthline >> Proc_count;
		tempProc_count = Proc_count;

		for (int i = 0; i < Proc_count; i++)
		{
			int AT, PID, CT, N, IOR=0, IOD=0;
			std::getline(inputfile, line);
			std::stringstream Processline(line);
			Processline >> AT >> PID >> CT >> N;
			Process* P=new Process(AT, PID, CT);
			P->SetNumberOfRequests(N);
			if (N != 0)
			{
				for (int i = 0; i < N; i++)
				{
					int IOR, IOD;
					char c;

					// read until the first digit is found
					while (Processline.get(c) && !isdigit(c));

					// if a digit is found, put it back and read the number


					Processline.putback(c);
					Processline >> IOR;

					// read until the second digit is found
					while (Processline.get(c) && !isdigit(c));

					// if a digit is found, put it back and read the number


					Processline.putback(c);
					Processline >> IOD;

					// set input data for the process
					P->SetInputData(IOR, IOD, i);

				}

			}
			NEW->Enqueue(P);
		}
		//Bfore last line in text file
		std::getline(inputfile, line);
		std::stringstream Beforelastline(line);
		Beforelastline >> KillPID;

		//Last line in text file
		std::getline(inputfile, line);
		std::stringstream Lastline(line);
		Lastline >> KillTime;
		inputfile.close();
	}
	else
	{
		return;
	}
}

void Scheduler::PrintWindow()
{
	std::cout << "===========================================================" << '\n';
	std::cout << "Current Timestep:" << CurrentTimestep << '\n';
	std::cout << "---------------------- RDY processes ----------------------" << '\n';
	for (int i = 0; i < FCFS_Count; i++)
	{
		FCFS* Ptemp = nullptr;
		FCFS_Processors->Traversal(Ptemp, i);
		std::cout << *Ptemp;
		std::cout << '\n';
	}
	for (int i = 0; i < SJF_Count; i++)
	{
		SJF* Stemp = nullptr;
		SJF_Processors->Traversal(Stemp, i);
		std::cout << *Stemp;
		std::cout << '\n';
	}
	for (int i = 0; i < RR_Count; i++)
	{
		RR* Rtemp = nullptr;
		RR_Processors->Traversal(Rtemp, i);
		std::cout << *Rtemp;
		std::cout << '\n';
	}

	std::cout << "---------------------- BLK processes ----------------------" << '\n';
	std::cout << BLK_count << " BLK: ";
	BLK->print();
	std::cout << '\n';

	std::cout << "---------------------- RUN processes ----------------------" << '\n';
	int actur = 0;

	for (int i = 0; i < FCFS_Count; i++)
	{
		FCFS* P = nullptr;
		FCFS_Processors->Traversal(P, i);
		if (P->IsBusy()) actur++;
	}
	for (int i = 0; i < SJF_Count; i++)
	{
		SJF* P = nullptr;
		SJF_Processors->Traversal(P, i);
		if (P->IsBusy()) actur++;
	}
	for (int i = 0; i < RR_Count; i++)
	{
		RR* P = nullptr;
		RR_Processors->Traversal(P, i);
		if (P->IsBusy()) actur++;;
	}

	std::cout << actur << " RUN: ";

	for (int i = 0; i < FCFS_Count; i++)
	{
		FCFS* P = nullptr;
		FCFS_Processors->Traversal(P, i);
		if (P->IsBusy())
		{
			P->PrintRUN();
			std::cout << ", ";
		}
	}
	for (int i = 0; i < SJF_Count; i++)
	{
		SJF* P = nullptr;
		SJF_Processors->Traversal(P, i);
		if (P->IsBusy())
		{
			P->PrintRUN();
			std::cout << ", ";
		}
	}
	for (int i = 0; i < RR_Count; i++)
	{
		RR* P = nullptr;
		RR_Processors->Traversal(P, i);
		if (P->IsBusy())
		{
			P->PrintRUN();
			std::cout << ", ";
		}
	}
	std::cout << '\n';
	std::cout << "---------------------- TRM processes ----------------------" << '\n';
	std::cout << TRM_count << " TRM: ";
	TRM->print();
	std::cout << '\n';
}
//================================================================================================================================//
//============================================================== END OF PHASE-2 ==================================================//
//================================================================================================================================//

//================================================================================================================================//
//================================================================= PHASE-1 ======================================================//
//================================================================================================================================//
//void Scheduler::Phase1Simulator()
//{
//	//============================================================ READ_FILE =====================================================//
//	int id = 1;
//	ReadFile();
//	for (int i = 0; i < FCFS_Count; i++)
//	{
//		FCFS* P = new FCFS(id++);
//		Processor* Pproc = P;
//		FCFS_Processors->InsertEnd(P);
//		AllProcessors->add(Pproc);
//	}
//	for (int i = 0; i < SJF_Count; i++)
//	{
//		SJF* P = new SJF(id++);
//		Processor* Pproc = P;
//		SJF_Processors->InsertEnd(P);
//		AllProcessors->add(Pproc);
//	}
//	for (int i = 0; i < RR_Count; i++)
//	{
//		RR* P = new RR(id++);
//		Processor* Pproc = P;
//		RR_Processors->InsertEnd(P);
//		AllProcessors->add(Pproc);
//	}
//	while (!WorkisDone())
//	{
//		//============================================================= i  =========================================================//(DONE)
//		MoveFromNewToRdy();
//		//============================================================= ii  ========================================================//(DONE) 
//		DistToRUN();
//		//============================================================= iii ========================================================//(DONE)
//		Blk_Rdy_Trm();
//		//============================================================= iv =========================================================//(DONE)
//		MoveFromBLKToRDY();
//		//=============================================================  v  ========================================================//(DONE)
//		GetInterface()->UpdateInterface();
//		CurrentTimestep++;
//		//==========================================================================================================================//
//	}
//}

int Scheduler::GenerateRandom()
{
	srand(time(0));                                                 // seed the random number generator with current time
	int random_num = rand() % 100 + 1;
	if (random_num >= 1 && random_num <= 15)
	{
		return 1;
	}
	if (random_num >= 20 && random_num <= 30)
	{
		return 2;
	}
	if (random_num >= 50 && random_num <= 60)
	{
		return 3;
	}
	return 0;
}

//void Scheduler::SetActualRUN() {
//	int i = 1;
//	Processor* temp = nullptr;
//	while (AllProcessors->remove(temp)) {
//
//		if (temp->IsBusy())
//		{
//			ActualRUNcount++;
//		}
//		Process* prtemp = temp->GetRUN();
//		ActualRUN->Enqueue(prtemp);
//		AllProcessors->add(temp);
//		i++;
//		if (i == Processor_count) break;
//	}
//}

bool Scheduler::DistToRUN()
{
	int c = 0;
	bool done = true;
	for (int i = 0; i < Processor_count; i++)
	{
		if (i < FCFS_Count)
		{
			FCFS* Fproc = nullptr;
			FCFS_Processors->Traversal(Fproc, i);
			if (!Fproc->IsBusy() && !Fproc->IsIdeal())
			{
				done = Fproc->MoveFromRDYToRUN(CurrentTimestep);
				if (done) c++;
			}
		}

		if (i >= (FCFS_Count) && i < (FCFS_Count + SJF_Count))
		{
			int is = i - FCFS_Count;
			SJF* Sproc = nullptr;
			SJF_Processors->Traversal(Sproc, is);
			if (!Sproc->IsBusy() && !Sproc->IsIdeal())
			{
				done = Sproc->MoveFromRDYToRUN(CurrentTimestep);
				if (done) c++;
			}
		}

		if (i >= (SJF_Count + FCFS_Count) && i < (FCFS_Count + SJF_Count + RR_Count))
		{
			int ir = i - FCFS_Count - SJF_Count;
			RR* Rproc = nullptr;
			RR_Processors->Traversal(Rproc, ir);
			if (!Rproc->IsBusy() && !Rproc->IsIdeal())
			{
				done = Rproc->MoveFromRDYToRUN(CurrentTimestep);
				if (done) c++;
			}
		}
	}
	return (c != 0);
}

//bool Scheduler::Blk_Rdy_Trm()
//{
//	int c = 0;
//	Process* Rtemp = nullptr;
//	Processor* Atemp = nullptr;
//	int i = 0;
//	while (AllProcessors->remove(Atemp))
//	{
//		Rtemp = Atemp->GetRUN();
//		if (Rtemp == nullptr)
//		{
//		}
//		else if (!Rtemp->IsOpDone(CurrentTimestep))
//		{
//			if (GenerateRandom() == 1)
//			{
//				MoveToBlk(Rtemp);
//				Atemp->KillRUN();
//				c++;
//			}
//			if (GenerateRandom() == 2)
//			{
//				MoveToRDY(Rtemp);
//				Atemp->KillRUN();
//				c++;
//			}
//			if (GenerateRandom() == 3)
//			{
//				MovetoTRM(Rtemp);
//				Atemp->KillRUN();
//				c++;
//			}
//		}
//		AllProcessors->add(Atemp);
//		i++;
//		if (i == Processor_count) break;
//	}
//	return (c != 0);
//}

bool Scheduler::KillFromFCFS()
{
	srand(time(0));
	int random_num2 = rand() % tempProc_count + 1;
	Process pfind(0, random_num2, 0);
	for (int i = 0; i < FCFS_Count; i++)
	{
		FCFS* temp;
		FCFS_Processors->Traversal(temp, i);
		if (temp->ProcIsFound(&pfind) && !pfind.IsOpDone(CurrentTimestep))
		{
			pfind.OpIsDone(CurrentTimestep);
			MovetoTRM(&pfind);
			return true;
		}
	}
	return false;
}