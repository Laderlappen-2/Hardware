#pragma once
#include "AbsDriver.h"
//#include <time.h>

class AutoDriver : public AbsDriver
{
public:

  AutoDriver();

  void init() override;
  void run() override;
  void listener(int[], int) override;
  void onActivation() override;
  void onDeactivation() override;

private:

    bool runAvoidCollision();
    SensorController* sensorInstance;
    BluetoothController* bluetoothInstance;
};
