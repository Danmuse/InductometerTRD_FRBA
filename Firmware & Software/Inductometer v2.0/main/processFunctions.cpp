#include "processFunctions.h"

static RRE_Font rreInstance_12x16 = { RRE_16B, 11, 16, 0x20, 0x7f, (const uint8_t*)font12x16_RectsInstance, (const uint16_t*)font12x16_CharOffsInstance };
static RRE_Font rreInstance_Bold13x20v = { RRE_V16B, 13, 20, 0x20, 0x3a, (const uint8_t*)fontBold13x20v_RectsInstance, (const uint16_t*)fontBold13x20v_CharOffsInstance };

static bool isNumberExt(uint8_t character);
static int32_t getMultiplierFromExponentialNotation(char *exponentialNotation);

static void reverse(char *str, byte length);
static uint8_t intToStr(int32_t num, char *str, int decimals);
static char *cftoa(float num, char *str, uint8_t decimals);
static char *citoa(int32_t num, char *str, uint8_t base);

static void configFont(RREFont &font, RRE_Font *fontInstance, uint16_t color, uint8_t scaleX = 1, uint8_t scaleY = 1, uint8_t bold = 1, uint8_t letterSpacing = LETTER_SPACING_SIZE, uint8_t minWidth = 1, uint16_t backColor = 0, bool backColorUsage = false);

static void signal_drawCharts(Arduino_ST7789 &tft, RREFont &font, byte x, byte y, byte w, byte h, char *label, bool hover = false, bool idle = true);
static void signal_displayValue(Arduino_ST7789 &tft, RREFont &font, void* value, byte x, byte y, uint8_t amountOfDecimals, textTypes_t type, bool idle = true);
static void changeSettingSignal(Arduino_ST7789 &tft, RotaryEncoder &encoder, RREFont &font, char setting[MAX_SIGNAL_SETTING_SIZE], sig_operation_t operation);

static void printMenuGraphics(Arduino_ST7789 &tft);

static bool isNumberExt(uint8_t character) {
  return isdigit(character) || character=='-' || character=='+' || character=='.' || character==' ';
}

static int32_t getMultiplierFromExponentialNotation(char *exponentialNotation) {
  return (int32_t)pow(10, atoi(&(exponentialNotation[SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION])));
}

static void reverse(char *str, byte length) {
  uint8_t start = 0, end = length - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    end--; start++;
  }
}

static uint8_t intToStr(int32_t num, char *str, int decimals) { 
  uint8_t idx = 0;
  if (num == 0) str[idx++] = '0';
  else {
    while (num) { 
      str[idx++] = (num % 10) + '0'; 
      num /= 10; 
    }
  }
  while (idx < decimals) str[idx++] = '0'; 
  reverse(str, idx); 
  str[idx] = '\0'; 
  return idx; 
}

static char *cftoa(float num, char *str, uint8_t decimals) {
  if (num < 0) {
    *str++ = '-';
    num *= -1;
  }
  int32_t integerPart = (int32_t)num; // Extract integer part 
  float floatPart = num - (float)integerPart; // Extract floating part  
  uint8_t idx = intToStr(integerPart, str, 0); // Convert integer part to string
  if (decimals != 0) { // Check for display option after point 
    str[idx++] = '.';
    floatPart *= pow(10, decimals); // Get the value of fraction part upto given number of points after dot.
    intToStr((int32_t)round(floatPart), str + idx, decimals); 
  }
  return str;
}

