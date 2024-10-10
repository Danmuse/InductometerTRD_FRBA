#include "processFunctions.h"

bool cardPlugged = false;

static int32_t getMultiplierFromExponentialNotation(char *exponentialNotation);

static void signal_drawCharts(byte x, byte y, byte w, byte h, char *label, bool hover = false, bool idle = true);
static void signal_displayValue(void* value, byte x, byte y, uint8_t amountOfDecimals, textTypes_t type, bool idle = true);
static void changeSettingSignal(char setting[MAX_SIGNAL_SETTING_SIZE], sig_operation_t operation);

static void printMenuGraphics(void);

static int32_t getMultiplierFromExponentialNotation(char *exponentialNotation) {
  return (int32_t)pow(10, atoi(&(exponentialNotation[SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION])));
}

static void signal_drawCharts(byte x, byte y, byte w, byte h, char *label, bool hover, bool idle) {
  g_tft.drawRoundRect(x+0, y+10, w-2*0, h-10-2*0, 10, hover ? FRAME_HOVER_CHARTS_COLOR : FRAME_CHARTS_COLOR(idle));
  g_tft.drawRoundRect(x+1, y+11, w-2*1, h-10-2*1, 10, hover ? FRAME_HOVER_CHARTS_COLOR : FRAME_CHARTS_COLOR(idle));
  g_tft.drawRoundRect(x+2, y+12, w-2*2, h-10-2*2, 10, hover ? FRAME_HOVER_CHARTS_COLOR : FRAME_CHARTS_COLOR(idle));
  g_tft.fillRoundRect(x+3, y+13, w-2*3, h-10-2*3, 10, BACKGROUND_CHARTS_COLOR);
  configFont(g_font, &rreInstance_12x16, hover ? TITLE_HOVER_CHARTS_COLOR : TITLE_CHARTS_COLOR(idle), 1, 1, 1, LETTER_SPACING_SIZE);
  g_font.printStr(x+(w-g_font.strWidth(label))/2, y-(g_font.getHeight())/2-LETTER_SPACING_SIZE, label);
}

