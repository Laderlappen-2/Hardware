#include "MeAuriga.h"
#include "EngineModule.h"

EngineModule engine = EngineModule(SLOT1, SLOT2);

void setup()
{
  //Serial.begin(9600);
  
  EngineModule::cmd command = {150, -30, 1500};
  engine.setCommand(command);
}


void loop()
{  
    static int increase=0;
    if(engine.isReady())
    {
        engine.setCommand(EngineModule::cmd{10 + increase, 50, 500});
        increase += 5;
    }
    engine.run();
}
