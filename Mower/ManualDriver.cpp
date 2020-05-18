#include "ManualDriver.h"

ManualDriver::ManualDriver()
{
	sensorInstance = SensorController::getInstance();
	engineInstance = EngineModule::getInstance();
}

void ManualDriver::init()
{
}

void ManualDriver::sendCMD(int speed, int turn) 
{
  engineInstance->setSpeed(speed);
  engineInstance->setTurn(turn);
}

void ManualDriver::run() 
{
	enum state_s
	{
		normal,
		fix,
	};
	static state_s state = normal;
	EngineModule::cmd command = {-50,0,-1};

	switch (state)
	{
	case normal:
		if (sensorOn == true && sensorInstance->getUltrasonicValue() <= safetyDistance)
		{
			state = fix;
		}
		break;
	case fix:
		if (sensorInstance->getUltrasonicValue() >= safetyDistance)
			state = normal;
		break;
	default:
		break;
	}

}
void ManualDriver::listener(int data[], int size) 
{
	int speed = data[0];
	int turn = data[1];
	sendCMD(speed, turn);
}
