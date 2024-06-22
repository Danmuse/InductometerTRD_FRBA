#include "walk.h"

#define SCR_WD   240
#define SCR_HT   240
#include <SPI.h>
#include <Adafruit_GFX.h>

#include <TFT_eSPI.h> // Hardware-specific library

#if (__STM32F1__) // bluepill
#define TFT_DC  PA1
#define TFT_RST PA0
//#include <Arduino_ST7789_STM.h>
#else
#define TFT_DC  8
#define TFT_RST 9
//#include <Arduino_ST7789_Fast.h>
//#include <Arduino_ST7789.h>
#endif

// Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);
  Serial.println(F("IPS 240x240 ST7789"));
  //tft.reset();
  //tft.init(SCR_WD, SCR_HT);
  tft.begin();

  // tft.setRotation(1);
  // tft.setSwapBytes(true);
  
  tft.fillScreen(TFT_WHITE);
}

void loop() {
  for (int i = 0; i < frames; i++) {
    delay(40);
    tft.pushImage(60, 15, animation_width, animation_height, walk[i]);
  }
}
