#include "SensorController.h"


SensorController::SensorController(){}

SensorController::~SensorController(){}


double SensorController::getUltrasonicValue()
{
  return ultraSensor.distanceCm();
}
