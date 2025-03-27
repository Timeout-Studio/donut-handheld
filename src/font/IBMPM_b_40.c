/*******************************************************************************
 * Size: 40 px
 * Bpp: 4
 * Opts: --no-compress --no-prefilter --bpp 4 --size 40 --font IBMPlexMono-Medium.ttf --symbols ° --format lvgl -o IBMPM_b_40.c --force-fast-kern-format
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include <lvgl.h>
#endif

#ifndef IBMPM_B_40
#define IBMPM_B_40 1
#endif

#if IBMPM_B_40

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+00B0 "°" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x29, 0xef, 0xfe, 0x92, 0x0, 0x0,
    0x0, 0x8, 0xff, 0xff, 0xff, 0xff, 0x70, 0x0,
    0x0, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xf7, 0x0,
    0x3, 0xff, 0xfd, 0x40, 0x4, 0xdf, 0xff, 0x30,
    0xa, 0xff, 0xe1, 0x0, 0x0, 0x1e, 0xff, 0xa0,
    0xf, 0xff, 0x70, 0x0, 0x0, 0x7, 0xff, 0xf0,
    0x1f, 0xff, 0x30, 0x0, 0x0, 0x3, 0xff, 0xf1,
    0x1f, 0xff, 0x30, 0x0, 0x0, 0x4, 0xff, 0xf1,
    0xf, 0xff, 0x70, 0x0, 0x0, 0x7, 0xff, 0xe0,
    0xa, 0xff, 0xe1, 0x0, 0x0, 0x1e, 0xff, 0xa0,
    0x3, 0xff, 0xfd, 0x51, 0x15, 0xef, 0xff, 0x20,
    0x0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x0,
    0x0, 0x7, 0xff, 0xff, 0xff, 0xff, 0x60, 0x0,
    0x0, 0x0, 0x28, 0xdf, 0xfd, 0x81, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 384, .box_w = 16, .box_h = 15, .ofs_x = 4, .ofs_y = 14}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 176, .range_length = 1, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t IBMPM_b_40 = {
#else
lv_font_t IBMPM_b_40 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = -14,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -5,
    .underline_thickness = 3,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if IBMPM_B_40*/

