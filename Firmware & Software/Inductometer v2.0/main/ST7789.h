#ifndef _ST7789_H_
#define _ST7789_H_

#include "utils.h"
#if (__STM32F1__)
#include <Arduino_ST7789_STM.h>
#else
#include <Arduino_ST7789_Fast.h>
#endif

#include "RREFont.h"
// #include "rre_12x16.h"
// #include "rre_bold13x20v.h"

#include "pitches.h"

#define SCR_WD 240
#define SCR_HT 240

#define SYSTEM_FAULT_TEXT_COLOR 0xFF1B // RGB: (255, 226, 220)
#define DEFAULT_BACKGROUND_COLOR 0x0020
#define DEFAULT_LETTER_SPACING_SIZE 1

#define MIN_SLOPE_FACTOR_EFFECT 14
#define MAX_SLOPE_FACTOR_EFFECT 36
#define HEIGHT_FLAME_EFFECT 6
#define FLAME_EFFECT_TRANSITION_TIME 47

extern SPISettings ST7789_SPISettings;
extern Arduino_ST7789 g_tft;
extern RREFont g_font;

extern RRE_Font rreInstance_12x16;
extern RRE_Font rreInstance_Bold13x20v;

// Needed for RREFont library initialization
void customRect(int x, int y, int w, int h, int c);

void drawImage(const uint16_t *image);
void drawGradientMenu(bool clearBackground = false);
void drawGradientFault(const char *error);
// void clearScreenBlock(byte x_initPosition, byte y_initPosition, byte width, byte height); // It would replace by the "fillRect" function.

void configFont(RREFont &font, RRE_Font *fontInstance, uint16_t color, uint8_t scaleX = 1, uint8_t scaleY = 1, uint8_t bold = 1, uint8_t letterSpacing = DEFAULT_LETTER_SPACING_SIZE, uint8_t minWidth = 1, uint16_t backColor = 0, bool backColorUsage = false);

#endif // _ST7789_H_
