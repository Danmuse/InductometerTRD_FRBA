#include "main.h"

operation_t instr_operation = OP_NONE;

void setup(void) {
  Serial.begin(115200);
  font.init(customRect, SCR_WD, SCR_HT);

  bool flagFailureSD = false;
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println(" Status: Failure");
    flagFailureSD = true;
  } else {
    Serial.println(" Status: Success");
    // File root = SD.open("/");  // Open SD card main root
    // printDirectory(root, 0);   // Print all files names and sizes
    // root.close();              // Close the opened root
  }

  while (flagFailureSD) {
    if (!SD.begin(SD_CS_PIN)) {
      Serial.println("Sorry, there's no SD!");
      Serial.println("Status: Failure");
      flagFailureSD = true;
    } else {
      Serial.println("Status: Sucess");
      flagFailureSD = false;
    }
  }

  tft.begin();
  tft.clearScreen();
  printFromSD(tft, "instr.bin");
  delay(2000);
  encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
}

void loop(void) {
  displayMenu(tft, encoder, font, instr_operation);
  processOperation(tft, encoder, font, instr_operation);
  
  // exit(0);
}
