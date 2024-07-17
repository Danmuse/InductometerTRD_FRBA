#include "ST7789.h"

void drawImage(Arduino_ST7789 &tft, const uint16_t *image) {
  // Serial.println("Let's get started");
  for (uint16_t y = 0; y < SCR_HT; y++) {
    for (uint16_t x = 0; x < SCR_WD; x++) {
      uint16_t color = pgm_read_word(&(image[y * SCR_WD + x]));
      // Serial.print("Pixel color: ");
      // Serial.println(color, HEX);
      tft.drawPixel(x, y, color);
    }
  }
}
