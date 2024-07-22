#include "ST7789.h"

void drawImage(Arduino_ST7789 &tft, const uint16_t *image) {
  for (byte y = 0; y < SCR_HT; y++) {
    for (byte x = 0; x < SCR_WD; x++) {
      uint16_t color = pgm_read_word(&(image[y * SCR_WD + x]));
      // Serial.print("Pixel color: ");
      // Serial.println(color, HEX);
      tft.drawPixel(x, y, color);
    }
  }
}

void drawGradientMenu(Arduino_ST7789 &tft) {
  uint16_t gradientInstance[SCR_WD];
  for (byte idx = 0; idx < SCR_WD; idx++) gradientInstance[idx] = gradientHr_ST7789[idx];
  for (byte lineTFT = 0; lineTFT < 12; lineTFT++) {
    for (byte slopeFactor = 0; slopeFactor < 10; slopeFactor++) {
      for (byte idx = 0; idx < SCR_WD / 2; idx++) {
        gradientInstance[(SCR_WD / 2) - 1 + idx] = gradientInstance[(SCR_WD / 2) - 1 + idx + 1];
        gradientInstance[(SCR_WD / 2) - idx] = gradientInstance[(SCR_WD / 2) - idx - 1];
      }
    }
    tft.drawImageF(0, lineTFT, SCR_WD, 1, gradientInstance);
    tft.drawImageF(0, SCR_HT - lineTFT, SCR_WD, 1, gradientInstance);
  }
}

/*
void clearScreenBlock(Arduino_ST7789 &tft, byte x_initPosition, byte y_initPosition, byte width, byte height) {
  uint16_t cleanVector[width];
  for (byte y = 0; y < height; y++) {
    for (byte x = 0; x < width; x++) {
      cleanVector[x] = 0x0000;
      tft.drawImageF(x + x_initPosition, y + y_initPosition, width, 1, cleanVector);
    }
  }
}
*/
