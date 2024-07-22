#ifndef _PROCESS_FUNCTIONS_H_
#define _PROCESS_FUNCTIONS_H_

#include "ST7789.h"
#include "cardReader.h"
#include "rotaryEncoder.h"
#include "AD9833.h"

#define FRAME_GRAPH_HT 120

#define BACKGROUND_COLOR_MENU 0x0021

#define ENCODER_LOWER_LIMIT_MENU 1 // OP_CALCULATE_INDUCTANCE
#define ENCODER_UPPER_LIMIT_MENU 3 // OP_GENERATE_SIGNALS

// #define CS_IDLE     digitalWrite(TFT_CS, HIGH)
// #define CS_ACTIVE   digitalWrite(TFT_CS, LOW)

typedef enum {
  OP_NONE = 0,
  OP_CALCULATE_INDUCTANCE = 1,
  OP_CALCULATE_QUALITY_FACTOR = 2,
  OP_GENERATE_SIGNALS = 3
} operation_t;

void printFromSD(Arduino_ST7789 &tft, const char *file);
void displayMenu(Arduino_ST7789 &tft, RotaryEncoder &encoder, operation_t &operation);
void processOperation(Arduino_ST7789 &tft, RotaryEncoder &encoder, operation_t &operation);

#endif // _PROCESS_FUNCTIONS_H_
