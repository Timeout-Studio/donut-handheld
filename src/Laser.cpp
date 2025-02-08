#include "Laser.h"
#include <Arduino.h>

void Dn_Laser::init(uint8_t pin)
{
    this->pin = pin;
    this->status = false;
    pinMode(this->pin, OUTPUT);
    digitalWrite(pin, LOW);
}

bool Dn_Laser::getStatus()
{
    return this->status;
}

bool Dn_Laser::toggle()
{
    this->status = !this->status;
    digitalWrite(this->pin, this->status ? HIGH : LOW);
    return this->status;
}