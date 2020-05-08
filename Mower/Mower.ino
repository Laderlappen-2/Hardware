#include "MeAuriga.h"
#include "CommandHandler.h"
#include "SensorController.h"


CommandHandler *commandHandler;
//SensorController controller;

void setup()
{
	  Serial.begin(9600);

	  commandHandler = CommandHandler::getInstance();
	  commandHandler->init(SLOT1, SLOT2);
	  //commandHandler = CommandHandler::getInstance();
	  //commandHandler = new CommandHandler();
	  //commandHandler->init(SLOT1,SLOT2);
	  //setTestSequence();

}

void setTestSequence()
{
	//Serial.println("Adding cmd Sequence");
	//								speed,	turn,		ms
//EngineModule::cmd sequence[] = {{255,    NO_TURN,    5000},
 //                                   {200,    7,            1000},};

	//commandHandler->addCommand(sequence, 2, setTestSequence);
}

void loop()
{  
	//commandHandler->run();
	//Serial.print("Distance : ");
  //Serial.print(controller.getUltrasonicValue());  
  //Serial.println(" cm");
  
}
