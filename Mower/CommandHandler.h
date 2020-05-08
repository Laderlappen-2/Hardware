#pragma once

#include <stdlib.h>
#include <Arduino.h>
//#include <QueueArray.h>


#include "EngineModule.h"
#include "Queue.h"

using namespace std;

class CommandHandler
{
public:

	struct cmdSequense
	{		
		//QueueArray<EngineModule::cmd> sequense;
		Queue<EngineModule::cmd> sequense;
		void(*callback)(void);
	};

	~CommandHandler();
	//get singelton instance
	static CommandHandler* getInstance()
	{
		static CommandHandler* instance = nullptr;
		if (instance == nullptr)
			instance = new CommandHandler();
		return instance;
	}

	//initilize the class. must be called before any other calls
	void init(int,int);

	//run the main state machine
	void run();

	//adds an engine command at the back if the queue
	void addCommand(EngineModule::cmd,void(*callback)(void) = nullptr);
	void addCommand(EngineModule::cmd[],int, void(*callback)(void) = nullptr);

	//cleares the entire command queue but dose NOT stop the engine
	void clear();
	//cleres the command queue and immediately stops the engine
	void stopEngine();

private:
	CommandHandler();
	
	//sends a command to engine
	void sendCmdToEngine(EngineModule::cmd);
	//sud state machine to run a singilar cmd sequense, returns true when done.
	bool runSequence();

	//static CommandHandler* instance;
	EngineModule* engine;
	//QueueArray<cmdSequense> commandQueue;
	Queue<cmdSequense*> commandQueue;

};
