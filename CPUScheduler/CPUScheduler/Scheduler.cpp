#include "Scheduler.h"
//===================================================================================================================================//
//=========================================================== CLASS IMPLIMENTATION ==================================================//
//===================================================================================================================================//
Scheduler::Scheduler()                      //Scheduler constructor where we create all list
{
	UserInterface = new UI(this);
	NEW = new LinkedQueue<Process*>;
	BLK = new LinkedQueue<Process*>;
	TRM = new LinkedQueue<Process*>;
	ActualRUN = new LinkedQueue<Process*>;
	FCFS_Processors = new LinkedList<FCFS*>;
	SJF_Processors = new LinkedList<SJF*>;
	RR_Processors = new LinkedList<RR*>;
	Treeptrs = new LinkedList<BTree<Process*>*>;
}

Scheduler::~Scheduler()                     //Scheduler destructor where we delete all dynamic allocated lists
{
	delete NEW, BLK,TRM, FCFS_Processors, RR_Processors, SJF_Processors, AllProcessors, ActualRUN, UserInterface,Treeptrs;
}
//===================================================================================================================================//
//================================================================= PHASE-2 =========================================================//
//===================================================================================================================================//
void Scheduler::SIMULATOR()
{
	//============================================================ READ FILE ========================================================//
	
	//Calling Read file function and insert all Processors
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
	//============================================================== SCHEDULING ======================================================//
	while (!WorkisDone()) 
	{
		if (Processor_count == 0)
			break;
		MoveFromNewToRdy();
		SetShortestSJF();
		SetShortestRR();
		//=================================== HANDLING FCFS ==================================//
		//Loop on FCFS Processors
		for (int i = 0; i < FCFS_Count; i++)
		{   
			//Creating needed pointers                                    
			FCFS* P = nullptr;
			Process* proc = nullptr;

			//Loop on FCFS processor by Delete fst
			FCFS_Processors->DeleteFirst(P);
			
			//Process Migration update the waiting time
			P->UpdateWT_RDY();

			//Calling Scheduler Algo 
			P->ScheduleAlgo(CurrentTimestep, MaxW, CheckForRR);

			//Handle BLK and TRM operation on RUN
			proc = P->GetRUN();
			if (P->IsBusy())                                                    
			{
				          
				//=========== Process Migration 2 ============//
				while (P->ProcessMigratonToRR(ShortestRR, MaxW))
				{
					maxWcount++;
					P->ScheduleAlgo(CurrentTimestep, MaxW, CheckForRR);
					SetShortestRR();
				}
				//Check on BLK condition
				if (proc->MustBeBlocked(CurrentTimestep))                       
				{
					MoveToBlk(proc);                            //Moving the process in RUN to BLK list               
					P->KillRUN();                               //Remove the process from RUN                                            
				}	                                                            
				//Check on the termination condition
				if (proc->MustbeTerminated() && MovetoTRM(proc))
				{
					if (P->GenerateKillSigToChild(proc))
					{
						ProcessOrphan(proc);
					}
					P->KillRUN();                                 //Remove the process from RUN
				}
			}
			
			//Process Forking 
			ProcessForking(P);

			//Return the FCFS processor back
			FCFS_Processors->InsertEnd(P);
		}

		//Process Killing
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
			S->ScheduleAlgo(CurrentTimestep, 0,false);
			SP = S->GetRUN();
			if (S->IsBusy())
			{
				if (SP->MustBeBlocked(CurrentTimestep))
				{
				
					MoveToBlk(SP);
					S->KillRUN();
				}
				else if (SP->MustbeTerminated() && MovetoTRM(SP))
				{		
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
			R->ScheduleAlgo(CurrentTimestep, RTF, CheckForRR);
			p = R->GetRUN();
			if (R->IsBusy())
			{
				if (p->MustbeTerminated() && MovetoTRM(p))
				{
					p->TerminationTime(CurrentTimestep);
					R->KillRUN();
				}
				//=========== Process Migration 1 ============//
				while (R->ProcessMigrationToSJF(ShortestSJF, RTF, TimeSliceRR))
				{
					RTFcount++;
					R->ScheduleAlgo(CurrentTimestep, RTF, CheckForSJF);
					SetShortestSJF();
				}
				if (p->MustBeBlocked(CurrentTimestep))
				{
					MoveToBlk(p);
					R->KillRUN();
				}
			}
			RR_Processors->InsertEnd(R);
			i++;
			if (i == RR_Count) break;
		}
		//============================== Work_Stealing ============================//
		Work_stealing();
		//============================== HANDLING BLK list ============================//
		MoveFromBLKToRDY();
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
	UserInterface->OutputPart();

	//============================= CALCULATIONS ===========================//
	MaxWpercentage = (static_cast<double>(maxWcount) / tempProc_count) * 100.00;
	RTFpercentage = (static_cast<double>(RTFcount) / tempProc_count) * 100.00;
	Process* P;
	int wtp, rtp, ttp = 0;
	for (int i = 0; i < TRM_count; i++)
	{
		TRM->Dequeue(P);
		P->CalcStatistics(wtp, rtp, ttp);
		TotalWT += wtp;
		TotalRT += rtp;
		TotalTT += ttp;
		TRM->Enqueue(P);
	}
	AvgWT = TotalWT / tempProc_count;
	AvgRT = TotalRT / tempProc_count;
	AvgTT = TotalTT / tempProc_count;
	ForkedPerc = ((double(ForkedCount) / tempProc_count) * 100);
	STL_Perc = (double(STL_Count) / tempProc_count) * 100;
	KillPerc = (double(KillCount) / tempProc_count) * 100;
	Load();
	Uti();
	
	//calloutputfile
	OutputFile();
	//=============================================================== END OF SCHEDULING ==========================================//
}
	
//================================================================ USED FUNCTIONS IN PHASE2 ======================================//

//=================================================================== PROCESS FORKING ============================================//
void Scheduler::ProcessForking(FCFS* P)
{
	Process* proc = P->GetRUN();
	if (P->ProcessorCanFork(proc, CurrentTimestep, ForkProb) && !proc->GetIsOrphaned())
	{
		//Create the Forked Process and set the data of the child
		int ForkedCT = 0;
		ForkedCT = proc->GetCT();
		
		Process* ForkedProc = new Process(CurrentTimestep, ForkedProcID++, ForkedCT);
		tempProc_count++;
		Proc_count++;
		ForkedCount++;
		ForkedProc->SetForkedProc();
		
		//Move to Shortest FCFS processor
		MoveToShFCFS(ForkedProc);

		//Handle The Parents Lists (Tree)
		BTree<Process*>* Tree = nullptr;  //Ptr of type tree is created 
		if (proc->FstForkFromParent())       //Law el Process ne2dar ne3mel meno forked list --> law parent w ma3mlsh fork abl keda
		{
			proc->SetChild(ForkedProc);   //Pointing to the Child
			ForkedProc->SetParent(proc);  //Pointing to the Parent 

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
			proc->SetChild(ForkedProc);   //Pointing to the Child
			ForkedProc->SetParent(proc);  //Pointing to the Parent 

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
			proc->SetChild(ForkedProc);   //Pointing to the Child
			ForkedProc->SetParent(proc);  //Pointing to the Parent 

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
//============================================================== PROCESS ORPHAN ========================================//
void Scheduler::ProcessOrphan(Process * P)
{
	if (!P->GetLeft() && !P->GetRight())
	{
		FCFS* tempProcessor = nullptr;
		tempProcessor->SetKillListOrphan(P->ID(), CurrentTimestep);
		P->SetIsOrphaned();
	}
	else if (P->GetLeft() && !P->GetRight())
	{
		ProcessOrphan(P->GetLeft());
	}
	else if (P->GetLeft() && P->GetRight())
	{
		FCFS* tempProcessor = nullptr;
		tempProcessor->SetKillListOrphan(P->ID(), CurrentTimestep);
		ProcessOrphan(P->GetLeft());
		ProcessOrphan(P->GetRight());
	}

}
//=======================================================================================================================//
bool Scheduler::MoveToBlk(Process* p)
{
	if (p == nullptr)
	{
		return false;
	}
	else
	{
		BLK->Enqueue(p);
		BLK_count++;
		return true;
	}
}

bool Scheduler::MovetoTRM(Process* p)
{
	if (p == nullptr)
	{
		return false;
	}
	else
	{
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

void Scheduler::MoveFromBLKToRDY()                                  
{
	if (!BLK->IsEmpty())
	{
		Process* TempProc;
		BLK->peek(TempProc);
		if (TempProc->MustRemovedFromBLK())
		{
			MoveToRDY(TempProc);	
			BLK->Dequeue(TempProc);
			BLK_count--;
		}
	}
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
	AllProcessors[minprocessori]->AddTime(p);
	AllProcessors[minprocessori]->InserttoRDY(p);
	return true;
}

void Scheduler::Work_stealing() 
{

	if (!LongestSteal() || !ShortestSteal())
	{
		return;
	}
	int OldLindex = LongestIndex;
	int OldSiindex = ShortestIndex;
	Processor* LQF = AllProcessors[LongestIndex];
	Processor* SQF = AllProcessors[ShortestIndex];
	if (STL == 0)
	{
		return;
	}
	while (((((float)(LQF->TotalTime() - SQF->TotalTime()) / LQF->TotalTime() >= 0.4)) && CurrentTimestep % STL == 0))
	{
		if (!(LQF->StealProcess(SQF)))
		{
			return;
		}

		STL_Count++;
		LongestSteal();
		ShortestSteal();
		LQF = AllProcessors[LongestIndex];
		SQF = AllProcessors[ShortestIndex];
		if (!LongestSteal() || !ShortestSteal())
		{
			return;
		}
		if (OldLindex == ShortestIndex && OldSiindex == LongestIndex)
		{
			return;
		}
	}
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

void Scheduler::SetOutputFName(string name)
{
	OutputFileName = name;
}

void Scheduler::OutputFile()
{	
	if (Processor_count == 0)
	{
		cout << "No Processors Available!" << endl;
		return;
	}
	std::ofstream outputfile(OutputFileName);
	if (outputfile.is_open())
	{
		Process* P = new Process;
		outputfile << "TT" << "\t" << "PID" << "\t" << "AT" << "\t" << "CT" << "\t" << "IO_D" << "\t" << "WT" << "\t" << "RT" << "\t" << "TRT" << std::endl;
		while (TRM->Dequeue(P))
		{
			outputfile <= P;
			outputfile << std::endl;
		}
		outputfile << std::endl;
		outputfile << "Processes: " << tempProc_count << endl;
		outputfile << "Avg WT = " << AvgWT << "," << "\t" << "Avg RT = " << AvgRT << "," << "\t" << "Avg TT = " << AvgTT << std::endl;
		outputfile << "Migration %: " << "RTF = " << RTFpercentage << "%," << "\t" << "MaxW = " << MaxWpercentage << "%" << std::endl;
		outputfile << "Work Steal %: " << STL_Perc << "%" << std::endl;
		outputfile << "Forked Process: " << ForkedPerc << "%" << std::endl;
		outputfile << "Killed Process: " << KillPerc << "%" << std::endl;
		outputfile << std::endl;
		outputfile << "Processors: " << Processor_count << " [" << FCFS_Count << " FCFS, " << SJF_Count << " SJF, " << RR_Count << " RR" << "]" << std::endl;
		outputfile << "Processors Load:"<< std::endl;
		for (int i = 0; i < Processor_count; i++)
		{
			outputfile << "p" << i+1 << "=" << ProcessorLoad[i] << "%" << "," << " " << " ";
		}
		outputfile << endl;
		outputfile << "Processors Utiliz:" << std::endl;
		for (int i = 0; i < Processor_count; i++)
		{
			outputfile << "p" << i+1 << "=" << ProcessorUti[i] << "%" << "," << " " << " ";
		}
		outputfile << endl;
		outputfile << "Avg Utilization = " << AvgUti() << "%" << std::endl;
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
		CheckForRR = RR_Count != 0;
		CheckForSJF = SJF_Count != 0;
		Processor_count = FCFS_Count + SJF_Count + RR_Count;
		AllProcessors = new Processor*[Processor_count];
		ProcessorLoad = new int[Processor_count];
		ProcessorUti = new int[Processor_count];
		
		//Second line in text file
		std::getline(inputfile, line);
		std::stringstream secondline(line);
		secondline >> TimeSliceRR;
		if (TimeSliceRR == 0)
		{
			Processor_count = Processor_count - RR_Count;
			RR_Count = 0;
			CheckForRR = false;
		}

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
					P->SetIOs(IOR, IOD);
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
		ProcessorLoad[i] = (double(BS) / AllProcessesTRT)*100;
	}
}
void Scheduler::Uti()
{
	for (int i = 0; i < Processor_count; i++)
	{
		int BS = AllProcessors[i]->GetBusyTime();
		int IT = AllProcessors[i]->GetIdleTime();
		ProcessorUti[i] = (double(BS) / (BS + IT))*100;
	}
}

int Scheduler::AvgUti()
{
	int sum = 0;
	for (int i = 0; i < Processor_count; i++)
	{
		sum = ProcessorUti[i] + sum;
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
				Process* ptemp = nullptr;
				FCFS* temp = nullptr;
				FCFS_Processors->DeleteFirst(temp);
				FCFS_Processors->InsertEnd(temp);
				if (temp->SearchForProcess(kid, ptemp, CurrentTimestep))
				{
					KillCount++;
					processfound = true;
					MovetoTRM(ptemp);
					temp->KillIsDone();
					ProcessOrphan(ptemp);
					if (!(temp->ProcIsRun(ptemp)))
						temp->RemTime(ptemp);
					if (temp->ProcIsRun(ptemp))
					{
						temp->KillRUN();
					}
					if (P->KillAgain(CurrentTimestep))
					{
						multkillsig = true;
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
int Scheduler::LongestQueue()
{
	int maxprocessor = AllProcessors[0]->TotalTime();
	int maxprocessori = 0;
	
	for (int i = 0; i < Processor_count; i++)
	{
		if (AllProcessors[i]->TotalTime() > maxprocessor)
		{
			maxprocessor = AllProcessors[i]->TotalTime();
			maxprocessori = i;
		}
	
	}

    return maxprocessori;
}

bool Scheduler::ShortestSteal()
{
	int minprocessor = AllProcessors[0]->TotalTime();
	ShortestIndex = 0;
	int numMinProcessors = 1; // Number of processors with max TotalTime

	for (int i = 1; i < Processor_count; i++)
	{
		if (AllProcessors[i]->TotalTime() < minprocessor)
		{
			minprocessor = AllProcessors[i]->TotalTime();
			ShortestIndex = i;
			numMinProcessors = 1;

		}
		else if (AllProcessors[i]->TotalTime() == minprocessor)
		{
			numMinProcessors++;
		}
	}
	if (numMinProcessors >= 2)
	{
		return false; // Return false if at least two processors have the max TotalTime
	}
	return true;
}
bool Scheduler::LongestSteal()
{
	int maxprocessor = AllProcessors[0]->TotalTime();
	LongestIndex = 0;
	int numMaxProcessors = 1; // Number of processors with max TotalTime

	for (int i = 1; i < Processor_count; i++)
	{
		if (AllProcessors[i]->TotalTime() > maxprocessor)
		{
			maxprocessor = AllProcessors[i]->TotalTime();
			LongestIndex = i;
			numMaxProcessors = 1;
		}
		else if (AllProcessors[i]->TotalTime() == maxprocessor)
		{
			numMaxProcessors++;
		}
	}
	if (numMaxProcessors >= 2)
	{
		return false; // Return false if at least two processors have the max TotalTime
	}
	return true;
}

//This function will search for the shortest SJF processor and save it on ShortestSJF variable
void Scheduler::SetShortestSJF()
{
	SJF* temp = nullptr;
	int  i = 0;
	SJF_Processors->peek(ShortestSJF);
	while (SJF_Processors->DeleteFirst(temp))
	{
		if (temp->TotalTime() < ShortestSJF->TotalTime())
		{
			ShortestSJF = temp;
		}
		i++;
		SJF_Processors->InsertEnd(temp);
		if (i == SJF_Count) break;
	}
}

//This function will search for the shortest RR processor and save it on ShortestRR variable
void Scheduler::SetShortestRR()
{
	RR* temp = nullptr;
	int i = 0;
	RR_Processors->peek(ShortestRR);
	while (RR_Processors->DeleteFirst(temp))
	{
		if (temp->TotalTime() < ShortestRR->TotalTime())
		{
			ShortestRR = temp;
		}
		i++;
		RR_Processors->InsertEnd(temp);
		if (i == RR_Count) break;
	}

}
//================================================================================================================================//
//============================================================== END OF PHASE-2 ==================================================//
//================================================================================================================================//