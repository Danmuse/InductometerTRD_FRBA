#include "processFunctions.h"

void printFromSD(Arduino_ST7789 &tft, const char *file) {
  static bool alreadyReaded = false;
  uint8_t chunkSize = 30; // It is recommended not to modify this variable so as not to slow down or affect the representation of the image.
  uint16_t image[SCR_WD * chunkSize];
  uint32_t color;
  
  if (!alreadyReaded) {
    File fileImg = SD.open(file, FILE_READ);
    if (!fileImg) {
      Serial.println("Failed to open file!");
      return;
    } else Serial.println("Success to open file!");

    uint32_t timeElapsed = millis();
    tft.clearScreen();
    for (uint8_t idx = 0; idx < SCR_WD / chunkSize; idx++) {

      /* NOTE: Slowest alternative
      for (uint16_t jdx = 0; jdx < SCR_WD * chunkSize;) {
        for (uint8_t kdx = 0; kdx < sizeof(color); kdx++) {
          color = read32(fileImg);
          image[jdx++] = ((uint16_t*)&color)[0];
          image[jdx++] = ((uint16_t*)&color)[1];
        }
      }
      */
      // NOTE: Optimized alternative
      for (uint8_t jdx = 0; jdx < (SCR_WD * chunkSize) / 240; jdx++) 
        readBlock(fileImg, &(image[jdx * 240]), sizeof(uint16_t) * 240);

      SPI.setDataSize(DATA_SIZE_16BIT);
      for (uint8_t y = 0; y < chunkSize; y++) 
        for (uint8_t x = 0; x < SCR_WD; x++) 
          tft.drawPixel(x, y + (idx * chunkSize), pgm_read_word(&(image[y * SCR_WD + x])));
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
