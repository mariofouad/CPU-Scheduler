#include "Scheduler.h"
//===================================================================================================================================//
//=========================================================== CLASS IMPLIMENTATION ==================================================//
//===================================================================================================================================//
Scheduler::Scheduler()
{
	UserInterface = new UI(this);
	NEW = new LinkedQueue<Process*>;
	BLK = new LinkedQueue<Process*>;
	TRM = new LinkedQueue<Process*>;
	ActualRUN = new LinkedQueue<Process*>;
	FCFS_Processors = new LinkedList<FCFS*>;
	SJF_Processors = new LinkedList<SJF*>;
	RR_Processors = new LinkedList<RR*>;
	//ForkTree = new BTree<Process*>;
	Treeptrs = new LinkedList<BTree<Process*>*>;
}

Scheduler::~Scheduler() 
{
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
	int SJFindex = 0;   //The index of SJF that RR processes will migrate to
	int RRindex = 0;    //The index of RR that FCFS process will migrate to
	while (!WorkisDone()) 
	{
		MoveFromNewToRdy();
		//=================================== HANDLING FCFS ==================================//
		for (int i = 0; i < FCFS_Count; i++)

		{   
			//Calling Scheduler Algo                                      
			FCFS* P = nullptr;
			Process* proc = nullptr;
			FCFS_Processors->Traversal(P, i);
			P->UpdateWT_RDY();
			P->ScheduleAlgo(CurrentTimestep,MaxW);
			//Handle BLK and TRM operation on RUN
			proc = P->GetRUN();
			if (P->IsBusy())                                                    //Busy? yes -> make checks needed : No-> nothing to be done
			{
				int timeleft = 0;                                               //there is requests to be done? true : No-> nothimg to be done          
				if (proc->MustBeBlocked(CurrentTimestep))                       //true-> (first IOreq time >= time in RUN? && no op done )-> req is done time in run=0
				{
					MoveToBlk(proc);
					proc->ExcutionTimeNeeded(timeleft);
					//P->RemTime(proc);
					P->KillRUN();                                               //mmken gedan ba3d ma tefred eno hai3mel el req yetrefed bara 3ashan 
					continue;
				}	                                                            //Iorequested? true -> move to BLK : false ->nothing to be done
				/////////////////////////////////////////////
				////////// Processes Migration 2 ////////////
				/////////////////////////////////////////////
				RR* firstRR = nullptr;
				RR_Processors->Traversal(firstRR, RRindex);
				while (P->ProcessMigratonToRR(firstRR, MaxW))
				{
					maxWcount++;
					P->ScheduleAlgo(CurrentTimestep, MaxW);
					RRindex++;
					if (RRindex == RR_Count) RRindex = 0;
					RR_Processors->Traversal(firstRR, RRindex);
				}

				proc->ExcutionTimeNeeded(timeleft);
				//P->DecrementET();
				if (timeleft <= 0 && MovetoTRM(proc))
				{
					//P->RemTime(proc);
					P->KillRUN();
				}
			}
			
			//Process Forking 
			ProcessForking(P);
			//Process Migration
		}
		KillSig();
		//====================================== HANDLING SJF PROCESSORS ===================================//	
		
			SJF* S = nullptr;
			Process* SP = nullptr;
			int j = 0;
		
			
			while (SJF_Processors->DeleteFirst(S))
			{
				/////////////////////////////////////////////
				////////// Calling Scheduler algo ///////////
				/////////////////////////////////////////////
				S->ScheduleAlgo(CurrentTimestep, 0);
				SP = S->GetRUN();
				if (S->IsBusy())
				{
					if (SP->MustBeBlocked(CurrentTimestep))
					{
						//S->RemTime(SP);
						MoveToBlk(SP);
						S->KillRUN();
					}
					else if (SP->MustbeTerminated() && MovetoTRM(SP))
					{
						//S->RemTime(SP);
						SP->TerminationTime(CurrentTimestep);
						S->KillRUN();
					}
				}
				SJF_Processors->InsertEnd(S);
					j++;
				if (j == SJF_Count) break;
			}

		
		//======================================= HANDLING RR PROCESSORS ===================================//				
		RR* R = nullptr;
		Process* p = nullptr;
		int i = 0;
		while (RR_Processors->DeleteFirst(R))
		{
			/////////////////////////////////////////////
			////////// Calling Scheduler algo ///////////
			/////////////////////////////////////////////
			R->ScheduleAlgo(CurrentTimestep, RTF);
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
					p->TerminationTime(CurrentTimestep);
					R->KillRUN();
				}
				/////////////////////////////////////////////
				/////////  Processes Migration 1 ////////////
				/////////////////////////////////////////////
				SJF* firstSJF = nullptr;
				SJF_Processors->Traversal(firstSJF, SJFindex);
				while (R->ProcessMigrationToSJF(firstSJF, RTF, TimeSliceRR))
				{
					RTFcount++;
					R->ScheduleAlgo(CurrentTimestep, RTF);
					SJFindex++;
					if (SJFindex == SJF_Count) SJFindex = 0;
					SJF_Processors->Traversal(firstSJF, SJFindex);
				}

			}
			RR_Processors->InsertEnd(R);
			i++;
			if (i == RR_Count) break;
		}
		
		//============================== HANDLING BLK list ============================//
		//me7tageen ne move men blk lel rdy lists law 5alst el IOduration beta3etha 
		//w da hait3mel b enna kol time step ne check eza kan ai process men el fel list 5alst el io duration
		//+ conditon en maikonsh 7asal 3aleeha ai operation tani f same current time step fa IsOpDone teb2a false 
		//law kolo true han move lel rdy list el expexted to finish eariler w call OpIsDone(CurrentTimeStep)
		//=============================================================================//
		for (int i = 0; i < Processor_count; i++)
		{
			AllProcessors[i]->CalcBusyTime();
			AllProcessors[i]->CalcIdleTime();
		}
		GetInterface()->UpdateInterface();
		CurrentTimestep++;
	}
	if (UserInterface->UpdateInterface() == 3)
	{
		UserInterface->SilentMode();
	}

	////////////////CALCULATIONS////////////////
	MaxWpercentage = (maxWcount / tempProc_count) * 100.00;
	RTFpercentage = (RTFcount / tempProc_count) * 100.00;
	//calloutputfile
	OutputFile();
	//=================================================================== END OF SCHEDULING ==========================================//
}
	
