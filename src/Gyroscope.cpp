#include <Gyroscope.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

void Dn_Gyroscope::init(uint8_t i2cAddr)
{
    mag = Adafruit_HMC5883_Unified(i2cAddr);

    if (!mag.begin())
    {
        Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
        while (1)
            ;
    }

    /* Display some basic information on this sensor */
    displaySensorDetails();
}

float Dn_Gyroscope::readDegree()
{
    sensors_event_t event;
    mag.getEvent(&event);

    float heading = atan2(event.magnetic.y, event.magnetic.x);

    float declinationAngle = 0.22;
    heading += declinationAngle;

    if (heading < 0)
        heading += 2 * PI;

    if (heading > 2 * PI)
        heading -= 2 * PI;

    float headingDegrees = heading * 180 / M_PI;

    Serial.print("Heading (degrees): ");
    Serial.println(headingDegrees);

    delay(100);
    return headingDegrees;
}

void Dn_Gyroscope::displaySensorDetails(void)
{
    sensor_t sensor;
    mag.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print("Sensor:       ");
    Serial.println(sensor.name);
    Serial.print("Driver Ver:   ");
    Serial.println(sensor.version);
    Serial.print("Unique ID:    ");
    Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    ");
    Serial.print(sensor.max_value);
    Serial.println(" uT");
    Serial.print("Min Value:    ");
    Serial.print(sensor.min_value);
    Serial.println(" uT");
    Serial.print("Resolution:   ");
    Serial.print(sensor.resolution);
    Serial.println(" uT");
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
}