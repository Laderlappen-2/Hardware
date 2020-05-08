#include "MeAuriga.h"
#include "CommandHandler.h"
#include "EngineModule.h"
#include "SensorController.h"

#include "MultiDriver.h"
#include "ManualDriver.h"
#include "AutoDriver.h"


CommandHandler *commandHandler;
SensorController *sensor;
MultiDriver *driver;
EngineModule* engine;

enum driveType_e
{
	manual = 'm',
	autonomous = 'a',
};

void setup()
{
	Serial.begin(9600);

	sensor = SensorController::getInstance();

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
	driver->run();
	engine->run();
}
