
#include "MeAuriga.h"
#include "CommandHandler.h"
#include "EngineModule.h"
#include "BluetoothController.h"

#include "MultiDriver.h"
#include "ManualDriver.h"
#include "AutoDriver.h"

CommandHandler *commandHandler;
EngineModule *engine;
BluetoothController *btController = BluetoothController::getInstance();
MultiDriver *driver;

enum driveType_e
{
	manual = 'm',
	autonomous = 'a',
};

void switchToAutoListner(int *data, int size)
{	driver->selectDriver(String(autonomous));}
void switchToManuelListner(int *data, int size)
{
  Serial.println("IN MANUAL LISTENER");
  driver->selectDriver(String(manual));
}

void setup()
{
	Serial.begin(115200);
	btController->addReciveListner(BluetoothController::reciveType_e::atomatic, switchToAutoListner);
	btController->addReciveListner(BluetoothController::reciveType_e::manuall, switchToManuelListner);

	commandHandler = CommandHandler::getInstance();
	commandHandler->init();

	engine = EngineModule::getInstance();
	engine->init(SLOT1, SLOT2);

	driver = MultiDriver::getInstance();
	driver->init();
	driver->addDriver(new ManualDriver(), (String)driveType_e::manual);
	driver->addDriver(new AutoDriver(), (String)driveType_e::autonomous);
  Serial.println("SETUP DONE!");
}

void loop()
{
	commandHandler->run();
	btController->run();
	driver->run();
	//engine->run();
}
