#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>

#if (__STM32F1__)
#define TFT_DC  PB1
#define TFT_RST PB0
#define TFT_CS  PA3

#define SD_CS_PIN PA4  // Chip Select (CS)
// #include <Arduino_ST7789_STM.h>
#else
#define TFT_DC  8
#define TFT_RST 9
#define TFT_CS  7

#define SD_CS_PIN 3  // Chip Select (CS)
// #include <Arduino_ST7789_Fast.h>
#endif

#include "SPI.h" // #include "CustomSPI.h"

// #define SCR_WD 240
// #define SCR_HT 240

#endif // _MAIN_H_
