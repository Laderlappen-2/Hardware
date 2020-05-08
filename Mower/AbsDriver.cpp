#include "AbsDriver.h"

AbsDriver::AbsDriver() 
{
	sensorInstance = SensorController::getInstance();
}

void AbsDriver::init()
{
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

void AbsDriver::onActivation()
{
}

void AbsDriver::onDeactivation()
{
}
