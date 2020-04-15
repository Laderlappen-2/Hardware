#pragma once 
#include <MeEncoderOnBoard.h>

class EngineModule{

public:

struct cmd
    {
        int speed;
        int turnRadius;
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

bool _ready;

cmd *current_command;

const float _wheelToWheelGap = 14.6;

};
