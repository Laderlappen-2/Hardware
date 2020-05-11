
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
{	driver->selectDriver(String(manual));}

void setup()
{
	Serial.begin(115200);
	btController->init(115200);
	btController->addReciveListner(BluetoothController::reciveType_e::atomatic, switchToAutoListner);
	btController->addReciveListner(BluetoothController::reciveType_e::manuall, switchToManuelListner);

	commandHandler = CommandHandler::getInstance();
	commandHandler->init();

	engine = EngineModule::getInstance();
	engine->init(SLOT1, SLOT2);

	driver = new MultiDriver();
	driver->init();
	driver->addDriver(new ManualDriver(), (String)driveType_e::manual);
	driver->addDriver(new AutoDriver(), (String)driveType_e::autonomous);
}

void loop()
{
	commandHandler->run();
	btController->run();
	driver->run();
}
