#include "AppInstructions.h"

AppInstructions::AppInstructions()
{
    
}

int AppInstructions::getVelocity()
{
  return _velocity;
}

void AppInstructions::setVelocity(int velocity)
{
  _velocity = velocity;
}

int AppInstructions::getTurn()
{
  return _turn;
}

void AppInstructions::setTurn(int turn)
{
  _turn = turn;
}

int AppInstructions::getLight()
{
  return _light;
}

void AppInstructions::setLight(int light)
{
  _light = light;
}

int AppInstructions::getHonk()
{
  return _honk;
}
void AppInstructions::setHonk(int honk)
{
  _honk = honk;
}

int AppInstructions::getAutoDrive()
{
  return _autoDrive;
}

void AppInstructions::setAutoDrive(int autodrive)
{
  _autoDrive = autodrive;
}

void AppInstructions::setInstructions(int velocity, int turn, int light, int honk, int autodrive)
{
  _velocity = velocity;
  _turn = turn;
  _light = light;
  _honk = honk;
  _autoDrive = autodrive;
}

void AppInstructions::setInstructionsAvailable(bool state)
{
  _instructionsAvailable = state;
}

bool AppInstructions::getInstructionsAvailable()
{
  return _instructionsAvailable;
}

    
