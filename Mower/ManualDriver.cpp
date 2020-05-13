#include "ManualDriver.h"

ManualDriver::ManualDriver()
{
	sensorInstance = SensorController::getInstance();
	commandInstance = CommandHandler::getInstance();
  engineInstance = EngineModule::getInstance();
}
void ManualDriver::init()
{
}
void ManualDriver::sendCMD(int speed, int turn) 
{
	//if (turn == 0)
		//turn = 3001;
	//EngineModule::setCommand(EngineModule::cmd(speed, turn, 3)) //TODO change 3ms time
  engineInstance->setSpeed(speed);
  engineInstance->setTurn(turn);
}
void ManualDriver::run() 
{
	enum state_s
	{
		normal,
		event,
		fix,
	};
	static state_s state = normal;
	EngineModule::cmd command = {-50,0,-1};

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
		//clock_t start = clock();
		/*
			Send a message followed by 'start', which is time elapsed since program start
			Example @event,X,Y,klocka$ :
				@1,X,Y,start$

		*/
		commandInstance->stopEngine();
		//TODO fix syntax
		commandInstance->addCommand(command);
		state = fix;
		break;
	case fix:
		if (sensorInstance->getUltrasonicValue() >= safetyDistance)
			state = normal;
		break;
	default:
		break;
	}

}
void ManualDriver::listener(int data[], int size) //0 speed - 1 turn
					// data i form av tal // size kommer alltid vara 2?
{
	int speed = data[0];

	int turn = data[1];

  Serial.println("MANUAL DRIVER DATA:" + speed);
  
	sendCMD(speed, turn);
}
