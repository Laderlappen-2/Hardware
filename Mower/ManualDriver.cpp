#include "ManualDriver.h"

ManualDriver::ManualDriver()
{

}
ManualDriver::init()
{
	//sensorInstance = SensorController::getInstance();
}
ManualDriver::sendCMD(int speed, int turn) 
{
	//if (turn == 0)
		//turn = 3001;
	EngineModule::setCommand(EngineModule::cmd(speed, turn, 3)) //TODO change 3ms time
}
ManualDriver::run() 
{
	if (sensorOn == true && sensorInstance.getUltrasonicValue >= safetyDistance)
	{
		//TODO report back to BluetoothController 
		
		CommandHandler::stopEngine(); //TODO make sure that mower does not get infinit stuck inside safetyDistance:)
	}
		

}
ManualDriver::listener(int data[], int size) //0 speed - 1 turn
					// data i form av tal // size kommer alltid vara 2?
{
	int speed = data[0];

	int turn = data[1];

	sendCMD(speed, turn);
}