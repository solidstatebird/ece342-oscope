#ifndef _ILI9341_t3_font_ChanceryItalic_
#define _ILI9341_t3_font_ChanceryItalic_

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

extern const ILI9341_t3_font_t Chancery_8_Italic;
extern const ILI9341_t3_font_t Chancery_9_Italic;
extern const ILI9341_t3_font_t Chancery_10_Italic;
extern const ILI9341_t3_font_t Chancery_11_Italic;
extern const ILI9341_t3_font_t Chancery_12_Italic;
extern const ILI9341_t3_font_t Chancery_13_Italic;
extern const ILI9341_t3_font_t Chancery_14_Italic;
extern const ILI9341_t3_font_t Chancery_16_Italic;
extern const ILI9341_t3_font_t Chancery_18_Italic;
extern const ILI9341_t3_font_t Chancery_20_Italic;
extern const ILI9341_t3_font_t Chancery_24_Italic;
extern const ILI9341_t3_font_t Chancery_28_Italic;
extern const ILI9341_t3_font_t Chancery_32_Italic;
extern const ILI9341_t3_font_t Chancery_40_Italic;
extern const ILI9341_t3_font_t Chancery_48_Italic;
extern const ILI9341_t3_font_t Chancery_60_Italic;
extern const ILI9341_t3_font_t Chancery_72_Italic;
extern const ILI9341_t3_font_t Chancery_96_Italic;

#ifdef __cplusplus
} // extern "C"
#endif

#endif
