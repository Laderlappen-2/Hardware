#include "AutoDriver.h"

AutoDriver::AutoDriver()
{
    sensorInstance = SensorController::getInstance();
    commandInstance = CommandHandler::getInstance();
    blueInstance = BluetoothController::getInstance();
}

void AutoDriver::init()
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
    EngineModule::cmd command;


    switch (state)
    {
    case idle:
        if (sensorInstance->getUltrasonicValue() < safetyDistance)
            state = avoidCollision;
        break;
    case avoidCollision:
        commandInstance->stopEngine();
        commandInstance->clear();          // Should be here ?
		//TODO Fix syntax

        //EngineModule::cmd command = { -50, 0, 250 };
        command.speed = -50;
        command.turnRadius = 0;
        command.time_ms = 250;
        commandInstance->addCommand(command);
        command.speed = 0;
        command.turnRadius = 25;
        command.time_ms = 100;
        commandInstance->addCommand(command);

        //commandInstance->addCommand(new EngineModule::cmd(-50, 0, 250));
        //commandInstance->addCommand(new EngineModule::cmd(0, 25, 100));

        // TODO rapportera till BT
        //clock_t start = clock();
        /*
            Send a message followed by 'start', which is time elapsed since program start
            Example @event,X,Y,klocka$ :
                @1,X,Y,start$
        
        */
        EngineModule* engine = EngineModule::getInstance();
        EngineModule::point_s point = engine->getPosition();
        blueInstance->send(BluetoothController::EventType_e::crachAvoidance, point._x, point._y);
        

        state = drive;
        break;
    case drive:

		//TODO fix syntax        EngineModule::cmd command = new EngineModule::cmd;
        //EngineModule::cmd command = new EngineModule::cmd;
        command.speed = 75;
        command.turnRadius = 0;
        command.time_ms = -1;
        commandInstance->addCommand(command);
        state = idle;
        break;
    default:
        break;
    }
}

void AutoDriver::listener(int[], int)
{

}