static char *citoa(int32_t num, char *str, uint8_t base) {
  byte idx = 0; 
  bool isNegative = false;
  if (num == 0) {
    str[idx++] = '0';
    str[idx] = '\0';
    return str;
  }
  if (num < 0 && base == 10) {
    isNegative = true;
    num = -num;
  }
  if (base == 10) idx = intToStr(num, str, 0);
  else {
    while (num != 0) {
      byte rem = num % base;
      str[idx++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
      num = num / base;
    }
    str[idx] = '\0';
    reverse(str, idx);
  }
  if (isNegative) {
    for (byte jdx = idx; jdx > 0; jdx--) 
      str[jdx] = str[jdx - 1];
    str[0] = '-';
    str[++idx] = '\0';
  }
  return str;
}

static void configFont(RREFont &font, RRE_Font *fontInstance, uint16_t color, uint8_t scaleX, uint8_t scaleY, uint8_t bold, uint8_t letterSpacing, uint8_t minWidth, uint16_t backColor, bool backColorUsage) {
  font.setFont(fontInstance);
  backColorUsage ? font.setColor(color, backColor) : font.setColor(color);
  font.setScale(scaleX, scaleY);
  font.setBold(bold);
  font.setSpacing(letterSpacing);
  if (fontInstance->type == RRE_16B) font.setCharMinWd(minWidth < 6 ? 6 : minWidth);
  else if (fontInstance->type == RRE_V16B) font.setDigitMinWd(minWidth < 7 ? 7 : minWidth);
}

static void signal_drawCharts(Arduino_ST7789 &tft, RREFont &font, byte x, byte y, byte w, byte h, char *label, bool hover, bool idle) {
  tft.drawRoundRect(x+0, y+10, w-2*0, h-10-2*0, 10, hover ? FRAME_HOVER_CHARTS_COLOR : FRAME_CHARTS_COLOR(idle));
  tft.drawRoundRect(x+1, y+11, w-2*1, h-10-2*1, 10, hover ? FRAME_HOVER_CHARTS_COLOR : FRAME_CHARTS_COLOR(idle));
  tft.drawRoundRect(x+2, y+12, w-2*2, h-10-2*2, 10, hover ? FRAME_HOVER_CHARTS_COLOR : FRAME_CHARTS_COLOR(idle));
  tft.fillRoundRect(x+3, y+13, w-2*3, h-10-2*3, 10, BACKGROUND_CHARTS_COLOR);
  configFont(font, &rreInstance_12x16, hover ? TITLE_HOVER_CHARTS_COLOR : TITLE_CHARTS_COLOR(idle), 1, 1, 1, LETTER_SPACING_SIZE);
  font.printStr(x+(w-font.strWidth(label))/2, y-(font.getHeight())/2-LETTER_SPACING_SIZE, label);
}

static void signal_displayValue(Arduino_ST7789 &tft, RREFont &font, void* value, byte x, byte y, uint8_t amountOfDecimals, textTypes_t type, bool idle) {
  char txt[MAX_SIGNAL_SETTING_SIZE];
  configFont(font, &rreInstance_Bold13x20v, TEXT_CHARTS_COLOR(idle), 1, 2, 1, LETTER_SPACING_SIZE, 16/*, BACKGROUND_CHARTS_COLOR, true*/);
  if (type == TYPE_NONE) return;
  else if (type == TYPE_UINT) citoa(*(uint16_t*)value, txt, 10);
  else if (type == TYPE_INT) citoa(*(int32_t*)value, txt, 10);
  else if (type == TYPE_FLOAT) cftoa(*(float*)value, txt, amountOfDecimals);
  else if (type == TYPE_STRING) {
    configFont(font, &rreInstance_12x16, TEXT_CHARTS_COLOR(idle), 1, 3, 1, LETTER_SPACING_SIZE, 13);
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
        RREFont fontUnits = font;
        RREFont fontExponent = font;
        configFont(fontUnits, &rreInstance_12x16, TEXT_CHARTS_COLOR(idle), 1, 3, 1, LETTER_SPACING_SIZE, 14/*, BACKGROUND_CHARTS_COLOR, true*/);
        configFont(fontExponent, &rreInstance_Bold13x20v, TEXT_CHARTS_COLOR(idle), 1, 1, 1, LETTER_SPACING_SIZE, 12/*, BACKGROUND_CHARTS_COLOR, true*/);
        fontUnits.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-(2+font.strWidth(exponentValue)+font.strWidth(unitBase)+fontUnits.strWidth(unit)))/2 + 4, y-2, unit);
        font.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-((2+font.strWidth(exponentValue)+font.strWidth(unitBase))-fontUnits.strWidth(unit)))/2 + 4, y, unitBase);
        fontExponent.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-(font.strWidth(exponentValue)-(2+font.strWidth(unitBase)+fontUnits.strWidth(unit))))/2 + 4, y, exponentValue);
      } else {
        if (amountOfDigits > MAX_MULTIPLIER_DIGITS) return;
        char unit[2] = {'x', '\0'};
        char *multiplierValue = &(txt[1]);
        RREFont fontUnits = font;
        configFont(fontUnits, &rreInstance_12x16, TEXT_CHARTS_COLOR(idle), 1, 3, 1, LETTER_SPACING_SIZE, 14/*, BACKGROUND_CHARTS_COLOR, true*/);
        fontUnits.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-(font.strWidth(multiplierValue)+fontUnits.strWidth(unit)))/2, y-2, unit);
        font.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-(font.strWidth(multiplierValue)-fontUnits.strWidth(unit)))/2, y, multiplierValue);
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
      RREFont fontUnits = font;
      configFont(fontUnits, &rreInstance_12x16, TEXT_CHARTS_COLOR(idle), 1, 3, 1, LETTER_SPACING_SIZE, 14/*, BACKGROUND_CHARTS_COLOR, true*/);
      font.printStr(x+(CONTENT_WIDTH_FILL_OUT_CHARTS_SIZE-(fontUnits.strWidth(unit)+font.strWidth(frequencyValue)))/2 + 3, y, frequencyValue);
      fontUnits.printStr(x+(CONTENT_WIDTH_FILL_OUT_CHARTS_SIZE-(fontUnits.strWidth(unit)-font.strWidth(frequencyValue)))/2 - 3, y-1, unit);
    }
    return;
  }
  font.printStr(x+(CONTENT_WIDTH_MIDDLE_CHARTS_SIZE-font.strWidth(txt))/2, y, txt);
}

