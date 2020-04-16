#pragma once 
#include <MeEncoderOnBoard.h>

class EngineModule{

public:

#define NO_TURN 3001 //preset value for driving forward

struct cmd
    {
        int speed;      // speed in pwm
        int turnRadius; // in cm
        int time_ms;
    };

EngineModule();

EngineModule(int, int);

void run();

void setCommand(cmd);

bool isReady();

void stopp();

private:

MeEncoderOnBoard *Wheel_Right;
MeEncoderOnBoard *Wheel_Left;

void execute_command(cmd*);

bool _ready = true;

cmd *current_command;

const float _wheelToWheelGap = 14.6; //physical value 

const int _rightWheelOffset = 17;    //value to compensate for a slower motor
const int _leftWheelOffset = 0;
};
