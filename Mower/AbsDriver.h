#pragma once
#include <Arduino.h>
#include <stdlib.h>
#include "CommandHandler.h"
#include "SensorController.h"


class AbsDriver
{
public:

	AbsDriver();

	virtual void init();

	virtual	void run();
	virtual	void sendCMD();
	virtual	void listener(int[], int);


	virtual void onActivation();
	virtual void onDeactivation();
protected:


	static const int safetyDistance = 10;

	SensorController* sensorInstance;
};
