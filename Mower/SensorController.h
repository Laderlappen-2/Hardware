#include "MeUltraSonicSensor.h"
class SensorController
{
public:

    static SensorController* getInstance();

    double getUltrasonicValue();

    ~SensorController();
private:

    SensorController();

    MeUltrasonicSensor ultraSensor = MeUltrasonicSensor(PORT_10);
};
