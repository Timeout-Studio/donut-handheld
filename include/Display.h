#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "PinDefinition.h" // Get pin definition of TFT_eSPI

// Custom Fonts Declare
LV_FONT_DECLARE(RMP1c_r_16)
LV_FONT_DECLARE(IBMPM_m_72)
LV_FONT_DECLARE(IBMPM_m_12)
LV_FONT_DECLARE(IBMPM_b_40)

LV_IMG_DECLARE(indicator)

// PWM Channel Assign
#define LCD_BL_PWM_CHANNEL 1

// Display Size
#define MY_DISP_HOR_RES (240)
#define MY_DISP_VER_RES (240)

enum Dn_Mode
{
    GAME,
    DEBUG
};

class Dn_Display
{
private:
    // Component Declaration
    lv_obj_t *container;
    lv_obj_t *circle;
    lv_obj_t *angleNum;
    lv_obj_t *hanLabel;
    lv_obj_t *angleSymbol;
    lv_obj_t *angleArc;
    lv_obj_t *indicatorLine;

    // Basic Setting
    void lvglInit();

    // Component Set
    void componentsReset();
    void gameComponentSettings();
    void debugComponentSettings();

public:
    // Display System
    void init();
    void routine(); // lv_task_handler
    void setBacklight(uint8_t range);
    void setBackgroundColor(lv_color_t color);
    void setDisplayMode(Dn_Mode mode);
    void displayUpdateLoop(int16_t angle);

    // Game Display
    void gameDisplay(int16_t angle);

    // Debug Display
    void debugDisplay(char IPAddress, char MAC, uint8_t batteryPertentage, bool laserStatus);
};