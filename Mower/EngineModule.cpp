#include "EngineModule.h"

EngineModule::EngineModule(){}

EngineModule::EngineModule(int slot1, int slot2)
{
  Wheel_Right = new MeEncoderOnBoard(slot1);
  Wheel_Left = new MeEncoderOnBoard(slot2);
  
}
  
void EngineModule::setCommand(cmd command)
{
    Serial.print("\nsetCommand START!!!!!");
    //delay(1);
    /*if (current_command == nullptr)
    {
       Serial.print("\nIFF START!!!!!(engine)");
       current_command = new cmd{0,0,0};
       Serial.print("\nIFF END!!!!!(engine)");
    }*/
    
    current_command.speed = command.speed;
    current_command.turnRadius = command.turnRadius; 
    current_command.time_ms = command.time_ms; 
    
    //*current_command = command;
    
    _ready = false;
    Serial.print("setCommand END!!!!!");
}

bool EngineModule::isReady()
{
  //Serial.print((current_command == nullptr) ? "Ready" : "Note ready");
  //return current_command == nullptr;
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
    if (!isReady())
    {
      //_ready = false;
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
      if (millis() > startWait + current_command.time_ms)
      {
        //delete(current_command);
        //current_command = nullptr;
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
  command->speed = constrain(command->speed, -255, 255);

  if (command->turnRadius == NO_TURN)
  {
    Wheel_Right->setMotorPwm(-command->speed - _rightWheelOffset);
    Wheel_Left->setMotorPwm(command->speed + _leftWheelOffset);
    return;
  }
  
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
