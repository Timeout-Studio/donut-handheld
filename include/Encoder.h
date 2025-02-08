#pragma once

class Dn_Encoder {
private:
    int s1Pin;
    int s2Pin;
    int keyPin;

    int lastClockwiseState;
    int lastCounterClockwiseState;
    int pwmValue;

public:
    void init(uint8_t s1Pin, uint8_t s2Pin, uint8_t keyPin);
    int read();
};