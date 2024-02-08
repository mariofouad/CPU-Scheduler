#include"Scheduler.h"

using namespace std;

int main()
{
   /* Process* p1 = new Process(0, 1, 5);
    Process* p2 = new Process(0, 2, 2);
    Process* p3 = new Process(0, 3, 6);

    SL_PriorityQueue<Process*>* q = new SL_PriorityQueue<Process*>;
    q->add(p2, p2->GetCT());
    q->add(p1, p1->GetCT());
    q->add(p3, p3->GetCT());
    q->print();
    Process* ptest;
    q->remove(ptest);
    cout << ptest->GetCT();*/
	Scheduler Sch1;
	Sch1.SIMULATOR();
}