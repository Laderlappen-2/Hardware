#include "AutoDriver.h"

AutoDriver::AutoDriver()
{
    commandInstance = CommandHandler::getInstance();
}

void init()
{

}

void AutoDriver::onDeactivation()
{

}

void AutoDriver::onActivation()
{
    commandInstance->stopEngine();
    commandInstance->clear();
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
        if (sensorInstance->getUltrasonicValue() < safetyDistance)
            state = avoidCollision;
        break;
    case avoidCollision:
        commandInstance->stopEngine();
        commandInstance->clear();          // Should be here ?
        commandInstance->addCommand(-50, 0, 250);
        commandInstance->addCommand(0, 25, 100);

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
        commandInstance->addCommand(75, 0, -1);
        state = idle;
        break;
    default:
        break;
    }
}

void listener(int[], int)
{

}
