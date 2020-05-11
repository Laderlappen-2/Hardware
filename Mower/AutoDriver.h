#pragma once
#include "AbsDriver.h"
//#include <time.h>

class AutoDriver : public AbsDriver
{
public:

  AutoDriver();

  void init();
  void run();
  void listener(int[], int);
  void onActivation();
  void onDeactivation();
private:

    CommandHandler* commandInstance;
    SensorController* sensorInstance;
};
