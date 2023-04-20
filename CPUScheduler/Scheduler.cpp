#include "Scheduler.h"
#include "FCFS.h"
#include "SJF.h"
#include "RR.h"

using namespace std;

//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//
Scheduler::Scheduler() {
	UserInterface = new UI(this);
	NEW = new LinkedQueue<Process>;
	BLK = new LinkedList<Process>;
	TRM = new LinkedQueue<Process>;
	ActualRUN = new LinkedQueue<Process*>;
	FCFS_Processors = new LinkedList<FCFS*>;
	SJF_Processors = new LinkedList<SJF*>;
	RR_Processors = new LinkedList<RR*>;
	AllProcessors = new SL_PriorityQueue<Processor*>;
}

UI* Scheduler::GetInterface() {
	return UserInterface;
}

void Scheduler::setFileName(string fname) {
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

void Scheduler::ReadFile()
{
	string line;
	ifstream inputfile(FileName);
	if (inputfile.is_open())
	{
		//Fist line in text file
		std::getline(inputfile, line);
		stringstream firstline(line);
		firstline >> FCFS_Count >> SJF_Count >> RR_Count;
		Processor_count = FCFS_Count + SJF_Count + RR_Count;

		//Second line in text file
		std::getline(inputfile, line);
		stringstream secondline(line);
		secondline >> TimeSliceRR;

		//Third line in text file
		std::getline(inputfile, line);
		stringstream thirdline(line);
		thirdline >> RTF >> MaxW >> STL >> ForkProb;

		//Fourth line in text file
		std::getline(inputfile, line);
		stringstream fourthline(line);
		fourthline >> Proc_count;
		tempProc_count = Proc_count;

		for (int i = 0; i < Proc_count; i++)
		{
			int AT, PID, CT, N, IOR, IOD;
			std::getline(inputfile, line);
			stringstream Processline(line);
			Processline >> AT >> PID >> CT >> N;
			Process P(AT, PID, CT);
			if (N != 0)
			{
				P.SetNumberOfRequests(N);
				for (int i = 0; i < N; i++)
				{
					int IOR, IOD;
					char c;

					// read until the first digit is found
					while (Processline.get(c) && !isdigit(c));

					// if a digit is found, put it back and read the number
					if (isdigit(c))
					{
						Processline.putback(c);
						Processline >> IOR;

						// read until the second digit is found
						while (Processline.get(c) && !isdigit(c));

						// if a digit is found, put it back and read the number
						if (isdigit(c))
						{
							Processline.putback(c);
							Processline >> IOD;

							// set input data for the process
							P.SetInputData(IOR, IOD, i);
						}
					}
				}

			}
			NEW->Enqueue(P);
		}
		//Bfore last line in text file
		std::getline(inputfile, line);
		stringstream Beforelastline(line);
		Beforelastline >> KillPID;

		//Last line in text file
		std::getline(inputfile, line);
		stringstream Lastline(line);
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
	cout << "===========================================================" << endl;
	cout << "Current Timestep:" << CurrentTimestep << endl;
	cout << "---------------------- RDY processes ----------------------" << endl;
	for (int i = 0; i < FCFS_Count; i++)
	{
		FCFS* Ptemp = nullptr;
		FCFS_Processors->Traversal(Ptemp, i);
		cout << *Ptemp;
		cout << endl;
	}

	for (int i = 0; i < SJF_Count; i++)
	{
		SJF* Stemp = nullptr;
		SJF_Processors->Traversal(Stemp, i);
		cout << *Stemp;
		cout << endl;
	}

	for (int i = 0; i < RR_Count; i++)
	{
		RR* Rtemp = nullptr;
		RR_Processors->Traversal(Rtemp, i);
		cout << *Rtemp;
		cout << endl;
	}

	cout << "---------------------- BLK processes ----------------------" << endl;
	cout << BLK_count << " BLK: ";
	BLK->PrintList();
	cout << endl;

	cout << "---------------------- RUN processes ----------------------" << endl;
	Processor* Ptemp = nullptr;
	int actur = 0;
	for (int i = 0; i < Processor_count; i++)
	{
		AllProcessors->Traversal(Ptemp, i);
		if (Ptemp->IsBusy()) actur++;
	}
	cout << actur << " RUN: ";
	for (int i = 0; i < Processor_count; i++)
	{
		AllProcessors->Traversal(Ptemp, i);
		if (Ptemp->IsBusy())
		{
			Ptemp->PrintRUN();
			cout << ", ";
		}
	}
	cout << endl;

	cout << "---------------------- TRM processes ----------------------" << endl;
	cout << TRM_count << " TRM: ";
	TRM->print();
	cout << endl;

}

void Scheduler::Process_Migartion(Processor* p1, Processor* p2) {}	//this fn will be responsible for migrating processes through different processors
//====================================================================================================================//
//================================================== PHASE-1 SIMULATOR ===============================================//
//====================================================================================================================//
void Scheduler::Phase1Simulator()
{
	//================================================= READ_FILE =====================================================//
	int id = 1;
	ReadFile();
	for (int i = 0; i < FCFS_Count; i++)
	{
		FCFS* P = new FCFS(id++);
		Processor* Pproc = P;
		FCFS_Processors->InsertEnd(P);
		AllProcessors->add(Pproc);
	}
	for (int i = 0; i < SJF_Count; i++)
	{
		SJF* P = new SJF(id++);
		Processor* Pproc = P;
		SJF_Processors->InsertEnd(P);
		AllProcessors->add(Pproc);
	}
	for (int i = 0; i < RR_Count; i++)
	{
		RR* P = new RR(id++);
		Processor* Pproc = P;
		RR_Processors->InsertEnd(P);
		AllProcessors->add(Pproc);
	}
	//==============================================  i  ==============================================================//(DONE)
	while (!WorkisDone())
	{
		int s = 0;
		if (MoveFromNewToRdy())
		{
			GetInterface()->UpdateInterface();
			CurrentTimestep++;
		}
		//=================================================  ii  =========================================================//(DONE) 
		else if (DistToRUN())
		{
			GetInterface()->UpdateInterface();
			CurrentTimestep++;
		}
		//==================================================  iii  =======================================================//(DONE)
		else if (RandomSch())
		{
			GetInterface()->UpdateInterface();
			CurrentTimestep++;
		}
		//=====================================================  iv  =====================================================//(DONE)
		else if (Blk_Rdy_Trm())
		{
			GetInterface()->UpdateInterface();
			CurrentTimestep++;

		}
		////============================================================= v ====================================================//(DONE)
		else if (KillFromFCFS())
		{
			GetInterface()->UpdateInterface();
			CurrentTimestep++;

		}
	}
}

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

//====================================================================================================================//
//====================================================================================================================//
void Scheduler::Work_stealing() {}									//will manage the work stealing

bool Scheduler::WorkisDone()
{
	if (NEW->IsEmpty() && BLK->IsEmpty() && TRM_count == tempProc_count) return true;
	return false;
}


bool Scheduler::MovetoTRM(Process* p)
{
	if (p == nullptr) return false;
	TRM->Enqueue(*p);
	TRM_count++;
	return true;
}


bool Scheduler::MoveFromNewToRdy()
{
	int c = 0;
	Process ptemp;
	FCFS* Ftemp = nullptr;
	SJF* Stemp = nullptr;
	RR* Rtemp = nullptr;
	Process Ntemp;
	while (!(NEW->IsEmpty()))
	{
		if (ProcessorI == (FCFS_Count + SJF_Count + RR_Count)) { ProcessorI = 0; }
		NEW->peek(ptemp);
		if (ptemp.GetAT() == CurrentTimestep)
		{
			NEW->Dequeue(Ntemp);
			Proc_count--;

			if (ProcessorI < FCFS_Count)
			{
				FCFS_Processors->Traversal(Ftemp, ProcessorI);
				Ftemp->InserttoRDY(Ntemp);
				c++;
			}

			if (ProcessorI >= (FCFS_Count) && ProcessorI < (FCFS_Count + SJF_Count))
			{
				int is = ProcessorI - FCFS_Count;
				SJF_Processors->Traversal(Stemp, is);
				Stemp->InserttoRDY(Ntemp);
				c++;
			}

			if (ProcessorI >= (SJF_Count + FCFS_Count) && ProcessorI < (FCFS_Count + SJF_Count + RR_Count))
			{
				int ir = ProcessorI - SJF_Count - FCFS_Count;
				RR_Processors->Traversal(Rtemp, ir);
				Rtemp->InserttoRDY(Ntemp);
				c++;
			}
			ProcessorI++;
		}
		else
		{
			break;
		}
	}
	return (c != 0);
}

bool Scheduler::MoveToBlk(Process* p)
{
	if (p == nullptr) return false;
	BLK->InsertEnd(*p);
	BLK_count++;
	return true;
}

bool Scheduler::MoveToRDY(Process* p)
{
	if (p == NULL) return false;
	srand(time(0));                                                 // seed the random number generator with current time
	Processor* P = nullptr;
	int random_num = rand() % Processor_count;
	AllProcessors->Traversal(P, random_num);
	P->InserttoRDY(*p);
	return true;
}

void Scheduler::SetActualRUN() {
	int i = 1;
	Processor* temp = nullptr;
	while (AllProcessors->remove(temp)) {

		if (temp->IsBusy())
		{
			ActualRUNcount++;
		}
		Process* prtemp = temp->GetRUN();
		ActualRUN->Enqueue(prtemp);
		AllProcessors->add(temp);
		i++;
		if (i == Processor_count) break;
	}
}

bool Scheduler::DistToRUN()
{
	int c = 0;
	for (int i = 0; i < Processor_count; i++)
	{
		if (i < FCFS_Count)
		{
			FCFS* Fproc = nullptr;
			FCFS_Processors->Traversal(Fproc, i);
			if (!Fproc->IsBusy() && !Fproc->IsIdeal())
			{
				Fproc->MoveFromRDYToRUN();
				c++;
			}
		}

		if (i >= (FCFS_Count) && i < (FCFS_Count + SJF_Count))
		{
			int is = i - FCFS_Count;
			SJF* Sproc = nullptr;
			SJF_Processors->Traversal(Sproc, is);
			if (!Sproc->IsBusy() && !Sproc->IsIdeal())
			{
				Sproc->MoveFromRDYToRUN();
				c++;
			}
		}

		if (i >= (SJF_Count + FCFS_Count) && i < (FCFS_Count + SJF_Count + RR_Count))
		{
			int ir = i - FCFS_Count - SJF_Count;
			RR* Rproc = nullptr;
			RR_Processors->Traversal(Rproc, ir);
			if (!Rproc->IsBusy() && !Rproc->IsIdeal())
			{
				Rproc->MoveFromRDYToRUN();
				c++;
			}
		}
	}
	return (c != 0);
}

bool Scheduler::Blk_Rdy_Trm()
{
	int c = 0;
	Process* Rtemp = nullptr;
	Processor* Atemp = nullptr;
	int i = 0;
	while (AllProcessors->remove(Atemp))
	{
		if (GenerateRandom() == 1)
		{
			Rtemp = Atemp->GetRUN();
			MoveToBlk(Rtemp);
			Atemp->KillRUN();
			c++;
		}
		if (GenerateRandom() == 2)
		{
			Rtemp = Atemp->GetRUN();
			MoveToRDY(Rtemp);
			Atemp->KillRUN();
			c++;
		}
		if (GenerateRandom() == 3)
		{
			Rtemp = Atemp->GetRUN();
			MovetoTRM(Rtemp);
			Atemp->KillRUN();
			c++;
		}
		AllProcessors->add(Atemp);
		i++;
		if (i == Processor_count) break;
	}
	return (c != 0);
}

bool Scheduler::RandomSch()
{
	srand(time(0));                                                 // seed the random number generator with current time
	int random_num1 = rand() % 100 + 1;                              // generate a random number between 1 and 100
	if (random_num1 < 10 && !BLK->IsEmpty())
	{
		Process p;
		BLK->DeleteFirst(p);
		BLK_count--;
		MoveToRDY(&p);
		return true;
	}
	return false;
}

bool Scheduler::KillFromFCFS()
{
	srand(time(0));
	int random_num2 = rand() % tempProc_count + 1;
	Process pfind(0, random_num2, 0);
	for (int i = 0; i < FCFS_Count; i++)
	{
		FCFS* temp;
		FCFS_Processors->Traversal(temp, i);
		if (temp->ProcIsFound(&pfind))
		{
			MovetoTRM(&pfind);
			return true;
		}
	}
	return false;
}

Scheduler::~Scheduler() {}


//for (int i = 0; i < Processor_count; i++)
	//{
	//	Process* p = new Process;
	//	Processor * Ptemp = nullptr;
	//	AllProcessors->Traversal(Ptemp, i);
	//	int a = Ptemp->GenerateRandom(&p);
	//	if (a == 1)
	//	{
	//		Ptemp->KillRUN();
	//		MoveToBlk(p);
	//	}
	//	if (a == 2)
	//	{
	//		Ptemp->KillRUN();
	//		Ptemp->InserttoRDY(*p);
	//	}
	//	if (a == 3)
	//	{
	//		Ptemp->KillRUN();
	//		MovetoTRM(p);
	//	}
	//}