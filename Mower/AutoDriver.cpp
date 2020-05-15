#include "AutoDriver.h"

AutoDriver::AutoDriver()
{
    sensorInstance = SensorController::getInstance();
    commandInstance = CommandHandler::getInstance();
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
    commandInstance->stopEngine();
    commandInstance->clear();
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
        engine->updatePosition();
        //bluetoothInstance->send(bluetoothInstance->EventType_e::crachAvoidance, engine->getX(), engine->getY());
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

/*bool AutoDriver::runAvoidCollision()
{
  enum state_e
  {
    reverse,
    waitReverse,
    turn,
    waitTurn
  };
  static state_e state = reverse;
  const int reverseSpeed = 50;
  const int reverseWait = 1000;
  const int turnSpeed = 25;
  const int turnWait = random(1000, 2500);
  static unsigned long startWaitTime = 0;
  EngineModule *engine = EngineModule::getInstance();
  
  switch(state)
  {
    case reverse:
      engine->setSpeed(-reverseSpeed);
      engine->setTurn(0);
      startWaitTime = millis();
      state = waitReverse;
      break;
    case waitReverse:
      if(millis() > startWaitTime + reverseWait)
        state = turn;
      break;
    case turn:
      engine->setSpeed(50);
      engine->setTurn(100);
      startWaitTime = millis();
      state = waitTurn;
      break;
    case waitTurn:
      if(millis() > startWaitTime + turnWait)
      {
        state = reverse;
        return true;
      }
      break;

    default:
      break;
  }
  
  return false;
}*/
