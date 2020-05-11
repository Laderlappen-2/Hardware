#pragma once
//#include "EngineModule.h
#include "CommandHandler.h"
#include "AbsDriver.h"
#include <time.h>

class ManualDriver : public AbsDriver
{
public:

	ManualDriver();

	void init();
	void run();
	void listener(int[], int);
	void sendCMD(int, int);
	
private:

	bool sensorOn = false;

	SensorController* sensorInstance;
	CommandHandler* commandInstance;


};
