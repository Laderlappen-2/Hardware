#include "AutoDriver.h"

AutoDriver::AutoDriver()
{
    sensorInstance = SensorController::getInstance();
    bluetoothInstance = BluetoothController::getInstance();
}

void AutoDriver::init()
{
}

void AutoDriver::onDeactivation()
{
}

void AutoDriver::onActivation()
{
}

void AutoDriver::run()
{
  enum state_e
  {
    drive,
    waitForCollision,
    handleCollision,
    
  };
  static state_e state = drive;        
  EngineModule::cmd command;
  EngineModule *engine = EngineModule::getInstance();
  static unsigned long startWaitTime = 0;
  const int reverseWait = 2000;

  switch(state)
  {
    case drive:
      engine->setSpeed(40);
      engine->setTurn(0);
      state = waitForCollision;
      break;

    case waitForCollision:
      if(sensorInstance->getUltrasonicValue() < safetyDistance)
      {
        String x = String(engine->getX());
        String y = String(engine->getY());
        Serial.print("@1," + x + ";" + y + "$");
        state = handleCollision;
        startWaitTime = millis();
      }
      break;

    case handleCollision:
      if(millis() < startWaitTime + reverseWait)
      {
        engine->setSpeed(-30);
        engine->setTurn(60);
      }
      else
        state = drive;
      break;

    default:
      break;
  }
}

void AutoDriver::listener(int[], int)
{
}
