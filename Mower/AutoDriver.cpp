#include "AutoDriver.h"

AutoDriver::AutoDriver(){}

void init()
{

}

void onDeactivation()
{

}

void onActivation()
{
    CommandHandler::stopEngine();
    CommandHandler::clear();
}

void AutoDriver::run()
{
    enum state_e
    {
    idle,
    avoidCollision,
    drive
    };
    static state_e state = drive;

    switch (state)
    {
    case idle:
        if (sensorInstance->getUltrasonicValue < safetyDistance)
            state = avoidCollision;
        break;
    case avoidCollision:
        CommandHandler::stopEngine();
        CommandHandler::clear();          // Should be here ?
        CommandHandler::addCommand(EngineModule::cmd(-50, 0, 250));
        CommandHandler::addCommand(EngineModule::cmd(0, 25, 100));

        // TODO rapportera till BT
        clock_t start = clock();
        /*
            Send a message followed by 'start', which is time elapsed since program start
            Example @event,X,Y,klocka$ :
                @1,X,Y,start$
        
        */

        state = drive;
        break;
    case drive:
            CommandHandler::addCommand(EngineModule::cmd(75, 0, -1));
            state = idle;
        break;
    default:
        break;
    }
}

void listener(int[], int)
{

}


