#pragma once

class Dn_Display
{
private:
    void lv_demo(void);

public:
    void init();
    void routine(void);
    void setBacklight(uint8_t range);
};