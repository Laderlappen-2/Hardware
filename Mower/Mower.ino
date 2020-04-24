
#include "MeAuriga.h"
#include "CommandHandler.h"
#include "BluetoothController.h"

CommandHandler *commandHandler;
BluetoothController *btController = BluetoothController::getInstance();


void rxListner(int data[], int size)
{
	Serial.print("Recived data: ");
	for (int i = 0; i < size; i++)
	{
		Serial.print(data[i]);
		Serial.print(", ");
	}

	Serial.print('\n');
}
void setTestSequence()
{
	Serial.println("Adding cmd Sequence");
	//								speed,	turn,		ms
	EngineModule::cmd sequence[] = {{255,	NO_TURN,	500},
									{200,	7,			100},};

	commandHandler->addCommand(sequence, 2, setTestSequence);
}

void setup()
{
	  Serial.begin(9600);

	  //commandHandler = CommandHandler::getInstance();
	  commandHandler = new CommandHandler();
	  commandHandler->init(SLOT1,SLOT2);
	  setTestSequence();

	  btController->addReciveListner(BluetoothController::reciveType_e::atomatic, rxListner);
	  btController->addReciveListner(BluetoothController::reciveType_e::drive, rxListner);
	  btController->addReciveListner(BluetoothController::reciveType_e::honk, rxListner);
	  btController->addReciveListner(BluetoothController::reciveType_e::ligh, rxListner);
	  btController->addReciveListner(BluetoothController::reciveType_e::manuall, rxListner);
	  btController->addReciveListner(BluetoothController::reciveType_e::turnOff, rxListner);
}

void loop()
{  
	btController->run();
	commandHandler->run();
}
