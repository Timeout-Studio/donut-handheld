#pragma once
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

class Dn_Gyroscope
{
private:
    Adafruit_HMC5883_Unified mag;
    void displaySensorDetails(void);
public:
    float calibrationFactor = 0.0;
    void init(uint8_t address);
    float readAngle();
    float readCalibratedAngle();
    void calibrate();
};