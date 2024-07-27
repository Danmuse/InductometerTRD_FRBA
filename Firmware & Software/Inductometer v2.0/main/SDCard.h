#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include "utils.h"
#include <SD_STM.h>

extern SPISettings SDCard_SPISettings;

// These read 16-bit and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.
 
uint16_t read16(File f);
uint32_t read32(File f);
uint16_t readBlock(File f, uint16_t *block, size_t nbyte);

// void printDirectory(File dir, int numTabs);

#endif // _SD_CARD_H_
