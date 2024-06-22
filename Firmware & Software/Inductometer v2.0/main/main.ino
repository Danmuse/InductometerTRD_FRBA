#include "main.h"

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);

void setup(void) {
  //Serial.begin(9600);
  
  //tft.reset();
  //tft.init(SCR_WD, SCR_HT);
  tft.begin();
  tft.fillScreen(BLACK);
  //tft.setCursor(0, 0);
  //tft.setTextColor(WHITE);
  //tft.setTextSize(2);
}

void loop(void) { }
