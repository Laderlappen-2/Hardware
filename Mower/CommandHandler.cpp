#include "CommandHandler.h"
#include <MeEncoderOnBoard.h>
#include <Arduino.h>

CommandHandler::CommandHandler()
{
}
void CommandHandler::init(int slot1,int slot2)
{
	engine = EngineModule::getInstance();
}



CommandHandler::~CommandHandler()
{	
	//delete engine;
}

void CommandHandler::run()
{
	engine->run();
	
	enum state_e
	{
		idle,
		doSequence,
		callback,
	};
	static state_e state = idle;


	switch (state)
	{
	case idle://wait until there is a sequense to run
		if (!commandQueue.isEmpty())
		{
			state = doSequence;
			
			//Serial.print("Starting sequence ");
			//Serial.println(commandQueue.count());
		}
		break;
	case doSequence:// runs the sequense and waits until it's done
		if (runSequence())
			state = callback;
		break;
	case callback:// calls the callback function (if there is any) and removes the sequense from the queue
		
		//Serial.print("Sequence Done ");
		//Serial.println(commandQueue.count());
		delete commandQueue.dequeue();
		state = idle;
		if (commandQueue.front()->callback != nullptr)
     commandQueue.front()->callback();
		break;
	default:
		break;
	}
}
bool CommandHandler::runSequence()
{
	enum state_s
	{
		load,
		wait,
		send,
		check,
	};
	static state_s state = load;

#define SEQUENCE commandQueue.front()->sequense

	switch (state)
	{
	case load:// loads the front sequense from the queue
		if (commandQueue.isEmpty())
		{
			//Serial.println("\tEmpty sequence");
			return true;
		}

		state = send;
		break;

	case send:// sends the front cmd in the sequense to the engine and removes it from the queue
		if (!engine->isReady())
			break;
		//Serial.print("\tSending cmd ");

		//Serial.println(SEQUENCE.count());
		sendCmdToEngine(SEQUENCE.dequeue());
		state = wait;

		//Serial.print("\tWaiting...");
		break;
	case wait:// waits to run the sequense until the engine is ready
		if (engine->isReady()) 
			state = check;
		break;
	case check://checks if end of sequens and returns true if it is.
		//Serial.println("Done.");
		//if (sequence.sequense.isEmpty())
		if (SEQUENCE.isEmpty())
		{
			state = load;
			return true;
		}
		else state = send;
		break;
	default:
		break;
	}

	return false;
}


void CommandHandler::addCommand(EngineModule::cmd command, void(*callback)(void))
{
	Queue<EngineModule::cmd> cmdQueue; 
	cmdQueue.enqueue(command);
	commandQueue.enqueue(new cmdSequense {cmdQueue, callback});
}

void CommandHandler::addCommand(EngineModule::cmd command[],int size, void(*callback)(void))
{

	cmdSequense* sequense = new cmdSequense;
	for (int i = 0; i < size; i++)
	{
		sequense->sequense.enqueue(command[i]);		
	}
	sequense->callback = callback;
	commandQueue.enqueue(sequense);
}
void CommandHandler::clear()
{	
	while (!commandQueue.isEmpty())
		delete commandQueue.dequeue();
}

void CommandHandler::stopEngine()
{	engine->stopp();}

void CommandHandler::sendCmdToEngine(EngineModule::cmd command)
{	engine->setCommand(command);}
