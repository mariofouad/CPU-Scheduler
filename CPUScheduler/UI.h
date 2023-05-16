#ifndef USERINTERFACE
#define USERINTERFACE

#pragma once
#include <iostream>		//This class will be the only class containing I/O library

using namespace std;
class Scheduler;


//this enum class is made to give interface modes values with their actual interface name
enum Mode {
	INTERACTIVE = 1,
	STEPBYSTEP = 2,
	SILENT = 3
};


class UI {

private:

	Scheduler* S;						//Pointer from Scheduler class because it is the only class allowed to access the needed data
	Mode M;								//The value of interaction mode
	int Input;							//Input value from the user to set the Interface mode

public:

	UI(Scheduler* a);

	int UpdateInterface() const;

	void SilentMode() const;

	~UI();
};

#endif