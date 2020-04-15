#include "EngineModule.h"

EngineModule::EngineModule(){}

EngineModule::EngineModule(int slot1, int slot2)
{
  Wheel_Right = new MeEncoderOnBoard(slot1);
  Wheel_Left = new MeEncoderOnBoard(slot2);
  
}
  
void EngineModule::setCommand(cmd command)
{
    if (current_command == nullptr)
       current_command = new cmd;
    current_command->speed = command.speed;
    current_command->turnRadius = command.turnRadius; 
    current_command->time_ms = command.time_ms; 

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
  static long startWait = 0;
  
  switch(state)
  {
    case idle:
    if (current_command != NULL)
    {
      _ready = false;
      state = update;
    }
    else 
      stopp();
      break;
    case update:
      execute_command(current_command);
      startWait = millis();
      state = wait;
      break;
    case wait:
      if (millis() > startWait + current_command->time_ms)
      {
        delete(current_command);
        current_command = nullptr;
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
  constrain(command->speed, -255, 255);
  
  float rRight, rLeft;
  rRight = command->turnRadius - _wheelToWheelGap / 2;
  rLeft = command->turnRadius + _wheelToWheelGap / 2;

  if(command->turnRadius > 0)
  {
    float ratio = rRight / rLeft;
    Wheel_Right->setMotorPwm(-command->speed * ratio);
    Wheel_Left->setMotorPwm(command->speed);
  }

  else
  {
    float ratio = rLeft / rRight;
    Wheel_Right->setMotorPwm(-command->speed);
    Wheel_Left->setMotorPwm(command->speed * ratio);
  }
}

void EngineModule::stopp()
{
  Wheel_Right->setMotorPwm(0);
  Wheel_Left->setMotorPwm(0);
}