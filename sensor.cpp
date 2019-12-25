//Kevin Ray 2019
#include "utils/sensor.h"
//Constructor to int. sensors, example: Sensor.sensors(1,1)
//Can easily be extended with other sensors
Sensor::Sensor(bool imuInit, bool distanceInit)
{
    if(imuInit)
        this->IMUInit();
    if(distanceInit)
        this->VL53Init();
}

void Sensor::VL53Init()
{
	distanceSensor.initialize();
}
void Sensor::IMUInit()
{
	mag.initialize();
}

//VL53L0x Distance Sensor Functions

//configDistance - optional function to configure VL53L0x Distance sensor parameters
//If not specified, default values are initialized
void Sensor::configDistance(uint16_t timeout, float signalRate, bool longRange, bool highSpeed, bool highAccuracy)
{
    if(timeout != 0)
    distanceSensor.setTimeout(timeout);
    if(signalRate != 0)
    distanceSensor.setSignalRateLimit(signalRate);

    if(longRange)
    {
        distanceSensor.setVcselPulsePeriod(VcselPeriodPreRange, 18);
        distanceSensor.setVcselPulsePeriod(VcselPeriodFinalRange, 14);
    }
    if(highSpeed)
        distanceSensor.setMeasurementTimingBudget(20000);
    else if(highAccuracy)
        distanceSensor.setMeasurementTimingBudget(200000);

}

//Return distance sensor reading
uint16_t Sensor::readDistance()
{
    return distanceSensor.readRangeSingleMillimeters();
}

//GY-85 IMU Functions

int Sensor::getHeading()
{
    mag.getHeading(&mx, &my, &mz);

    int heading = atan2(my, mx);
    if(heading < 0)
      heading += 2 * M_PI;

    return heading * 180/M_PI;
}

