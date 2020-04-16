#include "CommandHandler.h"

#include <MeEncoderOnBoard.h>

CommandHandler::CommandHandler()
{

}
void CommandHandler::init()
{
}





CommandHandler::~CommandHandler()
{	delete instance;}


CommandHandler * CommandHandler::getInstance()
{
	if (instance == nullptr)
		instance = new CommandHandler();
	return instance;
}

void CommandHandler::run()
{
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
			state = doSequence;
		break;
	case doSequence:// runs the sequense and waits until it's done
		if (runSequence())
			state = callback;
		break;
	case callback:// calls thr callback function (if there is any) and removes the sequense from the queue
		if (commandQueue.front().callback != nullptr)
			commandQueue.front().callback();
		commandQueue.pop();
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
	static state_s state = send;
	static cmdSequense* sequence = nullptr;

	switch (state)
	{
	case load:// loads the front sequense from the queue
		if (commandQueue.isEmpty())
			return true;
		sequence = &commandQueue.front();
		state = wait;
		break;
	case wait:// waits to run the sequense until the engine is ready
		//TODO implement ready function in engine to check here
		//if (ready()) state = send;
		break;
	case send:// sends the front cmd in the sequense to the engine and removes it from the queue
		sendCmdToEngine(sequence->sequense.front());
		sequence->sequense.pop();
		state = check;
		break;
	case check://checks if end of sequens and returns trur if it is.
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


void CommandHandler::addCommand(cmd command, void(*callback)(void))
{
	QueueArray<cmd> cmdQueue;
	cmdQueue.push(command);
	commandQueue.push(cmdSequense {cmdQueue, callback});
}

void CommandHandler::addCommand(cmd command[],int size, void(*callback)(void))
{
	cmdSequense sequense;
	for (int i = 0; i < size; i++)
	{
		sequense.sequense.push(command[i]);
	}
	sequense.callback = callback;
	commandQueue.push(sequense);
}

void CommandHandler::addCommand(cmdSequense commandSeq)
{	commandQueue.push(commandSeq);}

void CommandHandler::clear()
{	
	while (!commandQueue.isEmpty())
		commandQueue.pop();
}

void CommandHandler::stopEngine()
{

	//TODO implement when engine is ready 
}



void CommandHandler::sendCmdToEngine(cmd)
{
	//TODO implement when engine is ready
}
