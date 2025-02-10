#pragma once

class Dn_Display
{
private:
    // void lv_demo(void);
    void initInformation();

public:
    void init();
    void routine(void);
    void setBacklight(uint8_t range);
    void gameBackground();
    void gameBackgrgameAngleDisplayound(int16_t angle);
    void gameAnswerEvent(bool isAnswer);
    void debugBackground();
    void debugInformation(char IPAddress, char MAC, uint8_t batteryPertentage, bool laserStatus);
};