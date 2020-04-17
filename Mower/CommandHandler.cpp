#include "CommandHandler.h"
#include <MeEncoderOnBoard.h>
#include <Arduino.h>

CommandHandler::CommandHandler()
{
}
void CommandHandler::init(int slot1,int slot2)
{
	engine = new EngineModule(slot1,slot2);
}



CommandHandler::~CommandHandler()
{	
	delete engine;
}
/*
CommandHandler * CommandHandler::getInstance()
{
	if (CommandHandler::instance == nullptr)
		CommandHandler::instance = new CommandHandler();
	return CommandHandler::instance;
}*/


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
			
			Serial.print("Starting sequence ");
			Serial.println(commandQueue.count());
		}
		break;
	case doSequence:// runs the sequense and waits until it's done
		if (runSequence())
			state = callback;
		break;
	case callback:// calls thr callback function (if there is any) and removes the sequense from the queue
		if (commandQueue.front().callback != nullptr)
			commandQueue.front().callback();
		Serial.println("Sequence Done ");
		Serial.print(commandQueue.count());
		commandQueue.dequeue();
		state = idle;
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
	static cmdSequense* sequence = nullptr;

	switch (state)
	{
	case load:// loads the front sequense from the queue
		if (commandQueue.isEmpty())
		{
			Serial.println("\tEmpty sequence");
			return true;
		}
		sequence = &commandQueue.front();

		state = send;
		break;

	case send:// sends the front cmd in the sequense to the engine and removes it from the queue
		if (!engine->isReady())
			break;
		Serial.print("\tSending cmd ");
		Serial.println(sequence->sequense.count());
		sendCmdToEngine(sequence->sequense.dequeue());
		state = wait;

		Serial.print("\tWaiting...");
		break;
	case wait:// waits to run the sequense until the engine is ready
		if (engine->isReady()) 
			state = check;
		break;
	case check://checks if end of sequens and returns true if it is.
		Serial.println("Done.");
		if (sequence->sequense.isEmpty())
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
	//QueueArray<EngineModule::cmd> cmdQueue;
	Queue<EngineModule::cmd> cmdQueue;
	cmdQueue.enqueue(command);
	commandQueue.enqueue(cmdSequense {cmdQueue, callback});
}

void CommandHandler::addCommand(EngineModule::cmd command[],int size, void(*callback)(void))
{
	cmdSequense sequense;
	for (int i = 0; i < size; i++)
	{
		sequense.sequense.enqueue(command[i]);
	}
	sequense.callback = callback;
	commandQueue.enqueue(sequense);
}

void CommandHandler::addCommand(cmdSequense commandSeq)
{	commandQueue.enqueue(commandSeq);}

void CommandHandler::clear()
{	
	while (!commandQueue.isEmpty())
		commandQueue.dequeue();
}

void CommandHandler::stopEngine()
{	engine->stopp();}

void CommandHandler::sendCmdToEngine(EngineModule::cmd command)
{	engine->setCommand(command);}
