#include "processFunctions.h"

bool cardPlugged = false;

void printFromSD(const char *file) {
  uint8_t chunkSize = HOR_LEN; // It is recommended not to modify this variable so as not to slow down or affect the representation of the image.
  // If the value of "chunkSize" is 30, 14400 bytes will be stored, that is, 72% of the SRAM memory occupied for the STM32F103x family of microcontrollers.
  // uint16_t image[ST7789_WIDTH * chunkSize];

  if (!cardPlugged) {
    uint8_t attempts = 0;
    while (attempts < CARD_CONNECTION_ATTEMPTS) {
      if (SD.begin(SD_CS_PIN)) {
        cardPlugged = true;
        break;
      }
      attempts++;
    }
    if (!cardPlugged) {
      SPI.setDataSize(DATA_SIZE_16BIT);
      ST7789_Fill_Color(RED);
      // tft.fillRect(0, 0, SCR_WD, SCR_HT, BACKGROUND_COLOR_MENU);
      return;
    }
  }
  
  File fileImg = SD.open(file, FILE_READ);
  if (!fileImg) {
    cardPlugged = false;
    SPI.setDataSize(DATA_SIZE_16BIT);
    ST7789_Fill_Color(RED);
    // tft.fillRect(0, 0, SCR_WD, SCR_HT, BACKGROUND_COLOR_MENU);
    return;
  }

  uint32_t timeElapsed = millis();
  ST7789_Fill_Color(BLACK); // tft.clearScreen();
  for (uint8_t idx = 0; idx < ST7789_HEIGHT / chunkSize; idx++) {
    /* NOTE: Slowest alternative. Does not take up a lot of additional SRAM memory.
    for (uint16_t jdx = 0; jdx < SCR_WD * chunkSize;) {
      for (uint8_t kdx = 0; kdx < sizeof(color); kdx++) {
        uint32_t color = read32(fileImg);
        image[jdx++] = ((uint16_t*)&color)[0];
        image[jdx++] = ((uint16_t*)&color)[1];
      }
    }
    */
    // NOTE: Optimized alternative. An additional 4% of SRAM memory is used for the STM32 family of microcontrollers.
    for (uint8_t jdx = 0; jdx < (ST7789_WIDTH * chunkSize) / 240; jdx++) 
      readBlock(fileImg, &(disp_buf[jdx * 240]), sizeof(uint16_t) * 240);
    SPI.setDataSize(DATA_SIZE_16BIT);
    ST7789_DrawImage(0, idx * chunkSize, ST7789_WIDTH, chunkSize, disp_buf);
    // tft.drawImage(0, idx * chunkSize, SCR_WD, chunkSize, image);
  }
  uint32_t result = millis() - timeElapsed;
  Serial.print("Time elapsed: ");
  Serial.println(result);
  fileImg.close();
}
