#include <Display.h>

TFT_eSPI tft = TFT_eSPI();

void Dn_Display::gameComponentSettings()
{
    container = lv_obj_create(lv_scr_act());
    lv_obj_remove_style(container, NULL, LV_PART_MAIN);
    lv_obj_set_size(container, 240, 240);
    lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);

    circle = lv_obj_create(container);
    lv_obj_set_size(circle, 10, 10);
    lv_obj_set_style_radius(circle, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(circle, lv_color_hex(0x666666), LV_PART_MAIN);
    lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
    lv_obj_align(circle, LV_ALIGN_CENTER, 0, -110);

    angleNum = lv_label_create(container);
    lv_obj_set_style_text_font(angleNum, &IBMPM_m_72, 0);
    static lv_style_t angleNumStyle;
    lv_style_init(&angleNumStyle);
    lv_style_set_text_letter_space(&angleNumStyle, -5);
    lv_style_set_text_color(&angleNumStyle, lv_color_hex(0xffffff));
    lv_obj_add_style(angleNum, &angleNumStyle, LV_PART_MAIN);
    lv_obj_align(angleNum, LV_ALIGN_CENTER, 0, 10);
    lv_label_set_text(angleNum, "0");

    hanLabel = lv_label_create(container);
    lv_obj_set_style_text_font(hanLabel, &RMP1c_r_16, 0);
    lv_label_set_text(hanLabel, "距離聲源");
    lv_obj_align(hanLabel, LV_ALIGN_CENTER, 0, -30);
    static lv_style_t hanLabelStyle;
    lv_style_init(&hanLabelStyle);
    lv_style_set_text_letter_space(&hanLabelStyle, 2);
    lv_style_set_text_color(&hanLabelStyle, lv_color_hex(0xffffff));
    lv_obj_add_style(hanLabel, &hanLabelStyle, LV_PART_MAIN);

    angleSymbol = lv_label_create(container);
    lv_obj_set_style_text_font(angleSymbol, &IBMPM_b_40, 0);
    lv_label_set_text(angleSymbol, "°");
    lv_obj_align(angleSymbol, LV_ALIGN_CENTER, 50, -12);
    static lv_style_t angleSymbolStyle;
    lv_style_init(&angleSymbolStyle);
    lv_style_set_text_letter_space(&angleSymbolStyle, 0);
    lv_style_set_text_color(&angleSymbolStyle, lv_color_hex(0xffffff));
    lv_obj_add_style(angleSymbol, &angleSymbolStyle, LV_PART_MAIN);

    angleArc = lv_arc_create(container);
    lv_obj_remove_style(angleArc, NULL, LV_PART_KNOB); /*Be sure the knob is not displayed*/
    lv_obj_remove_style(angleArc, NULL, LV_PART_MAIN);
    lv_obj_clear_flag(angleArc, LV_OBJ_FLAG_CLICKABLE); /*To not allow adjusting by click*/
    lv_obj_set_size(angleArc, 224, 224);
    lv_arc_set_rotation(angleArc, 270);
    lv_arc_set_range(angleArc, 0, 360);
    lv_obj_set_style_arc_width(angleArc, 0, LV_PART_MAIN);
    lv_obj_set_style_arc_width(angleArc, 4, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(angleArc, lv_color_hsv_to_rgb(0, 100, 100), LV_PART_INDICATOR);
    lv_obj_center(angleArc);
    lv_arc_set_angles(angleArc, 0, 0);

    indicatorLine = lv_img_create(container);
    lv_img_set_src(indicatorLine, &indicator);
    lv_obj_set_style_border_width(indicatorLine, 0, LV_PART_MAIN);
    lv_obj_align(indicatorLine, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_pivot(indicatorLine, 4, 120);
}

void Dn_Display::debugComponentSettings()
{
}

// LVGL disp_flush, which connect between LVGL and TFT_eSPI by pixel
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

// Display System
void Dn_Display::init()
{
    lvglInit(); // MUST RUN!
    gameComponentSettings();
}

void Dn_Display::lvglInit()
{
    lv_init();
    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(2);

    // Assign pwm channel
    ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);
    ledcAttachPin(TFT_BL, LCD_BL_PWM_CHANNEL);

    // Create a buffer for drawing and initialize the display
    static lv_disp_draw_buf_t draw_buf_dsc;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 10];
    static lv_color_t buf_2[MY_DISP_HOR_RES * 10];
    lv_disp_draw_buf_init(&draw_buf_dsc, buf_1, buf_2, MY_DISP_HOR_RES * 10);

    // Register the display in LVGL
    static lv_disp_drv_t disp_drv; // Descriptor of a display driver
    lv_disp_drv_init(&disp_drv);   // Basic initialization

    // Set the resolution of the display
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /* Used to copy the buffer's content to the display */
    disp_drv.flush_cb = disp_flush;

    // Set a display buffer
    disp_drv.draw_buf = &draw_buf_dsc;

    disp_drv.full_refresh = 1;

    // Finally register the driver
    lv_disp_drv_register(&disp_drv);

    // Style & components settings
    setBackgroundColor(lv_color_hex(0x000000));

    // routine();
    // Serial.println("hello1");
}

void Dn_Display::routine(uint16_t angle)
{
    lv_task_handler();
    Serial.println(angle);
    lv_img_set_angle(indicatorLine, angle * 10);
    lv_arc_set_angles(angleArc, 0, angle);
}

void Dn_Display::setBacklight(uint8_t range)
{
    ledcWrite(LCD_BL_PWM_CHANNEL, range);
}

void Dn_Display::setBackgroundColor(lv_color_t color)
{
    lv_obj_set_style_bg_color(lv_scr_act(), color, LV_PART_MAIN);
}

// Game Display
void Dn_Display::gameAngleDisplay(int16_t angle)
{
    // Serial.print("angle: ");
    // Serial.println(angle);

    // int16_t target_angle = (angle >= 0) ? angle : (360 + angle);
    // setAngle(target_angle);
    // lv_label_set_text_fmt(angleNum, "%d", angle);
    // lv_obj_set_style_arc_color(angleArc, lv_color_hsv_to_rgb(angle, 100, 100), LV_PART_INDICATOR);

    // lv_obj_set_style_transform_angle(indicator, angle, LV_PART_MAIN);
}

void Dn_Display::setAngle(int16_t target_angle)
{
    // static lv_anim_t angleArcSmooth;
    // lv_anim_init(&angleArcSmooth);
    // lv_anim_set_var(&angleArcSmooth, angleArc);

    // lv_anim_set_exec_cb(&angleArcSmooth, [](void *var, int32_t v)
    //                     { lv_arc_set_angles((lv_obj_t *)var, 0, v); });
    // lv_anim_set_time(&angleArcSmooth, 10);
    // lv_anim_set_path_cb(&angleArcSmooth, lv_anim_path_ease_in);
    // lv_anim_set_values(&angleArcSmooth, lv_arc_get_angle_end(angleArc), target_angle);
    // lv_anim_start(&angleArcSmooth);
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
