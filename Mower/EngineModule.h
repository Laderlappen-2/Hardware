#include <MeAuriga.h>
#include <MeEncoderOnBoard.h>


class EngineModule{

public:

struct cmd
    {
        int speed;
        int turnRaduis;
        int time_ms;
    };

EngineModule();
EngineModule(int, int);


void run(){}

void setCommand(cmd){}

bool isReady(){}

void stopp(){}



private:

MeEncoderOnBoard Encoder_1;
MeEncoderOnBoard Encoder_2;

void execute_command(cmd){}

bool _ready;

cmd current_command;

};
