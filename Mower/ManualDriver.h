#pragma once
#include "EngineModule.h"
#include "CommandHandler.h"
#include "AbsDriver.h"
#include <time.h>

class ManualDriver : public AbsDriver
{
public:

	ManualDriver();

	void init() override;
	void run() override;
	void listener(int[], int) override;
	void sendCMD(int, int);
	
private:

	bool sensorOn = false;

	SensorController* sensorInstance;
	CommandHandler* commandInstance;
  EngineModule* engineInstance;
};
