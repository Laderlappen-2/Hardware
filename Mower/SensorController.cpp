#include "SensorController.h"


SensorController::SensorController(){}


double SensorController::getUltrasonicValue()
{
  return ultraSensor.distanceCm();
}