static void changeSettingSignal(Arduino_ST7789 &tft, RotaryEncoder &encoder, RREFont &font, char setting[MAX_SIGNAL_SETTING_SIZE], sig_operation_t operation) {
#ifndef MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION
  const char* multipliers[] = {"1", "10", "100", "1000", "10000"};
#else
  const char* exponents[] = {"10^0", "10^1", "10^2", "10^3", "10^4", "10^5", "10^6"};
#endif // !defined(MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION)
  const char* signalTypes[] = {"SINE", "SQUARE", "TRIANG"};
  const uint8_t numSignalTypes = sizeof(signalTypes) / sizeof(signalTypes[0]);
  static uint32_t multiplierStored = 1000;
  uint16_t blinkInterval = ACTIVE_BLINK_DIGIT_TIME;
  uint32_t blinkTimeElapsed = 0;
  bool blinkStatus = true;
  encoder.mismatchEngaged();
  tft.fillRect(0, 0, SCR_WD, 44, BACKGROUND_CHARTS_COLOR);
  tft.drawFastHLine(0, 44, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
  tft.drawFastHLine(0, 45, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
  tft.drawFastHLine(0, 46, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
  configFont(font, &rreInstance_12x16, PANEL_ELEMENTS_COLOR(false), 1, 2, 2, 2, 10);
  font.printStr(ALIGN_CENTER, 8, "PRESS TO CONFIRM");

  if (operation == SIG_OP_SET_MULT) {
    encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_MULTIPLIER_OPTIONS, ENCODER_UPPER_LIMIT_SIGNAL_MULTIPLIER_OPTIONS);
#ifndef MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION
    for (byte idx = 0; idx < ENCODER_UPPER_LIMIT_SIGNAL_MULTIPLIER_OPTIONS+1; idx++) {
      if (!strcmp(&(setting[1]), multipliers[idx])) {
        encoder.setPosition(idx);
        break;
      }
    }
    uint8_t currentPosition = encoder.getPosition();
    blinkTimeElapsed = millis();
    while (!encoder.buttonPressed()) {
      encoder.rotaryEncoderCallback();
      if (currentPosition != encoder.getPosition()) {
        currentPosition = encoder.getPosition();
        encoder.mismatchEngaged();
        multiplierStored = atoi(multipliers[currentPosition]);
        snprintf(setting, MAX_SIGNAL_SETTING_SIZE, "x%s", multipliers[currentPosition]);
        signal_drawCharts(tft, font, MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", true);
        signal_displayValue(tft, font, setting, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
      }
      blinkInterval = blinkStatus ? ACTIVE_BLINK_DIGIT_TIME : INACTIVE_BLINK_DIGIT_TIME;
      if (millis() - blinkTimeElapsed > blinkInterval) {
        blinkTimeElapsed = millis();
        blinkStatus = !blinkStatus;
        encoder.mismatchEngaged();
        signal_drawCharts(tft, font, MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", true);
        if (blinkStatus) signal_displayValue(tft, font, setting, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
        else {
          char settingInstance[MAX_SIGNAL_SETTING_SIZE] = "x";
          memset(settingInstance + 1, ' ', strlen(multipliers[currentPosition]));
          settingInstance[strlen(multipliers[currentPosition]) + 1] = '\0';
          signal_displayValue(tft, font, settingInstance, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
        }
      }
    }
#else
    for (byte idx = 0; idx < ENCODER_UPPER_LIMIT_SIGNAL_MULTIPLIER_OPTIONS+1; idx++) {
      if (!strcmp(&(setting[1]), exponents[idx])) {
        encoder.setPosition(idx);
        break;
      }
    }
    uint8_t currentPosition = encoder.getPosition();
    while (!encoder.buttonPressed()) {
      encoder.rotaryEncoderCallback();
      if (currentPosition != encoder.getPosition()) {
        currentPosition = encoder.getPosition();
        encoder.mismatchEngaged();
        snprintf(setting, MAX_SIGNAL_SETTING_SIZE, "x%s", exponents[currentPosition]);
        multiplierStored = getMultiplierFromExponentialNotation(setting);
        signal_drawCharts(tft, font, MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", true);
        signal_displayValue(tft, font, setting, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
      }
      blinkInterval = blinkStatus ? ACTIVE_BLINK_DIGIT_TIME : INACTIVE_BLINK_DIGIT_TIME;
      if (millis() - blinkTimeElapsed > blinkInterval) {
        blinkTimeElapsed = millis();
        blinkStatus = !blinkStatus;
        encoder.mismatchEngaged();
        signal_drawCharts(tft, font, MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", true);
        if (blinkStatus) signal_displayValue(tft, font, setting, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
        else {
          char settingInstance[MAX_SIGNAL_SETTING_SIZE] = "x10";
          memset(settingInstance+(SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION-1), ' ', strlen(exponents[currentPosition])-SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION);
          settingInstance[strlen(exponents[currentPosition])-SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION+(SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION-1)] = '\0';
          signal_displayValue(tft, font, settingInstance, HORIZONTAL_PADDING_MIDDLE_CHARTS-6, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, true);
        }
      }
    }
#endif // !defined(MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION)
  } else if (operation == SIG_OP_SET_TYPE) {
    encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_SIG_TYPE_OPTIONS, ENCODER_UPPER_LIMIT_SIGNAL_SIG_TYPE_OPTIONS);
    for (byte idx = 0; idx < numSignalTypes; idx++) {
      if (!strcmp(&(setting[0]), signalTypes[idx])) {
        encoder.setPosition(idx);
        break;
      }
    }
    uint8_t currentPosition = encoder.getPosition();
    blinkTimeElapsed = millis();
    while (!encoder.buttonPressed()) {
      encoder.rotaryEncoderCallback();
      if (currentPosition != encoder.getPosition()) {
        currentPosition = encoder.getPosition();
        encoder.mismatchEngaged();
        snprintf(setting, MAX_SIGNAL_SETTING_SIZE, "%s", signalTypes[currentPosition]);
        signal_drawCharts(tft, font, MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE", true);
        signal_displayValue(tft, font, setting, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING, true);
      }
      blinkInterval = blinkStatus ? ACTIVE_BLINK_DIGIT_TIME : INACTIVE_BLINK_DIGIT_TIME;
      if (millis() - blinkTimeElapsed > blinkInterval) {
        blinkTimeElapsed = millis();
        blinkStatus = !blinkStatus;
        encoder.mismatchEngaged();
        signal_drawCharts(tft, font, MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE", true);
        if (blinkStatus) signal_displayValue(tft, font, setting, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING, true);
        else {
          char settingInstance[MAX_SIGNAL_SETTING_SIZE];
          memset(settingInstance, ' ', strlen(signalTypes[currentPosition]));
          settingInstance[strlen(signalTypes[currentPosition])] = '\0';
          signal_displayValue(tft, font, settingInstance, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING, true);
        }
      }
    }
  } else if (operation == SIG_OP_SET_FREQ) {
    encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_FREQUENCY, ENCODER_UPPER_LIMIT_SIGNAL_FREQUENCY);
    char *frequencyValue = strtok(setting, "H");
    strncpy(&(setting[strlen(setting)]), "Hz", strlen("Hz"));
    setting[strlen(setting)] = '\0';
    encoder.setPosition(atoi(frequencyValue));
    encoder.setMultiplier(multiplierStored);
    uint32_t currentPosition = encoder.getPosition();
    blinkTimeElapsed = millis();
    while (!encoder.buttonPressed()) {
      encoder.rotaryEncoderCallback();
      if (currentPosition != encoder.getPosition()) {
        currentPosition = encoder.getPosition();
        encoder.mismatchEngaged();
        snprintf(setting, MAX_SIGNAL_SETTING_SIZE, "%uHz", currentPosition);
        signal_drawCharts(tft, font, MARGIN_FILL_OUT_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY", true);
        signal_displayValue(tft, font, setting, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID, true);
      }
      blinkInterval = blinkStatus ? ACTIVE_BLINK_DIGIT_TIME : INACTIVE_BLINK_DIGIT_TIME;
      if (millis() - blinkTimeElapsed > blinkInterval) {
        blinkTimeElapsed = millis();
        blinkStatus = !blinkStatus;
        encoder.mismatchEngaged();
        signal_drawCharts(tft, font, MARGIN_FILL_OUT_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY", true);
        if (blinkStatus) signal_displayValue(tft, font, setting, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID, true);
        else {
          char settingInstance[MAX_SIGNAL_SETTING_SIZE];
          memset(settingInstance, ' ', strlen(setting) - strlen("Hz"));
          strncpy(&(settingInstance[strlen(setting) - strlen("Hz")]), "Hz", strlen("Hz"));
          settingInstance[strlen(setting)] = '\0';
          signal_displayValue(tft, font, settingInstance, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID, true);
        }
      }
    }
    encoder.usageMultiplier(false);
  }
}

static void printMenuGraphics(Arduino_ST7789 &tft) {
  uint8_t chunkSize = 30; // It is recommended not to modify this variable so as not to slow down or affect the representation of the image.
  // If the value of "chunkSize" is 30, 14400 bytes will be stored, that is, 72% of the SRAM memory occupied for the STM32F103x family of microcontrollers.
  uint16_t image[SCR_WD * chunkSize];

  tft.clearScreen();
  drawGradientMenu(tft);
  // TODO: Fill out the entire menu background with the "BACKGROUND_COLOR_MENU" color
  // tft.fillRect(0, 0, SCR_WD, SCR_HT, BACKGROUND_COLOR_MENU);
  File fileImg = SD.open("mFrame.bin", FILE_READ);
  if (!fileImg) {
    SPI.setDataSize(DATA_SIZE_16BIT);
    return;
  }

  for (uint8_t idx = 0; idx < FRAME_GRAPH_HT / chunkSize; idx++) {
    // NOTE: Optimized alternative. An additional 4% of SRAM memory is used for the STM32 family of microcontrollers.
    for (uint8_t jdx = 0; jdx < (SCR_WD * chunkSize) / 240; jdx++) 
      readBlock(fileImg, &(image[jdx * 240]), sizeof(uint16_t) * 240);
    SPI.setDataSize(DATA_SIZE_16BIT);
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
    SPI.setDataSize(DATA_SIZE_16BIT);
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
    SPI.setDataSize(DATA_SIZE_16BIT);
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

void displayMenu(Arduino_ST7789 &tft, RotaryEncoder &encoder, RREFont &font, operation_t &operation) {
  static bool alreadyDrawFrame = false;
  static operation_t currentOperation = OP_NONE;
  encoder.rotaryEncoderCallback();
  if (!alreadyDrawFrame) {
    encoder.mismatchEngaged();
    printMenuGraphics(tft); 
    alreadyDrawFrame = true;
  }
  if (currentOperation != encoder.getPosition()) {
    currentOperation = (operation_t)encoder.getPosition();
    tft.fillRoundRect(44, 88, 147, 63, 10, BACKGROUND_COLOR_MENU);
    configFont(font, &rreInstance_12x16, TEXT_COLOR_MENU, 1, 1, 1, LETTER_SPACING_SIZE, 12, BACKGROUND_COLOR_MENU, true);
    switch (currentOperation) {
      case OP_CALCULATE_INDUCTANCE:
        font.printStr(ALIGN_CENTER, 100, "INDUCTANCE");
        font.printStr(ALIGN_CENTER, 123, "METER");
        break;
      case OP_CALCULATE_QUALITY_FACTOR:
        font.printStr(ALIGN_CENTER, 100, "QUALITY");
        font.printStr(ALIGN_CENTER, 123, "METER");
        break;
      case OP_GENERATE_SIGNALS:
        font.printStr(ALIGN_CENTER, 100, "SIGNAL");
        font.printStr(ALIGN_CENTER, 123, "GENERATOR");
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

void processOperation(Arduino_ST7789 &tft, RotaryEncoder &encoder, RREFont &font, operation_t &operation) {
  switch (operation) {
    case OP_CALCULATE_INDUCTANCE:
      encoder.mismatchEngaged();
      tft.clearScreen();
      printFromSD(tft, "induc.bin");
      delay(2000);
      tft.clearScreen();
      printFromSD(tft, "mBack.bin");
      while (!encoder.buttonPressed()) {
        // TODO: It's not implemented yet
      }
      encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
      encoder.setPosition(OP_CALCULATE_INDUCTANCE);
      operation = OP_NONE;
      break;
    case OP_CALCULATE_QUALITY_FACTOR:
      encoder.mismatchEngaged();
      tft.clearScreen();
      printFromSD(tft, "qmeter.bin");
      delay(2000);
      tft.clearScreen();
      printFromSD(tft, "mBack.bin");
      while (!encoder.buttonPressed()) {
        // TODO: It's not implemented yet
      }
      encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
      encoder.setPosition(OP_CALCULATE_QUALITY_FACTOR);
      operation = OP_NONE;
      break;
    case OP_GENERATE_SIGNALS:
      encoder.mismatchEngaged();
      tft.clearScreen();
      printFromSD(tft, "sig.bin");
      delay(2000);
      tft.clearScreen();
      printFromSD(tft, "mBack.bin");
      settingSignal(tft, encoder, font);
      encoder.changeRange(ENCODER_LOWER_LIMIT_MENU, ENCODER_UPPER_LIMIT_MENU);
      encoder.setPosition(OP_GENERATE_SIGNALS);
      operation = OP_NONE;
      break;
    case OP_NONE:
    default:
      break;
  }
}

void settingSignal(Arduino_ST7789 &tft, RotaryEncoder &encoder, RREFont &font) {
  encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_PANEL, ENCODER_UPPER_LIMIT_SIGNAL_PANEL);
  encoder.setPosition(SIG_OP_COMPLETE);
  while (true) {
    static bool alreadyDrawPanel = false;
    static sig_operation_t signalOperation = SIG_OP_CANCEL;
    static char defaultMultiplier[MAX_SIGNAL_SETTING_SIZE];
    static char defaultSignalType[MAX_SIGNAL_SETTING_SIZE];
    static char defaultFrequency[MAX_SIGNAL_SETTING_SIZE];
    encoder.rotaryEncoderCallback();
    if (!alreadyDrawPanel) {
      encoder.mismatchEngaged();
      if (*defaultMultiplier == '\0' || *defaultSignalType == '\0' || *defaultFrequency == '\0') {
#ifndef MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION
        defaultMultiplier[0] = 'x';
        citoa(DEFAULT_SIGNAL_MULTIPLIER, &(defaultMultiplier[1]), 10);
#else
        strncpy(defaultMultiplier, "x10^", strlen("x10^"));
        citoa(DEFAULT_SIGNAL_MULTIPLIER, &(defaultMultiplier[SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION]), 10);
#endif // !defined(MULTIPLIER_EXPONENTIAL_NOTATION_REPRESENTATION)
        if (DEFAULT_SIGNAL_TYPE == SIG_SINE) {
          strncpy(defaultSignalType, "SINE", strlen("SINE"));
          defaultSignalType[strlen("SINE")] = '\0';
        } else if (DEFAULT_SIGNAL_TYPE == SIG_SQUARE) {
          strncpy(defaultSignalType, "SQUARE", strlen("SQUARE"));
          defaultSignalType[strlen("SQUARE")] = '\0';
        } else if (DEFAULT_SIGNAL_TYPE == SIG_TRIANGLE) {
          strncpy(defaultSignalType, "TRIANG", strlen("TRIANG"));
          defaultSignalType[strlen("TRIANG")] = '\0';
        }
        citoa(DEFAULT_SIGNAL_FREQUENCY, &(defaultFrequency[0]), 10);
        strncpy(&(defaultFrequency[strlen(defaultFrequency)]), "Hz", strlen("Hz"));
        defaultFrequency[strlen(defaultFrequency)] = '\0';
        signal_displayValue(tft, font, defaultMultiplier, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID);
        signal_displayValue(tft, font, defaultSignalType, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING);
        signal_displayValue(tft, font, defaultFrequency, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID);
      }
      alreadyDrawPanel = true;
    }
    if (signalOperation != encoder.getPosition()) {
      encoder.mismatchEngaged();
      signalOperation = (sig_operation_t)encoder.getPosition();
      signal_drawCharts(tft, font, MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER");
      signal_drawCharts(tft, font, MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE");
      signal_drawCharts(tft, font, MARGIN_FILL_OUT_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY");
      signal_displayValue(tft, font, defaultMultiplier, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID);
      signal_displayValue(tft, font, defaultSignalType, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING);
      signal_displayValue(tft, font, defaultFrequency, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID);
      tft.fillRect(0, 0, SCR_WD, 44, BACKGROUND_CHARTS_COLOR);
      tft.drawFastHLine(0, 44, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
      tft.drawFastHLine(0, 45, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
      tft.drawFastHLine(0, 46, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
      switch (signalOperation) {
        case SIG_OP_CANCEL:
          configFont(font, &rreInstance_12x16, PANEL_ELEMENTS_INACTIVE_COLOR, 1, 2, 2, 2, 10);
          font.printStr(ALIGN_CENTER,8,"PRESS TO EXIT");
          break;
        case SIG_OP_COMPLETE:
          configFont(font, &rreInstance_12x16, PANEL_ELEMENTS_ACTIVE_COLOR(true), 1, 2, 2, 2, 10);
          font.printStr(ALIGN_CENTER,8,"PRESS TO START");
          break;
        case SIG_OP_SET_MULT:
          configFont(font, &rreInstance_12x16, PANEL_ELEMENTS_COLOR(true), 1, 2, 2, 2, 10);
          font.printStr(ALIGN_CENTER,8,"PRESS TO SET");
          signal_drawCharts(tft, font, MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", true);
          signal_displayValue(tft, font, defaultMultiplier, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID);
          break;
        case SIG_OP_SET_TYPE:
          configFont(font, &rreInstance_12x16, PANEL_ELEMENTS_COLOR(true), 1, 2, 2, 2, 10);
          font.printStr(ALIGN_CENTER,8,"PRESS TO SET");
          signal_drawCharts(tft, font, MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE", true);
          signal_displayValue(tft, font, defaultSignalType, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING);
          break;
        case SIG_OP_SET_FREQ:
          configFont(font, &rreInstance_12x16, PANEL_ELEMENTS_COLOR(true), 1, 2, 2, 2, 10);
          font.printStr(ALIGN_CENTER,8,"PRESS TO SET");
          signal_drawCharts(tft, font, MARGIN_FILL_OUT_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY", true);
          signal_displayValue(tft, font, defaultFrequency, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID);
          break;
        default:
          break;
      }
    }
    if (encoder.buttonPressed()) {
      if (signalOperation == SIG_OP_CANCEL) {
        alreadyDrawPanel = false;
        break;
      } else if (signalOperation == SIG_OP_COMPLETE) {
        encoder.mismatchEngaged();
        signal_drawCharts(tft, font, MARGIN_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER", false, false);
        signal_drawCharts(tft, font, MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE", false, false);
        signal_drawCharts(tft, font, MARGIN_FILL_OUT_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY", false, false);
        signal_displayValue(tft, font, defaultMultiplier, HORIZONTAL_PADDING_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_HIBRID, false);
        signal_displayValue(tft, font, defaultSignalType, HORIZONTAL_PADDING_MIDDLE_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, TYPE_STRING, false);
        signal_displayValue(tft, font, defaultFrequency, HORIZONTAL_PADDING_FILL_OUT_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, TYPE_HIBRID, false);
        tft.fillRect(0, 0, SCR_WD, 44, BACKGROUND_CHARTS_COLOR);
        tft.drawFastHLine(0, 44, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
        tft.drawFastHLine(0, 45, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
        tft.drawFastHLine(0, 46, SCR_WD, FRAME_PANEL_ELEMENTS_COLOR);
        configFont(font, &rreInstance_12x16, PANEL_ELEMENTS_ACTIVE_COLOR(false), 1, 2, 2, 2, 10);
        font.printStr(ALIGN_CENTER,8,"PRESS TO STOP");
        
        while (!encoder.buttonPressed()); // TODO: It's not implemented yet

        encoder.setPosition(SIG_OP_COMPLETE);
        signalOperation = SIG_OP_CANCEL;
      } else if (signalOperation == SIG_OP_SET_MULT) {
        changeSettingSignal(tft, encoder, font, defaultMultiplier, signalOperation);
        encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_PANEL, ENCODER_UPPER_LIMIT_SIGNAL_PANEL);
        encoder.setPosition(SIG_OP_SET_MULT);
        signalOperation = SIG_OP_CANCEL;
      } else if (signalOperation == SIG_OP_SET_TYPE) {
        changeSettingSignal(tft, encoder, font, defaultSignalType, signalOperation);
        encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_PANEL, ENCODER_UPPER_LIMIT_SIGNAL_PANEL);
        encoder.setPosition(SIG_OP_SET_TYPE);
        signalOperation = SIG_OP_CANCEL;
      } else if (signalOperation == SIG_OP_SET_FREQ) {
        changeSettingSignal(tft, encoder, font, defaultFrequency, signalOperation);
        encoder.changeRange(ENCODER_LOWER_LIMIT_SIGNAL_PANEL, ENCODER_UPPER_LIMIT_SIGNAL_PANEL);
        encoder.setPosition(SIG_OP_SET_FREQ);
        signalOperation = SIG_OP_CANCEL;
      }
    }
  }
}
