#include "main.h"

Arduino_ST7789 tft(TFT_DC, TFT_RST, TFT_CS);

bool flagFailureSD = false;

void setup(void) {
  Serial.begin(115200);

  delay(1000);
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println(" Status: Failure");
    flagFailureSD = true;
  } else {
    Serial.println(" Status: Success");
    // File root = SD.open("/");  // open SD card main root
    // printDirectory(root, 0);   // print all files names and sizes
    // root.close();              // close the opened root
  }
  
  tft.begin();
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  /*
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.println("IPS 240x240 ST7789");
  tft.println("Library Benchmark");
  tft.println("starts in 3s ...");
  delay(3000);
  */
}

void loop(void) {
  if (flagFailureSD) {
    Serial.println("Sorry, there's no SD!");
    if (!SD.begin(SD_CS_PIN)) {
      Serial.println("Status: Failure");
      flagFailureSD = true;
    } else {
      Serial.println("Status: Sucess");
      flagFailureSD = false;
    }
  } else {
    printFromSD(tft, "instr.bin");
    // tft.clearScreen(); delay(10);
    delay(5000);
  }
  // exit(0);
}
