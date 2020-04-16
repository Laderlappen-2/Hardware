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
    _ready = false;
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
  
  Wheel_Right->loop(); //run MeEncoderOnBoard state machines
  Wheel_Left->loop();
  static state_s state = idle;
  static long startWait = 0;
  
  switch(state)
  {
    case idle:
    if (current_command != NULL) //if we have a command incomming 
    {
      _ready = false;
      state = update;
    }
    else 
      stopp();
      break;
    case update:
      execute_command(current_command);
      startWait = millis(); // initiate timer
      state = wait;
      break;
    case wait:
      if (millis() > startWait + current_command->time_ms) // make sure we run the set time
      {
        delete(current_command);   //preventing memory leak
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
  command->speed = constrain(command->speed, -255, 255); // returns a safe value 

  if (command->turnRadius == NO_TURN) // if we want to drive exaclty forward
  {
    Wheel_Right->setMotorPwm(-command->speed - _rightWheelOffset);
    Wheel_Left->setMotorPwm(command->speed + _leftWheelOffset);
    return;
  }
  
  float rRight, rLeft;
  rRight = command->turnRadius - (_wheelToWheelGap / 2); 
  // compensate for the physical distance between the wheels
  rLeft = command->turnRadius + (_wheelToWheelGap / 2);

  if(command->turnRadius > 0) // turn right
  {
    float ratio = rRight / rLeft; // find a slower speed for the inner motor 
    Wheel_Right->setMotorPwm(-command->speed * ratio - _rightWheelOffset); // reduce the rWheel turn speed
    Wheel_Left->setMotorPwm(command->speed + _leftWheelOffset);
  }

  else// turn left
  {
    float ratio = rLeft / rRight; // find a slower speed for the inner motor
    Wheel_Right->setMotorPwm(-command->speed - _rightWheelOffset);
    Wheel_Left->setMotorPwm(command->speed * ratio + _leftWheelOffset); // reduce the lWheel turn speed
  }

  
}

void EngineModule::stopp()
{
  Wheel_Right->setMotorPwm(0);
  Wheel_Left->setMotorPwm(0);
}
