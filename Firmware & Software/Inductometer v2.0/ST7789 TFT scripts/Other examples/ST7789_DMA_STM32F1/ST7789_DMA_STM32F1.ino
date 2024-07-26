#include "processFunctions.h"

#define ONLY_TEST_ST7789

void setup() {
  Serial.begin(115220);
#ifndef ONLY_TEST_ST7789
  uint8_t attempts = 0;
  while (attempts < CARD_CONNECTION_ATTEMPTS) {
    if (SD.begin(SD_CS_PIN)) {
      cardPlugged = true;
      // File root = SD.open("/");  // Open SD card main root
      // printDirectory(root, 0);   // Print all files names and sizes
      // root.close();              // Close the opened root
    }
    attempts++;
  }
#endif
  ST7789_Init();
  ST7789_Fill_Color(BLACK);
}

void loop() {
#ifdef ONLY_TEST_ST7789
  uint32_t timeElapsed = millis();
  ST7789_Test();
  uint32_t result = millis() - timeElapsed;
  Serial.print("Time elapsed: ");
  Serial.println(result);
#else
  printFromSD("instr.bin");
#endif // defined(ONLY_TEST_ST7789)
}
