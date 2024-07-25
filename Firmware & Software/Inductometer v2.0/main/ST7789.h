#ifndef _ST7789_H_
#define _ST7789_H_

#include "utils.h"
#if (__STM32F1__)
#include <Arduino_ST7789_STM.h>
#else
#include <Arduino_ST7789_Fast.h>
#endif

#define SCR_WD 240
#define SCR_HT 240

#include "RREFont.h"
// #include "rre_12x16.h"
// #include "rre_bold13x20v.h"

#include "pitches.h"

void drawImage(Arduino_ST7789 &tft, const uint16_t *image);
void drawGradientMenu(Arduino_ST7789 &tft);
// void clearScreenBlock(Arduino_ST7789 &tft, byte x_initPosition, byte y_initPosition, byte width, byte height); // It would replace by the "fillRect" function.

#endif // _ST7789_H_
