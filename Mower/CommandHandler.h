#pragma once

#include <Arduino.h>

class CommandHandler
{
public:

	struct cmd
	{
		int speed;
		int turnRaduis;
		int time_ms;
	};
	struct cmdSequense
	{
		
	};

	~CommandHandler();
	CommandHandler* getInstance();

	void run();
	void addCommand(cmd);
	void addCommand(cmd[]);
	void addCommand(cmdSequense);
	void clear();
	void stopEngine();

private:
	CommandHandler();

	static CommandHandler* instance;
};

