#ifndef _MAIN_H_
#define _MAIN_H_

#include "processFunctions.h"
#include "processActions.h"

RotaryEncoder encoder(KY040_SW_PIN, KY040_CLK_PIN, KY040_DT_PIN);
Arduino_ST7789 tft(TFT_DC, TFT_RST, TFT_CS);
RREFont font;

// Needed for RREFont library initialization
void customRect(int x, int y, int w, int h, int c) { return tft.fillRect(x, y, w, h, c); }

// WARNING: The total memory used for parity checking in addition to the regular SRAM usage is 2560 Bytes,
//          that is, 12.8% of the SRAM memory occupied for the STM32F103x family of microcontrollers.
//          The "printFromSD" function located in "processFunctions.h" occupies 76% of the SRAM memory at maximum performance.
//          The vector to represent the gradient line in the menu occupies located in "gradientMenu.h" occupies 2.4% of the SRAM memory.
//          So the required SRAM memory will occupy approximately 91.2%, without considering the global variables of the program.

#endif // _MAIN_H_
