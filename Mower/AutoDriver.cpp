#include "AutoDriver.h"

AutoDriver::AutoDriver(){}

void AutoDriver::run()
{
  enum state_e
  {
    idle,
    avoidCollision,
    drive
  };
  static state_e state = idle;
}
