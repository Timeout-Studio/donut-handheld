#include <Display.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "PinDefinition.h" // Get pin definition of TFT_eSPI

// #include "examples/lv_examples.h"

#define LCD_BL_PWM_CHANNEL 1

// Display Size
#define MY_DISP_HOR_RES (240)
#define MY_DISP_VER_RES (240)

TFT_eSPI tft = TFT_eSPI();

// LVLG disp_flush, which connect between LVGL and TFT_eSPI by pixel
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv);
}

void Dn_Display::init()
{
    lv_init();
    tft.init();
    tft.fillScreen(TFT_BLACK);

    // Assign pwm channel
    ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);
    ledcAttachPin(TFT_BL, LCD_BL_PWM_CHANNEL);
    setBacklight(255);

    // Create a buffer for drawing and initialize the display
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 10];
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);

    // Register the display in LVGL
    static lv_disp_drv_t disp_drv; // Descriptor of a display driver
    lv_disp_drv_init(&disp_drv);   // Basic initialization

    // Set up the functions to access your display

    // Set the resolution of the display
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /* Used to copy the buffer's content to the display */
    disp_drv.flush_cb = disp_flush;

    // Set a display buffer
    disp_drv.draw_buf = &draw_buf_dsc_1;

    // Finally register the driver
    lv_disp_drv_register(&disp_drv);
}

void Dn_Display::routine(void)
{
    lv_task_handler();
}

void Dn_Display::setBacklight(uint8_t range)
{
    ledcWrite(LCD_BL_PWM_CHANNEL, range);
}

// Init Content
void Dn_Display::initInformation()
{
}

// Game Display
void Dn_Display::gameBackground()
{
}

void Dn_Display::gameBackgrgameAngleDisplayound(int16_t angle)
{
}

void Dn_Display::gameAnswerEvent(bool isAnswer)
{
}

// Debug Display
void Dn_Display::debugBackground()
{
}

void Dn_Display::debugInformation(char IPAddress, char MAC, uint8_t batteryPertentage, bool laserStatus)
{
}
