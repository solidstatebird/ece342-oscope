#ifndef _ILI9341_t3_font_LiberationSerifBold_
#define _ILI9341_t3_font_LiberationSerifBold_

#if __has_include(<RA8875.h>)
	#include "RA8875.h"
#elif __has_include(<ILI9488_t3.h>)
	#include "ILI9488_t3.h"
#elif __has_include(<ILI9341_t3n.h>)
	#include "ILI9341_t3n.h"
#elif __has_include(<ILI9341_t3.h>)
	#include "ILI9341_t3.h"
#elif __has_include(<ST7735_t3.h>)
	#include "ST7735_t3.h"
#elif __has_include(<HX8357_t3n.h>)
	#include "HX8357_t3n.h"
#elif __has_include(<GC9A01A_t3n.h>)
	#include "GC9A01A_t3n.h"
#else
	#include "def_ili9341_fonts.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern const ILI9341_t3_font_t LiberationSerif_8_Bold;
extern const ILI9341_t3_font_t LiberationSerif_9_Bold;
extern const ILI9341_t3_font_t LiberationSerif_10_Bold;
extern const ILI9341_t3_font_t LiberationSerif_11_Bold;
extern const ILI9341_t3_font_t LiberationSerif_12_Bold;
extern const ILI9341_t3_font_t LiberationSerif_13_Bold;
extern const ILI9341_t3_font_t LiberationSerif_14_Bold;
extern const ILI9341_t3_font_t LiberationSerif_16_Bold;
extern const ILI9341_t3_font_t LiberationSerif_18_Bold;
extern const ILI9341_t3_font_t LiberationSerif_20_Bold;
extern const ILI9341_t3_font_t LiberationSerif_24_Bold;
extern const ILI9341_t3_font_t LiberationSerif_28_Bold;
extern const ILI9341_t3_font_t LiberationSerif_32_Bold;
extern const ILI9341_t3_font_t LiberationSerif_40_Bold;
extern const ILI9341_t3_font_t LiberationSerif_48_Bold;
extern const ILI9341_t3_font_t LiberationSerif_60_Bold;
extern const ILI9341_t3_font_t LiberationSerif_72_Bold;
extern const ILI9341_t3_font_t LiberationSerif_96_Bold;

#ifdef __cplusplus
} // extern "C"
#endif

#endif
