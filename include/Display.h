#pragma once

class Dn_Display
{
private:
    void initInformation();

public:
    // Display System
    void init();
    void routine(void); // lv_task_handler
    void setBacklight(uint8_t range);

    // Game Display
    void gameBackground();
    void gameBackgrgameAngleDisplayound(int16_t angle);
    void gameAnswerEvent(bool isAnswer);

    // Debug Display
    void debugBackground();
    void debugInformation(char IPAddress, char MAC, uint8_t batteryPertentage, bool laserStatus);
};