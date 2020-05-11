#pragma once 
#include <MeEncoderOnBoard.h>
#include <math.h>

class EngineModule{

public:

#define NO_TURN 0

    struct cmd
    {
        int speed;
        int turnRadius;
        int time_ms;
		inline void operator= (const cmd other)
		{
			speed = other.speed;
			turnRadius = other.turnRadius;
			time_ms = other.time_ms;
		}
		inline bool operator==(const cmd& c) const
		{
			return speed == c.speed && turnRadius == c.turnRadius && time_ms == c.time_ms;				
		}
    };

    

    EngineModule(int, int);

    static CommandHandler* getInstance() {
        static CommandHandler* instance = nullptr;
        if (instance == nullptr)
            instance = new CommandHandler();
        return instance;
    }

    void run();

    void setCommand(cmd);

    void setTurn(int);

    void setSpeed(int);

    bool isReady();

    void stopp();

private:
    EngineModule();

    MeEncoderOnBoard *Wheel_Right;
    MeEncoderOnBoard *Wheel_Left;

    void execute_command(cmd*);

    bool _ready = true;

    cmd current_command;

	const int motorPWMmax = 255;
	const int motorPWMmin = -255;

    const float _wheelToWheelGap = 14.6;

    const int _rightWheelOffset = 17;
    const int _leftWheelOffset = 0;
};
