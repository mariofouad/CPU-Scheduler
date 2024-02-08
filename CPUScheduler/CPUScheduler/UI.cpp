#include <iostream>
#include "windows.h"
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
	string Ffinal;
	cout << "Enter your input file name here: " << endl;
	cin >> FnamefromUser;
	Ffinal = FnamefromUser + ".txt";
	S->setFileName(Ffinal);
	while (!S->FileisFound())
	{
		cout << "File is not found , RE-ENTER file nmae: " << endl;
		cin >> FnamefromUser;
		Ffinal = FnamefromUser + ".txt";
		S->setFileName(Ffinal);
	}
	cout << "You entered: " << FnamefromUser << endl;
	cout << "Your file is found : " << Ffinal << "" << endl;
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


int UI::UpdateInterface() const {

	char anykey;

	switch (M)
	{
	case INTERACTIVE:
		cout << endl;
		S->PrintWindow();
		cout << "PRESS ANYKEY TO MOVE TO NEXT STEP!";
		cin >> anykey;
		cout << endl;
		return 1;

	case STEPBYSTEP:
		cout << endl;
		S->PrintWindow();
		cout << "THE PROGRAM WILL MOVE TO THE NEXT STEP AUTOMATICALLY!";
		cout << endl;
		Sleep(1000);
		return 2;

	case SILENT:
		return 3;
	}
}

void UI::SilentMode() const {
	cout << endl;
	cout << "======================================" << endl;
	cout << "Silent mode...   Simulation Starts.." << endl;
}

void UI::OutputPart() const
{
	string namefromuser;
	cout << endl;
	cout << "======================================" << endl;
	cout << "ENTER THE OUTPUT FILE NAME HERE: " << endl;
	cin >> namefromuser;
	string FileName = namefromuser + ".txt";
	S->SetOutputFName(FileName);
	cout << "Simulation ends, Output file created" << endl;
}

UI::~UI() {}