#include "CommandHandler.h"



CommandHandler::CommandHandler()
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

void CommandHandler::addCommand(cmd command, void(*callback)(void))
{	commandQueue.push_back(cmdSequense{ vector<cmd>{command},callback});}

void CommandHandler::addCommand(cmd command[],int size, void(*callback)(void))
{
	cmdSequense sequense;
	for (int i = 0; i < size; i++)
	{
		sequense.sequense.push_back(command[i]);
	}
	sequense.callback = callback;
	commandQueue.push_back(sequense);
}

void CommandHandler::addCommand(cmdSequense commandSeq)
{	commandQueue.push_back(commandSeq);}

void CommandHandler::clear()
{	commandQueue.clear();}
