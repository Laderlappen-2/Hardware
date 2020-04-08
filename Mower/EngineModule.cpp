#include "EngineModule.h"
  //Encoder_1.setMotorPwm(-255);
  //Encoder_2.setMotorPwm(255);



EngineModule::EngineModule(){}

EngineModule::EngineModule(int slot1, int slot2)
{
  Encoder_1 = MeEncoderOnBoard(slot1);
  Encoder_2 = MeEncoderOnBoard(slot2);
  
}

  
void EngineModule::setCommand(cmd command)
{
  current_command = command; 
  
}

bool EngineModule::isReady()
{
  return ready;
}

void EngineModule::run()
{
  enum state_s
  {
    idle,
    update,
    wait,  
  };
  
  Encoder_1.loop();
  Encoder_2.loop();
  static state_s state = idle;
  static long startWait = 0;
  
  switch(state)
  {
    case idle:
    if (current_command != null)
    {
      _ready = false;
      state = update;
    }
      break;
    case update:
      execute_command(current_command);
      startWait = millis();
      state = wait;
      break;
    case wait:
      if (millis() > startWait + current_command.time_ms)
      {
        current_command = NULL;
        _ready = true;
        state = idle;
        
      }
      break;
    default:
                        // TODO felhantering
      break;
  }
}

void EngineModule::execute_command(cmd command)
{
  constrain(command.speed, -255, 255);
  Encoder_1.setMotorPwm(command.speed);
  Encoder_2.setMotorPwm(-command.speed);

  
}
