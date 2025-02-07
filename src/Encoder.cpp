#include "Encoder.h"
#include <Arduino.h>

void Dn_Encoder::init(int s1Pin, int s2Pin, int keyPin)
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