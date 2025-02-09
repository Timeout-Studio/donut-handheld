#include "Encoder.h"
#include <Arduino.h>

void IRAM_ATTR Dn_Encoder::readEncoder()
{
    uint32_t now = millis();
        if (now - lastInterruptTime < 50) return;
        lastInterruptTime = now;

        if (digitalRead(s2Pin) == digitalRead(s1Pin)) {
            encoderValue = -1;
        } else {
            encoderValue = 1;
        }
        rotating = true;
}

void IRAM_ATTR Dn_Encoder::isrHandler(void *arg)
{
    Dn_Encoder *instance = static_cast<Dn_Encoder *>(arg);
    instance->readEncoder();
}

void Dn_Encoder::init(uint8_t s1Pin, uint8_t s2Pin, uint8_t keyPin)
{
    this->s1Pin = s1Pin;
    this->s2Pin = s2Pin;
    this->keyPin = keyPin;
    pinMode(this->s1Pin, INPUT_PULLUP);
    pinMode(this->s2Pin, INPUT_PULLUP);
    pinMode(this->keyPin, INPUT_PULLUP);

    attachInterruptArg(digitalPinToInterrupt(this->s2Pin), isrHandler, this, CHANGE);
}

int8_t Dn_Encoder::getValue() {
    return this->encoderValue;
}

bool Dn_Encoder::hasRotated() {
    if (this->rotating) {
        this->rotating = false;
        return true;
    }
    return false;
}

bool Dn_Encoder::isButtonPressed() {
    if (digitalRead(this->keyPin) == LOW && !this->buttonPressed) {
        this->buttonPressed = true;
        return true;
    } else if (digitalRead(this->keyPin) == HIGH && this->buttonPressed) {
        this->buttonPressed = false;
    }
    return false;
}