#ifndef _CARD_READER_H_
#define _CARD_READER_H_

#include "utils.h"
#include <SD.h>

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.
 
uint16_t read16(File f);
uint32_t read32(File f);

#endif // _CARD_READER_H_