//================================================================ USED FUNCTIONS IN PHASE2 ======================================//

void Scheduler::ProcessForking(FCFS* P)
{
	Process* proc = P->GetRUN();
	if (P->ProcessorCanFork(proc, CurrentTimestep, ForkProb))
	{
		//Create the Forked Process and set the data of the child
		int ForkedCT = 0;
		ForkedCT = proc->GetCT();
		//proc->OpIsDone(CurrentTimestep);
		Process* ForkedProc = new Process(CurrentTimestep, ForkedProcID++, ForkedCT);
		tempProc_count++;
		ForkedProc->SetForkedProc();
		
		//Move to Shortest FCFS processor
		MoveToShFCFS(ForkedProc);

		//Handle The Parents Lists (Tree)
		BTree<Process*>* Tree = nullptr;  //Ptr of type tree is created 
		if (proc->CreateForkList())       //Law el Process ne2dar ne3mel meno forked list --> law parent w ma3mlsh fork abl keda
		{
			Tree = new BTree<Process*>;   //create el tree
			TreeCount++;
		
			Tree->insertBT(proc);         //Insert awel process PARENT to all coming processes of this list
			Tree->insertBT(ForkedProc);   //Insert the forked process in the tree  note will be inserted left as it is the fst node

			proc->SetRoot(Tree);
			ForkedProc->SetRoot(Tree);	  //insert the ptr to the tree to the forked process

			Treeptrs->InsertEnd(Tree);    //Insert ptr to the tree in the list of trees created
		}
		else if (proc->ForkFromParent())  //Law el process hia parent w bet3mel fork lelmara el tania
		{
			for (int i = 0; i < TreeCount; i++)  //loop on the tree ptrs
			{
				Treeptrs->DeleteFirst(Tree); 
				if (Tree == proc->GetRoot())  // law el ptr = ptr (mafrood eno nafs el adress)
				{

					Tree->insertBT(ForkedProc); // sa3etha ha insert el child
				}
				Treeptrs->InsertEnd(Tree);
			}
		}
		else
		{
			for (int i = 0; i < TreeCount; i++)  //loop on the tree ptrs
			{
				Treeptrs->DeleteFirst(Tree);
				if (Tree == proc->GetRoot())  // law el ptr = ptr (mafrood eno nafs el adress)
				{
					
					Tree->find(proc, ForkedProc); // sa3etha ha insert el child
				}
				Treeptrs->InsertEnd(Tree);
			}
		}
		proc->ForkOpIsDone();
	}
}

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
		AllProcessesTRT = AllProcessesTRT + p->proTRT();
		TRM->Enqueue(p);
		TRM_count++;
		return true;
	}
}

void Scheduler::MoveFromNewToRdy()
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

			MoveToRDY(ptemp);
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

