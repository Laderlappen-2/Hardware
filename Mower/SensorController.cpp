#include "SensorController.h"


SensorController::SensorController(){}

SensorController::~SensorController(){}

SensorController* SensorController::getInstance()
{
    static SensorController* instance = nullptr;
    if (instance == nullptr)
        instance = new SensorController();
    return instance;
}

double SensorController::getUltrasonicValue()
{
  return ultraSensor.distanceCm();
}
