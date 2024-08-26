#include "main.h"

operation_t instr_operation = OP_NONE;

void setup(void) {
  Serial.begin(115200);
  g_font.init(customRect, SCR_WD, SCR_HT);
  g_signalGenerator.begin();

  if (SD.begin(SD_CS_PIN)) {
    cardPlugged = true;
    // File root = SD.open("/");  // Open SD card main root
    // printDirectory(root, 0);   // Print all files names and sizes
    // root.close();              // Close the opened root
  }

  g_tft.begin();
  g_tft.clearScreen();
  if (cardPlugged) printFromSD("instr.bin");
  else drawGradientFault(ERR_SD_NOT_FOUND);
  g_encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
  delay(2000);
}

void loop(void) {
  displayMenu(instr_operation);
  processOperation(instr_operation);
  
  // exit(0);
}
