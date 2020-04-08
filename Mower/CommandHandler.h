#pragma once

#include <stdlib.h>
#include <Arduino.h>
#include <QueueArray.h>

using namespace std;

class CommandHandler
{
public:

	struct cmd//TODO move this struct to engine class
	{
		/*int speed;
		int turnRaduis;
		int time_ms;*/
	};

	struct cmdSequense
	{		
		QueueArray<cmd> sequense;
		void(*callback)(void);
	};

	~CommandHandler();
	//get singelton instance
	static CommandHandler* getInstance();
	//initilize the class. must be called before any other calls
	void init();

	//run the main state machine
	void run();

	//adds an engine command at the back if the queue
	void addCommand(cmd,void(*callback)(void) = nullptr);
	void addCommand(cmd[],int, void(*callback)(void) = nullptr);
	void addCommand(cmdSequense);

	//cleares the entire command queue but dose NOT stop the engine
	void clear();
	//cleres the command queue and immediately stops the engine
	void stopEngine();

private:
	CommandHandler();
	
	//sends a command to engine
	void sendCmdToEngine(cmd);
	//sud state machine to run a singilar cmd sequense, returns true when done.
	bool runSequence();

	static CommandHandler* instance;
	
	QueueArray<cmdSequense> commandQueue;

};