static void signal_displayValue(void* value, byte x, byte y, uint8_t amountOfDecimals, textTypes_t type, bool idle) {
  char txt[MAX_SIGNAL_SETTING_SIZE];
  configFont(g_font, &rreInstance_Bold13x20v, TEXT_CHARTS_COLOR(idle), 1, 2, 1, LETTER_SPACING_SIZE, 16);
  if (type == TYPE_NONE) return;
  else if (type == TYPE_UINT) citoa(*(uint16_t*)value, txt, 10);
  else if (type == TYPE_INT) citoa(*(int32_t*)value, txt, 10);
  else if (type == TYPE_FLOAT) cftoa(*(float*)value, txt, amountOfDecimals);
  else if (type == TYPE_STRING) {
    configFont(g_font, &rreInstance_12x16, TEXT_CHARTS_COLOR(idle), 1, 3, 1, LETTER_SPACING_SIZE, 13);
    strncpy(txt, (char*)value, strlen((char*)value));
    txt[strlen((char*)value)] = '\0';
    y -= 2;
  }
  else if (type == TYPE_HIBRID) {
    uint8_t amountOfDigits = 0;
    strncpy(txt, (char*)value, strlen((char*)value));
    txt[strlen((char*)value)] = '\0';
    if (txt[0] == 'x') {
      bool exponentialNotation = false;
      for (byte idx = 1; idx < strlen(txt) - 1 && txt[idx] != '\0'; idx++) {
        if (txt[idx] == '^' && !exponentialNotation && txt[1] == '1' && txt[2] == '0' && idx == SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION - 1) exponentialNotation = true;
        else if (!isNumberExt(txt[idx])) return;
        amountOfDigits++;
      }
      if (exponentialNotation) {
        char unit[2] = {'x', '\0'};
        char unitBase[3] = {'1', '0', '\0'};
        char *exponentValue = &(txt[SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION]);
        RREFont fontUnits = g_font;
        RREFont fontExponent = g_font;
        configFont(fontUnits, &rreInstance_12x16, TEXT_CHARTS_COLOR(idle), 1, 3, 1, LETTER_SPACING_SIZE, 14);
        configFont(fontExponent, &rreInstance_Bold13x20v, TEXT_CHARTS_COLOR(idle), 1, 1, 1, LETTER_SPACING_SIZE, 12);
        fontUnits.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-(2+g_font.strWidth(exponentValue)+g_font.strWidth(unitBase)+fontUnits.strWidth(unit)))/2 + 4, y-2, unit);
        g_font.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-((2+g_font.strWidth(exponentValue)+g_font.strWidth(unitBase))-fontUnits.strWidth(unit)))/2 + 4, y, unitBase);
        fontExponent.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-(g_font.strWidth(exponentValue)-(2+g_font.strWidth(unitBase)+fontUnits.strWidth(unit))))/2 + 4, y, exponentValue);
      } else {
        if (amountOfDigits > MAX_MULTIPLIER_DIGITS) return;
        char unit[2] = {'x', '\0'};
        char *multiplierValue = &(txt[1]);
        RREFont fontUnits = g_font;
        configFont(fontUnits, &rreInstance_12x16, TEXT_CHARTS_COLOR(idle), 1, 3, 1, LETTER_SPACING_SIZE, 14);
        fontUnits.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-(g_font.strWidth(multiplierValue)+fontUnits.strWidth(unit)))/2, y-2, unit);
        g_font.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-(g_font.strWidth(multiplierValue)-fontUnits.strWidth(unit)))/2, y, multiplierValue);
      }
    } else if (txt[strlen(txt) - 2] == 'H' && txt[strlen(txt) - 1] == 'z') {
      for (byte idx = 0; idx < strlen(txt) && txt[idx] != 'H'; idx++) {
        if (!isNumberExt(txt[idx])) return;
        amountOfDigits++;
      }
      if (amountOfDigits > MAX_FREQUENCY_DIGITS) return;
      char *frequencyValue = strtok(txt, "H");
      char unit[4] = {' ', 'H', 'z', '\0'};
      strncpy(&(txt[strlen(txt)]), "Hz", strlen("Hz"));
      txt[strlen(txt)] = '\0';
      RREFont fontUnits = g_font;
      configFont(fontUnits, &rreInstance_12x16, TEXT_CHARTS_COLOR(idle), 1, 3, 1, LETTER_SPACING_SIZE, 14);
      g_font.printStr(x+(CONTENT_WIDTH_FILL_OUT_CHARTS_SIZE-(fontUnits.strWidth(unit)+g_font.strWidth(frequencyValue)))/2 + 3, y, frequencyValue);
      fontUnits.printStr(x+(CONTENT_WIDTH_FILL_OUT_CHARTS_SIZE-(fontUnits.strWidth(unit)-g_font.strWidth(frequencyValue)))/2 - 3, y-1, unit);
    }
    return;
  }
  g_font.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-g_font.strWidth(txt))/2, y, txt);
}

