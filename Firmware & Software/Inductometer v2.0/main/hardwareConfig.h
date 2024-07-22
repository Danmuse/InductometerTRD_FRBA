#ifndef _HARDWARE_CONFIG_H_
#define _HARDWARE_CONFIG_H_

#if (__STM32F1__)
#define TFT_DC  PB1
#define TFT_RST PB0
#define TFT_CS  PA3

#define KY040_SW_PIN PC15
#define KY040_CLK_PIN PC13
#define KY040_DT_PIN PC14

#define SD_CS_PIN PA4  // Chip Select (CS)
#else
#define TFT_DC  8
#define TFT_RST 9
#define TFT_CS  7

#define KY040_SW_PIN 6
#define KY040_CLK_PIN 5
#define KY040_DT_PIN 4

#define SD_CS_PIN 3  // Chip Select (CS)
#endif

#endif // _HARDWARE_CONFIG_H_
