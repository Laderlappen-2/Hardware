#include "AbsDriver.h"

AbsDriver::AbsDriver() 
{
	sensorInstance = SensorController::getInstance();
}

void AbsDriver::run()
{
}

void AbsDriver::sendCMD()
{
}

void AbsDriver::listener(int data[], int size)
{
}
