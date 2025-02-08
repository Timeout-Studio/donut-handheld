#pragma once

class Dn_Laser
{
private:
    uint8_t pin;
    bool status;

public:
    void init(uint8_t pin);
    bool toggle();
    bool getStatus();
};
