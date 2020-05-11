
#include "MeAuriga.h"
#include "CommandHandler.h"
#include "BluetoothController.h"

CommandHandler *commandHandler;
BluetoothController *btController = BluetoothController::getInstance();
AppInstructions* appInstructions = AppInstructions::getInstance();
byte byteArr[3] = {97,98,99};

void rxListner(int data[], int size)
{
	Serial.println("IN rxListner(): Recived data: ");
	for (int i = 0; i < size; i++)
	{
		Serial.print(data[i]);
		Serial.print(", ");
	}
  Serial.print("\n");
}

void setTestSequence()
{
	//Serial.println("Adding cmd Sequence");
	//								speed,	turn,		ms
	//EngineModule::cmd sequence[] = {{appInstructions->getVelocity(),	appInstructions->getTurn(),	1000}};

  EngineModule::cmd sequence[] = {{appInstructions->getVelocity(), appInstructions->getTurn(), 500}};
	commandHandler->addCommand(sequence, 1, setTestSequence);
}

//TestCode for bluetooth
void checkBluetooth()
{
  while(Serial.available() > 0)
  {
    delay(10);
    char currentChar = Serial.read();
    Serial.println(currentChar);

    if(currentChar == '@')
	    setTestSequence();
  }

}

void setup()
{
  Serial.begin(115200);
	//btController->init(115200);

	//commandHandler = CommandHandler::getInstance();
	commandHandler = new CommandHandler();
	commandHandler->init(SLOT1,SLOT2);
	//setTestSequence();

	btController->addReciveListner(BluetoothController::reciveType_e::atomatic, rxListner);
	btController->addReciveListner(BluetoothController::reciveType_e::drive, rxListner);
	btController->addReciveListner(BluetoothController::reciveType_e::honk, rxListner);
	btController->addReciveListner(BluetoothController::reciveType_e::ligh, rxListner);
	btController->addReciveListner(BluetoothController::reciveType_e::manuall, rxListner);
	btController->addReciveListner(BluetoothController::reciveType_e::turnOff, rxListner);
}

void loop()
{
	commandHandler->run();
  btController->run();
  //btController->run();
	//btController->readBluetooth();

  /*if(appInstructions->getInstructionsAvailable())
  {
    setTestSequence();
    appInstructions->setInstructionsAvailable(false);
    Serial.write(appInstructions->getVelocity());
  }*/
  
}
