#pragma once
#include "MeUltraSonicSensor.h"
class SensorController
{
public:
    ~SensorController();

    static SensorController* getInstance()
	{
		static SensorController* instance = nullptr;
		if (instance == nullptr)
			instance = new SensorController();
		return instance;
	}
    double getUltrasonicValue();

private:
    SensorController();
    MeUltrasonicSensor ultraSensor = MeUltrasonicSensor(PORT_10);
};
