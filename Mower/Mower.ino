
#include "MeAuriga.h"
#include "EngineModule.h"
#include "BluetoothController.h"
#include "MultiDriver.h"
#include "ManualDriver.h"
#include "AutoDriver.h"

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
{ driver->selectDriver(String(manual));}

void setup()
{
	Serial.begin(115200);
	btController->addReciveListner(BluetoothController::reciveType_e::atomatic, switchToAutoListner);
	btController->addReciveListner(BluetoothController::reciveType_e::manuall, switchToManuelListner);

	engine = EngineModule::getInstance();
	engine->init(SLOT1, SLOT2);

	driver = MultiDriver::getInstance();
	driver->init();
	driver->addDriver(new ManualDriver(), (String)driveType_e::manual);
	driver->addDriver(new AutoDriver(), (String)driveType_e::autonomous);
}

void loop()
{
	btController->run();
	driver->run();
	engine->runOdometri();
}
