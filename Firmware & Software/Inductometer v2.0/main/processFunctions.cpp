#include "processFunctions.h"

static void printMenuGraphics(Arduino_ST7789 &tft);

static void printMenuGraphics(Arduino_ST7789 &tft) {
  uint8_t chunkSize = 30; // It is recommended not to modify this variable so as not to slow down or affect the representation of the image.
  // If the value of "chunkSize" is 30, 14400 bytes will be stored, that is, 72% of the SRAM memory occupied for the STM32F103x family of microcontrollers.
  uint16_t image[SCR_WD * chunkSize];

  tft.clearScreen();
  drawGradientMenu(tft);
  File fileImg = SD.open("mFrame.bin", FILE_READ);
  if (!fileImg) {
    SPI.setDataSize(DATA_SIZE_16BIT); // SPI_CR1_DFF
    return;
  }

  for (uint8_t idx = 0; idx < FRAME_GRAPH_HT / chunkSize; idx++) {
    // NOTE: Optimized alternative. An additional 4% of SRAM memory is used for the STM32 family of microcontrollers.
    for (uint8_t jdx = 0; jdx < (SCR_WD * chunkSize) / 240; jdx++) 
      readBlock(fileImg, &(image[jdx * 240]), sizeof(uint16_t) * 240);
    SPI.setDataSize(DATA_SIZE_16BIT); // SPI_CR1_DFF
    tft.drawImage(0, idx * chunkSize + (FRAME_GRAPH_HT / 2), SCR_WD, chunkSize, image);    
  }
  fileImg.close();
}

void printFromSD(Arduino_ST7789 &tft, const char *file) {
  uint8_t chunkSize = 30; // It is recommended not to modify this variable so as not to slow down or affect the representation of the image.
  // If the value of "chunkSize" is 30, 14400 bytes will be stored, that is, 72% of the SRAM memory occupied for the STM32F103x family of microcontrollers.
  uint16_t image[SCR_WD * chunkSize];
  
  File fileImg = SD.open(file, FILE_READ);
  if (!fileImg) {
    // Serial.println("Failed to open file!");
    SPI.setDataSize(DATA_SIZE_16BIT); // SPI_CR1_DFF
    return;
  } // else Serial.println("Success to open file!");

  // uint32_t timeElapsed = millis();
  tft.clearScreen();
  for (uint8_t idx = 0; idx < SCR_HT / chunkSize; idx++) {

    /* NOTE: Slowest alternative. Does not take up a lot of additional SRAM memory.
    for (uint16_t jdx = 0; jdx < SCR_WD * chunkSize;) {
      for (uint8_t kdx = 0; kdx < sizeof(color); kdx++) {
        uint32_t color = read32(fileImg);
        image[jdx++] = ((uint16_t*)&color)[0];
        image[jdx++] = ((uint16_t*)&color)[1];
      }
    }
    */
    // NOTE: Optimized alternative. An additional 4% of SRAM memory is used for the STM32 family of microcontrollers.
    for (uint8_t jdx = 0; jdx < (SCR_WD * chunkSize) / 240; jdx++) 
      readBlock(fileImg, &(image[jdx * 240]), sizeof(uint16_t) * 240);

    SPI.setDataSize(DATA_SIZE_16BIT); // SPI_CR1_DFF
    /* NOTE: Slowest alternative.
    for (uint8_t y = 0; y < chunkSize; y++) 
      for (uint8_t x = 0; x < SCR_WD; x++) 
        tft.drawPixel(x, y + (idx * chunkSize), pgm_read_word(&(image[y * SCR_WD + x])));
    */
    tft.drawImage(0, idx * chunkSize, SCR_WD, chunkSize, image);
  }
  // uint32_t result = millis() - timeElapsed;
  // Serial.print("Time elapsed: ");
  // Serial.println(result);
  fileImg.close();
}

void displayMenu(Arduino_ST7789 &tft, RotaryEncoder &encoder, operation_t &operation) {
  static bool alreadyDrawFrame = false;
  static operation_t currentOperation = OP_NONE;
  encoder.rotaryEncoderCallback();
  if (!alreadyDrawFrame) {
    printMenuGraphics(tft);
    alreadyDrawFrame = true;
  }
  if (currentOperation != encoder.getPosition()) {
    currentOperation = (operation_t)encoder.getPosition();
    tft.fillRoundRect(45, 88, 145, 63, 10, BACKGROUND_COLOR_MENU);
    tft.setTextColor(WHITE, BACKGROUND_COLOR_MENU);
    tft.setTextSize(2);
    switch (currentOperation) {
      case OP_CALCULATE_INDUCTANCE:
        tft.setCursor(60, 100); tft.print("Inductance");
        tft.setCursor(90, 122); tft.print("meter");
        break;
      case OP_CALCULATE_QUALITY_FACTOR:
        tft.setCursor(76, 100); tft.print("Quality");
        tft.setCursor(90, 122); tft.print("meter");
        break;
      case OP_GENERATE_SIGNALS:
        tft.setCursor(85, 100); tft.print("Signal");
        tft.setCursor(67, 122); tft.print("generator");
        break;
      case OP_NONE:
      default:
        break;
    }
  }
  if (encoder.buttonPressed()) {
    operation = currentOperation;
    currentOperation = OP_NONE;
    alreadyDrawFrame = false;
  }
}

void processOperation(Arduino_ST7789 &tft, RotaryEncoder &encoder, operation_t &operation) {
  switch (operation) {
    case OP_CALCULATE_INDUCTANCE:
      tft.clearScreen();
      printFromSD(tft, "induc.bin");
      delay(2000);
      tft.clearScreen();
      printFromSD(tft, "mBack.bin");
      while (!encoder.buttonPressed()) {
        
      }
      encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
      encoder.setPosition(OP_CALCULATE_INDUCTANCE);
      operation = OP_NONE;
      break;
    case OP_CALCULATE_QUALITY_FACTOR:
      tft.clearScreen();
      printFromSD(tft, "qmeter.bin");
      delay(2000);
      tft.clearScreen();
      printFromSD(tft, "mBack.bin");
      while (!encoder.buttonPressed()) {
        
      }
      encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
      encoder.setPosition(OP_CALCULATE_QUALITY_FACTOR);
      operation = OP_NONE;
      break;
    case OP_GENERATE_SIGNALS:
      tft.clearScreen();
      printFromSD(tft, "sig.bin");
      delay(2000);
      tft.clearScreen();
      printFromSD(tft, "mBack.bin");
      while (!encoder.buttonPressed()) {
        
      }
      encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
      encoder.setPosition(OP_GENERATE_SIGNALS);
      operation = OP_NONE;
      break;
    case OP_NONE:
    default:
      break;
  }
}
