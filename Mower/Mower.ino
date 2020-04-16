#include "MeAuriga.h"
#include "CommandHandler.h"


CommandHandler *commandHandler;

void setup()
{
	  Serial.begin(9600);

	  //commandHandler = CommandHandler::getInstance();
	  commandHandler = new CommandHandler();
	  commandHandler->init(SLOT1,SLOT2);
	  setTestSequence();

}

void setTestSequence()
{
	Serial.println("Adding cmd Sequence");
	//								speed,	turn,		ms
	EngineModule::cmd sequence[] = {{100,	NO_TURN,	500},
									{200,	7,			50},
									{-150,	NO_TURN,	250},
									{200,	-7,			50},
									{-100,	30,			500}, };

	commandHandler->addCommand(sequence, 5, setTestSequence);
}

void loop()
{  
	commandHandler->run();
}
