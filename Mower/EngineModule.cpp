#include "EngineModule.h"
  //Encoder_1.setMotorPwm(-255);
  //Encoder_2.setMotorPwm(255);



EngineModule::EngineModule(){}

EngineModule::EngineModule(int slot1, int slot2)
{
  Encoder_1 = new MeEncoderOnBoard(slot1);
  Encoder_2 = new MeEncoderOnBoard(slot2);
  
}

  
void EngineModule::setCommand(cmd command)
{
    if (current_command == nullptr)
       current_command = new cmd;
    current_command->speed = command.speed;
    current_command->turnRaduis = command.turnRaduis; 
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
  
  Encoder_1->loop();
  Encoder_2->loop();
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
  Encoder_1->setMotorPwm(-command->speed);
  Encoder_2->setMotorPwm(command->speed);

  
}

void EngineModule::stopp()
{
  Encoder_1->setMotorPwm(0);
  Encoder_2->setMotorPwm(0);
}