bool Scheduler::MoveToRDY(Process* p)
{
	if (p == nullptr) return false;
	int i = ShortestQueue();

	AllProcessors[i]->AddTime(p);
	AllProcessors[i]->InserttoRDY(p);
	return true;
}

bool Scheduler::MoveToShFCFS(Process* p)
{
	if (p == nullptr) return false;
	int minprocessor = AllProcessors[0]->TotalTime();
	int minprocessori = 0;
	for (int i = 0; i < FCFS_Count; i++)
	{
		if (AllProcessors[i]->TotalTime() < minprocessor)
		{
			minprocessor = AllProcessors[i]->TotalTime();
			minprocessori = i;
		}
	}
	p->OpIsDone(CurrentTimestep);
	AllProcessors[minprocessori]->AddTime(p);
	AllProcessors[minprocessori]->InserttoRDY(p);
	return true;
}

void Scheduler::Work_stealing() {

}

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
		Load();
		for (int i = 0; i < Processor_count; i++)
		{
			outputfile << "p" << i << "=" << ProcessorLoad[i] << "%" << "," << " " << " ";
		}
		outputfile << "Processors Utiliz:" << std::endl;//need to be handeled
		Uti();
		for (int i = 0; i < Processor_count; i++)
		{
		/*	outputfile << "p" << i << "=" << ProcessorUti[i] << "%" << "," << " " << " "; exception ya mooza*/
		}
		outputfile << "Avg Utilization = " << AvgUti() << "%" << std::endl;//need to be handeled
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
		ProcessorLoad = new int[Processor_count];
		
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
		ForkedProcID = Proc_count + 1;
		tempProc_count = Proc_count;
		KillPID = new int[Proc_count];
		KillTime = new int[Proc_count];

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
		//Reading kill signals
		while (!inputfile.eof())
		{
			int kt;
			int kd;
			FCFS* P = NULL;
			std::getline(inputfile, line);
			std::stringstream KillLines(line);
			KillLines >> kt >> kd;
			P->SetKillList(kd, kt);
		}
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

void Scheduler::Load()
{
	for (int i = 0; i < Processor_count; i++)
	{
		int BS = AllProcessors[i]->GetBusyTime();
		ProcessorLoad[i] = BS / AllProcessesTRT;
	}
}

void Scheduler::Uti()
{/* exception ya mooza
	for (int i = 0; i < Processor_count; i++)
	{
		int BS = AllProcessors[i]->GetBusyTime();
		int IT = AllProcessors[i]->GetIdleTime();
		ProcessorUti[i] = BS / (BS + IT);
	}*/
}

int Scheduler::AvgUti()
{
	int sum = 0;
	for (int i = 0; i < Processor_count; i++)
	{
		//sum = ProcessorUti[i] + sum; exception ya mooza
	}
	int avg = sum / Processor_count;
	return avg;
}

void Scheduler::KillSig()
{
	FCFS* P = NULL;
	int kid = 0;
	bool multkillsig = false;

	while (!multkillsig)
	{

		if (P->KillSignal(CurrentTimestep))
		{
			kid = P->KillSignalID(CurrentTimestep);
			int j = 0;
			bool processfound = false;
			for (int i = 0; i < FCFS_Count; i++)
			{
				Process* ptemp = NULL;
				FCFS* temp = NULL;
				//processfound = false;
				FCFS_Processors->DeleteFirst(temp);
				FCFS_Processors->InsertEnd(temp);
				if (temp->SearchForProcess(kid, ptemp, CurrentTimestep))
				{
					processfound = true;
					MovetoTRM(ptemp);
					temp->KillIsDone();
					if (!(temp->ProcIsRun(ptemp)))
						temp->RemTime(ptemp);
					if (temp->ProcIsRun(ptemp))
					{
						temp->KillRUN();
					}
					if (P->KillAgain(CurrentTimestep))
					{
						multkillsig = true;
						//break;
					}
				}
			}
			if (processfound)
				continue;
			P->KillIsDone();
			if (P->KillAgain(CurrentTimestep))
			{
				multkillsig = true;
			}
		}
		else
			break;
	}
}

int Scheduler::ShortestQueue()
{
	int minprocessor = AllProcessors[0]->TotalTime();
	int minprocessori = 0;
	for (int i = 0; i < Processor_count; i++)
	{
		if (AllProcessors[i]->TotalTime() < minprocessor)
		{
			minprocessor = AllProcessors[i]->TotalTime();
			minprocessori = i;
		}
	}
	return minprocessori;
}

//================================================================================================================================//
//============================================================== END OF PHASE-2 ==================================================//
//================================================================================================================================//