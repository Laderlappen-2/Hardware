#include "MeUltraSonicSensor.h"
class SensorController
{
  public:

  SensorController();

  double getUltrasonicValue();


  private:


  MeUltrasonicSensor ultraSensor = MeUltrasonicSensor(PORT_10);
};
