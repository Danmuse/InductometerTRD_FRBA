#include "processFunctions.h"

void printFromSD(Arduino_ST7789 &tft, const char *file) {
  static bool alreadyReaded = false;
  uint8_t chunkSize = 30;
  uint16_t color, image[SCR_WD * chunkSize];
  
  if (!alreadyReaded) {
    File fileImg = SD.open(file, FILE_READ);
    if (!fileImg) {
      Serial.println("Failed to open file!");
      return;
    } else Serial.println("Success to open file!");

    uint32_t timeElapsed = millis();
    for (uint16_t idx = 0; idx < SCR_WD / chunkSize; idx++) {
      for (uint16_t y = 0; y < SCR_WD * chunkSize; y++) {
        color = read16(fileImg);
        image[y] = color;
      }
      SPI.setDataSize(DATA_SIZE_16BIT);
      for (uint8_t y = 0; y < chunkSize; y++) {
        for (uint8_t x = 0; x < SCR_WD; x++) {
          color = pgm_read_word(&(image[y * SCR_WD + x]));
          // Serial.print("Pixel color: ");
          // Serial.println(color, HEX);
          tft.drawPixel(x, y + (idx * chunkSize), color);
        }
      }
    }
    uint32_t result = millis() - timeElapsed;
    Serial.print("Time elapsed: ");
    Serial.println(result);
    fileImg.close();
    alreadyReaded = true;
  } else {
    /*
    // tft.clearScreen(); delay(10);
    tft.begin();
    for (uint16_t idx = 0; idx < SCR_WD / chunkSize; idx++) {
      for (uint16_t y = 0; y < chunkSize; y++) {
        for (uint16_t x = 0; x < SCR_WD; x++) {
          color = pgm_read_word(&(image[y * SCR_WD + x]));
          // Serial.print("Px ");
          Serial.println(color, HEX);
          // Serial.println(color, BIN);
          tft.drawPixel(x, y + (idx * chunkSize), color);
        }
      }
    }
    */
  }
}
