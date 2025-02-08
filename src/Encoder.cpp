#include "Encoder.h"
#include <Arduino.h>

void Dn_Encoder::init(uint8_t s1Pin, uint8_t s2Pin, uint8_t keyPin)
{
    this->s1Pin = s1Pin;
    this->s2Pin = s2Pin;
    this->keyPin = keyPin;
    pinMode(this->s1Pin, INPUT_PULLUP);
    pinMode(this->s2Pin, INPUT_PULLUP);
    pinMode(this->keyPin, INPUT_PULLUP);
}

int Dn_Encoder::read()
{
    return 1;
}