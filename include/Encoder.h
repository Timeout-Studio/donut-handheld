#pragma once

class Dn_Encoder
{
private:
    uint8_t s1Pin;
    uint8_t s2Pin;
    uint8_t keyPin;

    volatile int8_t encoderValue;
    volatile bool rotating;
    bool buttonPressed;
    volatile uint32_t lastInterruptTime;

    static void IRAM_ATTR isrHandler(void *arg);
    void IRAM_ATTR readEncoder();

public:
    void init(uint8_t s1Pin, uint8_t s2Pin, uint8_t keyPin);
    int8_t getValue();
    bool hasRotated();
    bool isButtonPressed();
};