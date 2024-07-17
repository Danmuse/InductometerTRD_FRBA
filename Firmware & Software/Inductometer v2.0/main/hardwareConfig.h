#ifndef _HARDWARE_CONFIG_H_
#define _HARDWARE_CONFIG_H_

#if (__STM32F1__)
#define TFT_DC  PB1
#define TFT_RST PB0
#define TFT_CS  PA3
#else
#define TFT_DC  8
#define TFT_RST 9
#define TFT_CS  7
#endif

#define SD_CS_PIN PA4  // Chip Select (CS) en PA4

#endif // _HARDWARE_CONFIG_H_
