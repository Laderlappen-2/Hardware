#pragma once 
#include <MeEncoderOnBoard.h>
#include <math.h>

class EngineModule{

public:

#define NO_TURN 3001

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

    void setTurn(int);

    void setSpeed(int);

    bool isReady();

    void stopp();

private:

    MeEncoderOnBoard *Wheel_Right;
    MeEncoderOnBoard *Wheel_Left;

    void execute_command(cmd*);

    bool _ready = true;

    cmd current_command;

    const float _wheelToWheelGap = 14.6;

    const int _rightWheelOffset = 17;
    const int _leftWheelOffset = 0;
};