static void changeSettingSignal(char setting[MAX_SIGNAL_SETTING_SIZE], sig_operation_t operation) {
#ifndef MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION
  const char* multipliers[] = {"1", "10", "100", "1000", "10000"};
#else
  const char* exponents[] = {"10^0", "10^1", "10^2", "10^3", "10^4", "10^5"};
#endif // !defined(MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION)
  const char* signalTypes[] = {"SINE", "SQUARE", "TRIANG"};
  const uint8_t numSignalTypes = sizeof(signalTypes) / sizeof(signalTypes[0]);
  static uint32_t multiplierStored = 1000;
  uint16_t blinkInterval = ACTIVE_BLINK_DIGIT_TIME;
  uint32_t blinkTimeElapsed = 0;
  bool blinkStatus = true;
  g_encoder.mismatchEngaged();
  g_tft.fillRect(0, 0, SCR_WD, 44, BACKGROUND_CHARTS_COLOR);
  g_tft.drawFastHLine(0, 44, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
  g_tft.drawFastHLine(0, 45, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
  g_tft.drawFastHLine(0, 46, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
  configFont(g_font, &rreInstance_12x16, PANEL_ELEMENTS_COLOR(false), 1, 2, 2, 2, 10);
  g_font.printStr(ALIGN_CENTER, 8, "PRESS TO CONFIRM");

  if (operation == SIG_OP_SET_MULT) {
    g_encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_MULTIPLIER_OPTIONS, ENCODER_UPPER_LIMIT_SIGNAL_MULTIPLIER_OPTIONS);
#ifndef MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION
    for (byte idx = 0; idx < ENCODER_UPPER_LIMIT_SIGNAL_MULTIPLIER_OPTIONS+1; idx++) {
      if (!strcmp(&(setting[1]), multipliers[idx])) {
        g_encoder.setPosition(idx);
        break;
      }
    }
    uint8_t currentPosition = g_encoder.getPosition();
    blinkTimeElapsed = millis();
    while (!g_encoder.buttonPressed()) {
      g_encoder.rotaryEncoderCallback();
      if (currentPosition != g_encoder.getPosition()) {
        currentPosition = g_encoder.getPosition();
        g_encoder.mismatchEngaged();
        multiplierStored = atoi(multipliers[currentPosition]);
        snprintf(setting, MAX_SIGNAL_SETTING_SIZE, "x%s", multipliers[currentPosition]);
        signal_drawCharts(MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", true);
        signal_displayValue(setting, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
      }
      blinkInterval = blinkStatus ? ACTIVE_BLINK_DIGIT_TIME : INACTIVE_BLINK_DIGIT_TIME;
      if (millis() - blinkTimeElapsed > blinkInterval) {
        blinkTimeElapsed = millis();
        blinkStatus = !blinkStatus;
        g_encoder.mismatchEngaged();
        signal_drawCharts(MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", true);
        if (blinkStatus) signal_displayValue(setting, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
        else {
          char settingInstance[MAX_SIGNAL_SETTING_SIZE] = "x";
          memset(settingInstance + 1, ' ', strlen(multipliers[currentPosition]));
          settingInstance[strlen(multipliers[currentPosition]) + 1] = '\0';
          signal_displayValue(settingInstance, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
        }
      }
    }
#else
    for (byte idx = 0; idx < ENCODER_UPPER_LIMIT_SIGNAL_MULTIPLIER_OPTIONS+1; idx++) {
      if (!strcmp(&(setting[1]), exponents[idx])) {
        g_encoder.setPosition(idx);
        break;
      }
    }
    uint8_t currentPosition = g_encoder.getPosition();
    while (!g_encoder.buttonPressed()) {
      g_encoder.rotaryEncoderCallback();
      if (currentPosition != g_encoder.getPosition()) {
        currentPosition = g_encoder.getPosition();
        g_encoder.mismatchEngaged();
        snprintf(setting, MAX_SIGNAL_SETTING_SIZE, "x%s", exponents[currentPosition]);
        multiplierStored = getMultiplierFromExponentialNotation(setting);
        signal_drawCharts(MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", true);
        signal_displayValue(setting, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
      }
      blinkInterval = blinkStatus ? ACTIVE_BLINK_DIGIT_TIME : INACTIVE_BLINK_DIGIT_TIME;
      if (millis() - blinkTimeElapsed > blinkInterval) {
        blinkTimeElapsed = millis();
        blinkStatus = !blinkStatus;
        g_encoder.mismatchEngaged();
        signal_drawCharts(MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", true);
        if (blinkStatus) signal_displayValue(setting, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
        else {
          char settingInstance[MAX_SIGNAL_SETTING_SIZE] = "x10";
          memset(settingInstance+(SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION-1), ' ', strlen(exponents[currentPosition])-SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION);
          settingInstance[strlen(exponents[currentPosition])-SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION+(SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION-1)] = '\0';
          signal_displayValue(settingInstance, HORIZONTAL_PADDING_MIDDLE_CHARTS-6, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
        }
      }
    }
#endif // !defined(MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION)
  } else if (operation == SIG_OP_SET_TYPE) {
    g_encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_SIG_TYPE_OPTIONS, ENCODER_UPPER_LIMIT_SIGNAL_SIG_TYPE_OPTIONS);
    for (byte idx = 0; idx < numSignalTypes; idx++) {
      if (!strcmp(&(setting[0]), signalTypes[idx])) {
        g_encoder.setPosition(idx);
        break;
      }
    }
    uint8_t currentPosition = g_encoder.getPosition();
    blinkTimeElapsed = millis();
    while (!g_encoder.buttonPressed()) {
      g_encoder.rotaryEncoderCallback();
      if (currentPosition != g_encoder.getPosition()) {
        currentPosition = g_encoder.getPosition();
        g_encoder.mismatchEngaged();
        snprintf(setting, MAX_SIGNAL_SETTING_SIZE, "%s", signalTypes[currentPosition]);
        signal_drawCharts(MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE", true);
        signal_displayValue(setting, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING, true);
      }
      blinkInterval = blinkStatus ? ACTIVE_BLINK_DIGIT_TIME : INACTIVE_BLINK_DIGIT_TIME;
      if (millis() - blinkTimeElapsed > blinkInterval) {
        blinkTimeElapsed = millis();
        blinkStatus = !blinkStatus;
        g_encoder.mismatchEngaged();
        signal_drawCharts(MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE", true);
        if (blinkStatus) signal_displayValue(setting, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING, true);
        else {
          char settingInstance[MAX_SIGNAL_SETTING_SIZE];
          memset(settingInstance, ' ', strlen(signalTypes[currentPosition]));
          settingInstance[strlen(signalTypes[currentPosition])] = '\0';
          signal_displayValue(settingInstance, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING, true);
        }
      }
    }
  } else if (operation == SIG_OP_SET_FREQ) {
    g_encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_FREQUENCY, ENCODER_UPPER_LIMIT_SIGNAL_FREQUENCY);
    char *frequencyValue = strtok(setting, "H");
    strncpy(&(setting[strlen(setting)]), "Hz", strlen("Hz"));
    setting[strlen(setting)] = '\0';
    g_encoder.setPosition(atoi(frequencyValue));
    g_encoder.setMultiplier(multiplierStored);
    uint32_t currentPosition = g_encoder.getPosition();
    blinkTimeElapsed = millis();
    while (!g_encoder.buttonPressed()) {
      g_encoder.rotaryEncoderCallback();
      if (currentPosition != g_encoder.getPosition()) {
        currentPosition = g_encoder.getPosition();
        g_encoder.mismatchEngaged();
        snprintf(setting, MAX_SIGNAL_SETTING_SIZE, "%uHz", currentPosition);
        signal_drawCharts(MARGIN_FILL_OUT_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY", true);
        signal_displayValue(setting, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID, true);
      }
      blinkInterval = blinkStatus ? ACTIVE_BLINK_DIGIT_TIME : INACTIVE_BLINK_DIGIT_TIME;
      if (millis() - blinkTimeElapsed > blinkInterval) {
        blinkTimeElapsed = millis();
        blinkStatus = !blinkStatus;
        g_encoder.mismatchEngaged();
        signal_drawCharts(MARGIN_FILL_OUT_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY", true);
        if (blinkStatus) signal_displayValue(setting, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID, true);
        else {
          char settingInstance[MAX_SIGNAL_SETTING_SIZE];
          memset(settingInstance, ' ', strlen(setting) - strlen("Hz"));
          strncpy(&(settingInstance[strlen(setting) - strlen("Hz")]), "Hz", strlen("Hz"));
          settingInstance[strlen(setting)] = '\0';
          signal_displayValue(settingInstance, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID, true);
        }
      }
    }
    g_encoder.usageMultiplier(false);
  }
}

static void printMenuGraphics(void) {
  uint8_t chunkSize = 30; // It is recommended not to modify this variable so as not to slow down or affect the representation of the image.
  // If the value of "chunkSize" is 30, 14400 bytes will be stored, that is, 72% of the SRAM memory occupied for the STM32F103x family of microcontrollers.
  uint16_t image[SCR_WD * chunkSize];
  
  drawGradientMenu(true);
  if (!cardPlugged) return;
  else spiTransaction(SDCard_SPISettings, []() { });
  File fileImg = SD.open("mSFrame.bin", FILE_READ);
  if (!fileImg) {
    cardPlugged = false;
    spiTransaction(ST7789_SPISettings, []() { });
    return;
  }

  for (uint8_t idx = 0; idx < FRAME_GRAPH_HT / chunkSize; idx++) {
    // NOTE: Optimized alternative. An additional 4% of SRAM memory is used for the STM32 family of microcontrollers.
    for (uint8_t jdx = 0; jdx < (SCR_WD * chunkSize) / 240; jdx++) 
      readBlock(fileImg, &(image[jdx * 240]), sizeof(uint16_t) * 240);
    spiTransaction(ST7789_SPISettings, []() { });
    g_tft.drawImage(0, idx * chunkSize + (FRAME_GRAPH_HT / 2), SCR_WD, chunkSize, image);    
  }
  fileImg.close();
}

void printFromSD(const char *file) {
  uint8_t chunkSize = 30; // It is recommended not to modify this variable so as not to slow down or affect the representation of the image.
  // If the value of "chunkSize" is 30, 14400 bytes will be stored, that is, 72% of the SRAM memory occupied for the STM32F103x family of microcontrollers.
  uint16_t image[SCR_WD * chunkSize];

  if (!cardPlugged) {
    uint8_t attempts = 0;
    drawGradientFault(ERR_SD_NOT_FOUND);
    spiTransaction(SDCard_SPISettings, []() { });
    while (attempts < CARD_CONNECTION_ATTEMPTS) {
      if (SD.begin(SD_CS_PIN)) {
        cardPlugged = true;
        break;
      }
      attempts++;
    }
    spiTransaction(ST7789_SPISettings, []() { });
  }

  g_tft.fillRect(0, 0, SCR_WD, SCR_HT, DEFAULT_BACKGROUND_COLOR);
  if (!cardPlugged) return;
  else spiTransaction(SDCard_SPISettings, []() { });
  File fileImg = SD.open(file, FILE_READ);
  if (!fileImg) {
    cardPlugged = false;
    spiTransaction(ST7789_SPISettings, []() { });
    return;
  }

  // uint32_t timeElapsed = millis();
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
    spiTransaction(ST7789_SPISettings, []() { });
    g_tft.drawImage(0, idx * chunkSize, SCR_WD, chunkSize, image);
  }
  // uint32_t result = millis() - timeElapsed;
  // Serial.print("Time elapsed: ");
  // Serial.println(result);
  fileImg.close();
}

void displayMenu(operation_t &operation) {
  static bool alreadyDrawFrame = false;
  static operation_t currentOperation = OP_NONE;
  g_encoder.rotaryEncoderCallback();
  if (!alreadyDrawFrame) {
    g_encoder.mismatchEngaged();
    printMenuGraphics();
    alreadyDrawFrame = true;
  }
  drawGradientMenu();
  if (currentOperation != g_encoder.getPosition()) {
    currentOperation = (operation_t)g_encoder.getPosition();
    g_tft.fillRoundRect(44, 88, 147, 63, 10, BACKGROUND_COLOR_MENU);
    configFont(g_font, &rreInstance_12x16, TEXT_COLOR_MENU, 1, 1, 1, LETTER_SPACING_SIZE, 12, BACKGROUND_COLOR_MENU, true);
    switch (currentOperation) {
      case OP_CALCULATE_INDUCTANCE:
        g_font.printStr(ALIGN_CENTER, 100, "INDUCTANCE");
        g_font.printStr(ALIGN_CENTER, 123, "METER");
        break;
      case OP_CALCULATE_QUALITY_FACTOR:
        g_font.printStr(ALIGN_CENTER, 100, "QUALITY");
        g_font.printStr(ALIGN_CENTER, 123, "METER");
        break;
      case OP_GENERATE_SIGNALS:
        g_font.printStr(ALIGN_CENTER, 100, "SIGNAL");
        g_font.printStr(ALIGN_CENTER, 123, "GENERATOR");
        break;
      case OP_NONE:
      default:
        break;
    }
  }
  if (g_encoder.buttonPressed()) {
    operation = currentOperation;
    currentOperation = OP_NONE;
    alreadyDrawFrame = false;
  }
}

void processOperation(operation_t &operation) {
  switch (operation) {
    case OP_CALCULATE_INDUCTANCE:
      g_encoder.mismatchEngaged();
      printFromSD("induc.bin");
      if (cardPlugged) {
        delay(2000);
        printFromSD("mBack.bin");
      }
      
      if (!cardPlugged) drawGradientFault(ERR_SECTION_NOT_AVAILABLE);
      while (!g_encoder.buttonPressed()) {
        // TODO: It's not implemented yet
      }
      
      g_encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
      g_encoder.setPosition(OP_CALCULATE_INDUCTANCE);
      operation = OP_NONE;
      break;
    case OP_CALCULATE_QUALITY_FACTOR:
      g_encoder.mismatchEngaged();
      printFromSD("qmeter.bin");
      if (cardPlugged) {
        delay(2000);
        printFromSD("mBack.bin");
      }
      
      if (!cardPlugged) drawGradientFault(ERR_SECTION_NOT_AVAILABLE);
      while (!g_encoder.buttonPressed()) {
        // TODO: It's not implemented yet
      }
      
      g_encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
      g_encoder.setPosition(OP_CALCULATE_QUALITY_FACTOR);
      operation = OP_NONE;
      break;
    case OP_GENERATE_SIGNALS:
      g_encoder.mismatchEngaged();
      printFromSD("sig.bin");
      if (cardPlugged) {
        delay(2000);
        printFromSD("mBack.bin");
      }
      settingSignal();
      g_encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
      g_encoder.setPosition(OP_GENERATE_SIGNALS);
      operation = OP_NONE;
      break;
    case OP_NONE:
    default:
      break;
  }
}

void settingSignal(void) {
  g_encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_PANEL, ENCODER_UPPER_LIMIT_SIGNAL_PANEL);
  g_encoder.setPosition(SIG_OP_COMPLETE);
  while (true) {
    static bool alreadyDrawPanel = false;
    static sig_operation_t signalOperation = SIG_OP_CANCEL;
    static char strMultiplier[MAX_SIGNAL_SETTING_SIZE];
    static char strSignalType[MAX_SIGNAL_SETTING_SIZE];
    static char strFrequency[MAX_SIGNAL_SETTING_SIZE];
    g_encoder.rotaryEncoderCallback();
    if (!alreadyDrawPanel) {
      g_encoder.mismatchEngaged();
      if (*strMultiplier == '\0' || *strSignalType == '\0' || *strFrequency == '\0') {
#ifndef MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION
        strMultiplier[0] = 'x';
        citoa(DEFAULT_SIGNAL_MULTIPLIER, &(strMultiplier[1]), 10);
#else
        strncpy(strMultiplier, "x10^", strlen("x10^"));
        citoa(DEFAULT_SIGNAL_MULTIPLIER, &(strMultiplier[SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION]), 10);
#endif // !defined(MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION)
        if (DEFAULT_SIGNAL_TYPE == SIG_SINE) {
          strncpy(strSignalType, "SINE", strlen("SINE"));
          strSignalType[strlen("SINE")] = '\0';
        } else if (DEFAULT_SIGNAL_TYPE == SIG_FST_SQUARE) {
          strncpy(strSignalType, "SQUARE", strlen("SQUARE"));
          strSignalType[strlen("SQUARE")] = '\0';
        } else if (DEFAULT_SIGNAL_TYPE == SIG_TRIANGLE) {
          strncpy(strSignalType, "TRIANG", strlen("TRIANG"));
          strSignalType[strlen("TRIANG")] = '\0';
        }
        citoa(DEFAULT_SIGNAL_FREQUENCY, &(strFrequency[0]), 10);
        strncpy(&(strFrequency[strlen(strFrequency)]), "Hz", strlen("Hz"));
        strFrequency[strlen(strFrequency)] = '\0';
        signal_displayValue(strMultiplier, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID);
        signal_displayValue(strSignalType, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING);
        signal_displayValue(strFrequency, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID);
      }
      alreadyDrawPanel = true;
    }
    if (signalOperation != g_encoder.getPosition()) {
      g_encoder.mismatchEngaged();
      signalOperation = (sig_operation_t)g_encoder.getPosition();
      signal_drawCharts(MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER");
      signal_drawCharts(MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE");
      signal_drawCharts(MARGIN_FILL_OUT_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY");
      signal_displayValue(strMultiplier, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID);
      signal_displayValue(strSignalType, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING);
      signal_displayValue(strFrequency, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID);
      g_tft.fillRect(0, 0, SCR_WD, 44, BACKGROUND_CHARTS_COLOR);
      g_tft.drawFastHLine(0, 44, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
      g_tft.drawFastHLine(0, 45, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
      g_tft.drawFastHLine(0, 46, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
      switch (signalOperation) {
        case SIG_OP_CANCEL:
          configFont(g_font, &rreInstance_12x16, PANEL_ELEMENTS_INACTIVE_COLOR, 1, 2, 2, 2, 10);
          g_font.printStr(ALIGN_CENTER,8,"PRESS TO EXIT");
          break;
        case SIG_OP_COMPLETE:
          configFont(g_font, &rreInstance_12x16, PANEL_ELEMENTS_ACTIVE_COLOR(true), 1, 2, 2, 2, 10);
          g_font.printStr(ALIGN_CENTER,8,"PRESS TO START");
          break;
        case SIG_OP_SET_MULT:
          configFont(g_font, &rreInstance_12x16, PANEL_ELEMENTS_COLOR(true), 1, 2, 2, 2, 10);
          g_font.printStr(ALIGN_CENTER,8,"PRESS TO SET");
          signal_drawCharts(MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", true);
          signal_displayValue(strMultiplier, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID);
          break;
        case SIG_OP_SET_TYPE:
          configFont(g_font, &rreInstance_12x16, PANEL_ELEMENTS_COLOR(true), 1, 2, 2, 2, 10);
          g_font.printStr(ALIGN_CENTER,8,"PRESS TO SET");
          signal_drawCharts(MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE", true);
          signal_displayValue(strSignalType, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING);
          break;
        case SIG_OP_SET_FREQ:
          configFont(g_font, &rreInstance_12x16, PANEL_ELEMENTS_COLOR(true), 1, 2, 2, 2, 10);
          g_font.printStr(ALIGN_CENTER,8,"PRESS TO SET");
          signal_drawCharts(MARGIN_FILL_OUT_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY", true);
          signal_displayValue(strFrequency, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID);
          break;
        default:
          break;
      }
    }
    if (g_encoder.buttonPressed()) {
      if (signalOperation == SIG_OP_CANCEL) {
        alreadyDrawPanel = false;
        break;
      } else if (signalOperation == SIG_OP_COMPLETE) {
        g_encoder.mismatchEngaged();
        signal_drawCharts(MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", false, false);
        signal_drawCharts(MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE", false, false);
        signal_drawCharts(MARGIN_FILL_OUT_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY", false, false);
        signal_displayValue(strMultiplier, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, false);
        signal_displayValue(strSignalType, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING, false);
        signal_displayValue(strFrequency, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID, false);
        g_tft.fillRect(0, 0, SCR_WD, 44, BACKGROUND_CHARTS_COLOR);
        g_tft.drawFastHLine(0, 44, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
        g_tft.drawFastHLine(0, 45, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
        g_tft.drawFastHLine(0, 46, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
        configFont(g_font, &rreInstance_12x16, PANEL_ELEMENTS_ACTIVE_COLOR(false), 1, 2, 2, 2, 10);
        g_font.printStr(ALIGN_CENTER,8,"PRESS TO STOP");
        digitalWrite(BOOSTER_ENABLEMENT, HIGH);
        generateSignal(strFrequency, strSignalType);
        setVoltage(analogMCPA0_agnd, 360, FALSE); // TODO: Implement a dinamic adjustment
        delay(1); // Try to delete this
        while (!g_encoder.buttonPressed()) {
          // TODO: Read the output signal waveform with the ADC
        }
        g_signalGenerator.setWave(SIG_NONE);
        digitalWrite(BOOSTER_ENABLEMENT, LOW);
        spiTransaction(ST7789_SPISettings, []() { });
        g_encoder.setPosition(SIG_OP_COMPLETE);
        signalOperation = SIG_OP_CANCEL;
      } else if (signalOperation == SIG_OP_SET_MULT) {
        changeSettingSignal(strMultiplier, signalOperation);
        g_encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_PANEL, ENCODER_UPPER_LIMIT_SIGNAL_PANEL);
        g_encoder.setPosition(SIG_OP_SET_MULT);
        signalOperation = SIG_OP_CANCEL;
      } else if (signalOperation == SIG_OP_SET_TYPE) {
        changeSettingSignal(strSignalType, signalOperation);
        g_encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_PANEL, ENCODER_UPPER_LIMIT_SIGNAL_PANEL);
        g_encoder.setPosition(SIG_OP_SET_TYPE);
        signalOperation = SIG_OP_CANCEL;
      } else if (signalOperation == SIG_OP_SET_FREQ) {
        changeSettingSignal(strFrequency, signalOperation);
        g_encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_PANEL, ENCODER_UPPER_LIMIT_SIGNAL_PANEL);
        g_encoder.setPosition(SIG_OP_SET_FREQ);
        signalOperation = SIG_OP_CANCEL;
      }
    }
  }
}
