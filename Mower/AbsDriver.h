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

protected:
#define safetyDistance  10

	SensorController* sensorInstance;
};