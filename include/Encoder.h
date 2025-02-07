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
    void init(int s1Pin, int s2Pin, int keyPin);
    int read();
};