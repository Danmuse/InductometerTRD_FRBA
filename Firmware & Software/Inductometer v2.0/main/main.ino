#include "main.h"

Arduino_ST7789 tft(TFT_DC, TFT_RST, TFT_CS);
RotaryEncoder encoder(KY040_SW_PIN, KY040_CLK_PIN, KY040_DT_PIN);
operation_t instr_operation = OP_NONE;

void setup(void) {
  Serial.begin(115200);
  encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
  
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

  /*
  tft.setCursor(0, 0);
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.println("IPS 240x240 ST7789");
  tft.println("Library Benchmark");
  tft.println("starts in 3s ...");
  delay(3000);
  */
}

void loop(void) {
  displayMenu(tft, encoder, instr_operation);
  processOperation(tft, encoder, instr_operation);
  
  // exit(0);
}
