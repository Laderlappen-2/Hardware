#include "ManualDriver.h"

ManualDriver::ManualDriver()
{
	sensorInstance = SensorController::getInstance();
}
void ManualDriver::init()
{
	//sensorInstance = SensorController::getInstance();
}
void ManualDriver::sendCMD(int speed, int turn) 
{
	//if (turn == 0)
		//turn = 3001;
	//EngineModule::setCommand(EngineModule::cmd(speed, turn, 3)) //TODO change 3ms time
}
void ManualDriver::run() 
{
	switch (state)
	{
	case normal:
		if (sensorOn == true && sensorInstance->getUltrasonicValue() <= safetyDistance)
		{
			state = event;
		}
		break;
	case event:
		// TODO rapportera till BT
		clock_t start = clock();
		/*
			Send a message followed by 'start', which is time elapsed since program start
			Example @event,X,Y,klocka$ :
				@1,X,Y,start$

		*/
		state = fix;
		break;
	case fix:
		CommandHandler::stopEngine();
		CommandHandler::addCommand(EngineModule::cmd(-50, 0, -1));
		if (sensorInstance->getUltrasonicValue() >= safetyDistance)
		{
			state = normal;
		}
		break;
	default:
		break;
	}


	//if (sensorOn == true && sensorInstance->getUltrasonicValue() >= safetyDistance)
	//{
	//	//TODO report back to BluetoothController 
	//	
	//	//CommandHandler::stopEngine(); //TODO make sure that mower does not get infinit stuck inside safetyDistance:)
	//}

}
void ManualDriver::listener(int data[], int size) //0 speed - 1 turn
					// data i form av tal // size kommer alltid vara 2?
{
	int speed = data[0];

	int turn = data[1];

	sendCMD(speed, turn);
}