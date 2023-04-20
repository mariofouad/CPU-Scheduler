#include "UI.h"
#include "Scheduler.h"

using namespace std;

UI::UI(Scheduler* a)
{
	S = a;
	cout << "Welcome to ""processor scheduler"" !! " << endl;
	cout << "======================================" << endl;

	//////////////////////////////////////////
	//The user enters the input file name Here
	string FnamefromUser;
	cout << "Enter your input file name here: " << endl;
	cin >> FnamefromUser;
	S->setFileName(FnamefromUser);
	while (!S->FileisFound())
	{
		cout << "File is not found , RE-ENTER file nmae: " << endl;
		cin >> FnamefromUser;
		S->setFileName(FnamefromUser);
	}
	cout << "You entered: " << FnamefromUser << endl;
	cout << "======================================" << endl;
	cout << "There are Three Modes:" << endl << "Enter 1 For Interactive Mode" << endl << "Enter 2 For Step - by - step mode" << endl << "Enter 3 For Silent mode" << endl;
	//The input file is now set by the user
	///////////////////////////////////////

	////////////////////////////////////////////////////////////////
	//Now let the user choose what will be the Mode of the interface 
	cin >> Input;
	if (Input == 1) {
		M = INTERACTIVE;
	}
	else if (Input == 2) {
		M = STEPBYSTEP;
	}
	else {
		M = SILENT;
	}
	//Now We have the mode set by the user
	////////////////////////////////////////////////////////////////
}


void UI::UpdateInterface() const {

	char anykey;

	switch (M)
	{
	case INTERACTIVE:
		cout << endl;
		S->PrintWindow();
		cout << "PRESS ANYKEY TO MOVE TO NEXT STEP!";
		cin >> anykey;
		cout << endl;
		break;

	case STEPBYSTEP:
		cout << endl;
		S->PrintWindow();
		cout << "PRESS ANYKEY TO MOVE TO NEXT STEP!";
		cout << endl;
		/*_sleep(1000);*/
		break;

	case SILENT:
		cout << endl;
		cout << "=========================================";
		cout << "silent mode........   Simulation Starts.." << endl;
		cout << "Simulation ends, Output file created" << endl;
		break;
	}

}

UI::~UI() {}