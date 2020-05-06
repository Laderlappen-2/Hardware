#include "EngineModule.h"

EngineModule::EngineModule(){}

EngineModule::EngineModule(int slot1, int slot2)
{
  Wheel_Right = new MeEncoderOnBoard(slot1);
  Wheel_Left = new MeEncoderOnBoard(slot2);
  
}
  
void EngineModule::setCommand(cmd command)
{
    current_command.speed = command.speed;
    current_command.turnRadius = command.turnRadius; 
    current_command.time_ms = command.time_ms; //TODO if time = negative, drive forever and set _ready to true

    _ready = false;
}

void EngineModule::setTurn(int turn)
{

}

void EngineModule::setSpeed(int speed)
{

}

bool EngineModule::isReady()
{
  return _ready;
}

void EngineModule::run()
{
  enum state_s
  {
    idle,
    update,
    wait,  
  };

  Wheel_Right->loop();
  Wheel_Left->loop();
  static state_s state = idle;
  static unsigned long startWait = 0;
  
  switch(state)
  {
    case idle:
    if (!isReady())
    {
      state = update;
    }
    else 
      stopp();
      break;
    case update:
      execute_command(&current_command);
      startWait = millis();
      state = wait;
      break;
    case wait:
        if (current_command.time_ms < 0)
            _ready = true;
        else if (millis() > startWait + current_command.time_ms)
        {
        _ready = true;
        state = idle;
        
        }
      break;
    default:
                        // TODO felhantering
      break;
  }
}

void EngineModule::execute_command(cmd *command)
{
    //---New code that handles a range (-100 <-> 100)
    //->speed = constrain(command->speed, -100, 100);
    
    //int speed = fmax(command->speed, command->turnRadius);
    //command->speed = speed;
    
    //command->speed = command->speed * 2.55; // set range to -255 <-> 255 

    //--- End
    command->speed = constrain(command->speed, -255, 255); // make sure we don't anger the motors

    /*if(command->turnRadius == 0)
    {
        Wheel_Right->setMotorPwm(-command->speed - _rightWheelOffset);
        Wheel_Left->setMotorPwm(command->speed + _leftWheelOffset);
        return;
    }*/

    if (command->turnRadius == NO_TURN)
    {
    Wheel_Right->setMotorPwm(-command->speed - _rightWheelOffset);
    Wheel_Left->setMotorPwm(command->speed + _leftWheelOffset);
    return;
    }

    //command->turnRadius = command->turnRadius * 2.55;
    //should convert the new values to the old ones
    float rRight, rLeft;
    rRight = command->turnRadius - (_wheelToWheelGap / 2);
    rLeft = command->turnRadius + (_wheelToWheelGap / 2);

    if(command->turnRadius > 0)
    {
    float ratio = rRight / rLeft;
    Wheel_Right->setMotorPwm(-command->speed * ratio - _rightWheelOffset);
    Wheel_Left->setMotorPwm(command->speed + _leftWheelOffset);
    }

    else
    {
    float ratio = rLeft / rRight;
    Wheel_Right->setMotorPwm(-command->speed - _rightWheelOffset);
    Wheel_Left->setMotorPwm(command->speed * ratio + _leftWheelOffset);
    }

  
}

void EngineModule::stopp()
{
  Wheel_Right->setMotorPwm(0);
  Wheel_Left->setMotorPwm(0);
}
