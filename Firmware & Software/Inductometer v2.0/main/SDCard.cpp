#include "SDCard.h"

SPISettings SDCard_SPISettings(SPI_PERIPHERAL_SPEED, MSBFIRST, SPI_MODE0);

// These read 16-bit and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint8_t result[sizeof(uint16_t)];
  f.read(result, sizeof(uint16_t));
  return ((uint16_t)result[0] << 0) | ((uint16_t)result[1] << 8);
}

uint32_t read32(File f) {
  uint8_t result[sizeof(uint32_t)];
  f.read(result, sizeof(uint32_t));
  return ((uint32_t)result[0] << 0) | ((uint32_t)result[1] << 8) | ((uint32_t)result[2] << 16) | ((uint32_t)result[3] << 24);
}

uint16_t readBlock(File f, uint16_t *block, size_t nbyte) {
  return f.readTwoBytes(block, nbyte);
}

/*
void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;
    for (uint8_t i = 0; i < numTabs; i++) Serial.print('\t');
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // Files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
*/
