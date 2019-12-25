#include <math.h>
#include "lib/VL53L0X.h"
//#include "lib/ADXL345.h"
#include "lib/HMC5883L.h"
//#include "lib/ITG3200.h"


class Sensor
{
public:

    Sensor(bool imuInit, bool distanceInit);
    uint16_t readDistance();
    void configDistance(uint16_t timeout, float signalRate, bool longRange, bool highSpeed, bool highAccuracy);
    int getHeading();

private:
    int16_t mx, my, mz;
    void VL53Init();
    void IMUInit();
    VL53L0X distanceSensor;
    HMC5883L mag;
};
