#include "MeAuriga.h"
#include "EngineModule.h"

EngineModule engine = EngineModule(SLOT1, SLOT2);


void setup()
{
  //Serial.begin(9600);
  
  EngineModule::cmd command = {200, 0, 2000};
  engine.setCommand(command);
}


void loop()
{  
  if(engine.isReady())
  {
    engine.setCommand(EngineModule::cmd{-100, 0, 2000});
  }
  engine.run();
}
