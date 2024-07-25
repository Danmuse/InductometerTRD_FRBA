#define TFT_DC  PB1
#define TFT_RST PB0
#define TFT_CS  PA3
#define SCR_WD   240
#define SCR_HT   240
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_STM.h>
Arduino_ST7789 tft(TFT_DC, TFT_RST, TFT_CS);

#include <stdio.h>
#include "RREFont.h"

#include "rre_5x8.h"
#include "rre_6x8.h"
//#include "rre_7x8_nosort.h"
#include "rre_bold_6x8.h"
#include "rre_7x12.h"
#include "rre_8x12.h"
#include "rre_vga_8x16.h"
#include "rre_c64_7x7.h"
#include "rre_c64_7x8.h"
#include "rre_term_10x16.h"
#include "rre_nadine_16x16.h"
#include "rre_arialb_16.h"
#include "rre_arialn_16.h"
#include "rre_ariali_16.h"
#include "rre_ocr_16x24.h"
#include "rre_12x16.h"
#include "rre_term_10x16.h"

#include "rre_bold13x20v.h"
#include "rre_bold13x20.h"
#include "rre_chicago_20x24.h"
#include "rre_ubuntu_32.h"
#include "rre_ubuntub_32.h"
#include "rre_digi_19x15.h"

#include "rre_fjg_8x16.h"
#include "rre_c_8x16.h"
#include "rre_x_8x16.h"
#include "rre_comp_8x16.h"
#include "rre_ncw_8x16.h"
#include "rre_kappa_8x16.h"
#include "rre_script_8x16.h"
#include "rre_fixed_8x16.h"

RREFont font;

#define HORIZONTAL_PADDING_CHARTS 6*2
#define VERTICAL_PADDING_CHARTS(row) 80*(row%3)+1+(row%3)-1+25-8*(2*(row%2))
#define VERTICAL_POSITION_TITLE_CHARTS(row) 80*(row%3)+1+(row%3)-1-8*(2*(row%2))
#define MARGIN_LEFT_CENTER_MIDDLE_CHARTS SCR_WD/2+2

#define HEIGHT_CHARTS 80-2
#define WIDTH_MIDDLE_CHARTS SCR_WD/2-2
#define FRAME_MIDDLE_CHARTS_SIZE WIDTH_MIDDLE_CHARTS-2*HORIZONTAL_PADDING_CHARTS
#define WIDTH_FILL_OUT_CHARTS SCR_WD
#define FRAME_FILL_OUT_CHARTS_SIZE WIDTH_FILL_OUT_CHARTS-2*HORIZONTAL_PADDING_CHARTS

#define LETTER_SPACING_SIZE 1
#define MAX_MULTIPLIER_DIGITS 5
#define MAX_FREQUENCY_DIGITS 10

const uint16_t gradientHr_ST7789[] PROGMEM = {
  // The following data is an image with a horizontal line gradient to be displayed on a single row of the ST7789 TFT.
  // First row fragment that will represent the beginning to half of its width (240 pixels)
  0x0000,0x0000,0x0021,0x0021,0x0021,0x0041,0x0042,0x0042,0x0062,0x0062,0x0063,0x0083,0x0083,0x0084,0x00A4,0x00A4,
  0x00A4,0x00C5,0x00C5,0x00C5,0x00E5,0x00E6,0x00E6,0x0106,0x0107,0x0107,0x0127,0x0127,0x0148,0x0148,0x0148,0x0169,
  0x0169,0x0169,0x0189,0x018A,0x018A,0x01AA,0x01AB,0x01AB,0x01CB,0x01CB,0x01CC,0x01EC,0x01EC,0x01EC,0x020D,0x020D,
  0x020D,0x022D,0x022E,0x022E,0x024E,0x024F,0x024F,0x026F,0x0270,0x0270,0x0290,0x0290,0x02B1,0x02B1,0x02B1,0x02B2,
  0x02D2,0x02D2,0x02D2,0x02F3,0x02F3,0x02F3,0x0313,0x0314,0x0314,0x0334,0x0335,0x0335,0x0355,0x0355,0x0356,0x0376,
  0x0376,0x0376,0x0397,0x0397,0x03B7,0x03B8,0x03B8,0x03D8,0x03D8,0x03D9,0x03F9,0x03F9,0x03FA,0x041A,0x041A,0x041A,
  0x043B,0x043B,0x043B,0x045B,0x045C,0x045C,0x047C,0x047D,0x047D,0x049D,0x049D,0x049E,0x04BE,0x04BE,0x04BF,0x04DF,
  0x051F,0x051F,0x051F,0x051F,0x051F,0x051F,0x055F,0x055F,
  // Second row fragment that will represent half of its width (240 pixels) to the end.
  0x055F,0x055F,0x051F,0x051F,0x051F,0x051F,0x051F,0x051F,0x04DF,0x04BF,0x04BE,0x04BE,0x049E,0x049D,0x049D,0x047D,
  0x047D,0x047C,0x045C,0x045C,0x045B,0x043B,0x043B,0x043B,0x041A,0x041A,0x041A,0x03FA,0x03F9,0x03F9,0x03D9,0x03D8,
  0x03D8,0x03B8,0x03B8,0x03B7,0x0397,0x0397,0x0376,0x0376,0x0376,0x0356,0x0355,0x0355,0x0335,0x0335,0x0334,0x0314,
  0x0314,0x0313,0x02F3,0x02F3,0x02F3,0x02D2,0x02D2,0x02D2,0x02B2,0x02B1,0x02B1,0x02B1,0x0290,0x0290,0x0270,0x0270,
  0x026F,0x024F,0x024F,0x024E,0x022E,0x022E,0x022D,0x020D,0x020D,0x020D,0x01EC,0x01EC,0x01EC,0x01CC,0x01CB,0x01CB,
  0x01AB,0x01AB,0x01AA,0x018A,0x018A,0x0189,0x0169,0x0169,0x0169,0x0148,0x0148,0x0148,0x0127,0x0127,0x0107,0x0107,
  0x0106,0x00E6,0x00E6,0x00E5,0x00C5,0x00C5,0x00C5,0x00A4,0x00A4,0x00A4,0x0084,0x0083,0x0083,0x0063,0x0062,0x0062,
  0x0042,0x0042,0x0041,0x0021,0x0021,0x0021,0x0000,0x0000
};

// Needed for RREFont library initialization, define your fillRect
void customRect(int x, int y, int w, int h, int c) { return tft.fillRect(x, y, w, h, c); }

typedef enum {
  TYPE_NONE = 0,
  TYPE_UINT = 1,
  TYPE_INT = 2,
  TYPE_FLOAT = 3,
  TYPE_STRING = 4,
  TYPE_HIBRID = 5
} textTypes_t;

void setup() {
  Serial.begin(115200);
  tft.init(SCR_WD, SCR_HT);
  font.init(customRect, SCR_WD, SCR_HT); // custom fillRect function and screen width and height values
  tft.fillScreen(BLACK);
}

void showVal(void* value, byte x, byte y, uint8_t amountOfDecimals, uint16_t color, textTypes_t type);

void reverse(char str[], byte length) {
  uint8_t start = 0, end = length - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    end--; start++;
  }
}

char *citoa(int32_t num, char* str, uint8_t base) {
  byte idx = 0; bool isNegative = false;
  if (num == 0) {
    str[idx++] = '0';
    str[idx] = '\0';
    return str;
  }
  // In standard itoa(), negative numbers are handled
  // only with base 10. Otherwise numbers are
  // considered unsigned.
  if (num < 0 && base == 10) {
    isNegative = true;
    num *= -1;
  }
  while (num != 0) { // Process individual digits
    byte rem = num % base;
    str[idx++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }
  if (isNegative) str[idx++] = '-'; // If number is negative, append '-'
  str[idx] = '\0'; // Append string terminator
  reverse(str, idx); // Reverse the string
  return str;
}

uint8_t intToStr(int32_t x, char str[], int decimals) { 
    uint8_t idx = 0;
    if (x == 0) str[idx++] = '0';
    else {
      while (x) { 
        str[idx++] = (x % 10) + '0'; 
        x = x / 10; 
      }
    }
    // If number of digits required is more
    while (idx < decimals) str[idx++] = '0'; 
    reverse(str, idx); 
    str[idx] = '\0'; 
    return idx; 
} 

void cftoa(float num, char* str, uint8_t decimals) { 
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
} 

const uint16_t flColor  = RGBto565(36,36,36);
const uint16_t lnColor  = RGBto565(180,180,180); // RGBto565(255,150,250);
const uint16_t ln2Color = RGBto565(180,180,180);
const uint16_t labColor = RGBto565(227,236,240);
const uint16_t v3Color  = RGBto565(236,236,236); // RGBto565(120,255,255);
const uint16_t v4Color  = RGBto565(236,236,236); // RGBto565(255,120,120);
const uint16_t v5Color  = RGBto565(236,236,236); // RGBto565(150,150,200);

static unsigned long DrawImageFTest(void) {
  unsigned long start = millis();
  /*for(int i=0;i<5*4;i++)*/ for(int y=0;y<SCR_HT;y++) tft.drawImageF(0,y,SCR_WD,1,gradientHr_ST7789/*+(((y>>2)+i)&0xf)*/);
  return millis()-start;
}

bool isNumberExt(uint8_t character) {
  return isdigit(character) || character=='-' || character=='+' || character=='.' || character==' ';
}

void setBigNumFont() {
  font.setFont(&rre_Bold13x20v);
  font.setSpacing(LETTER_SPACING_SIZE);
  font.setScale(1,2);
  font.setDigitMinWd(16);
}

void drawField(byte x, byte y, byte w, byte h, char *label, uint16_t col = lnColor) {
  tft.drawRoundRect(x+0, y+10, w-2*0, h-10-2*0, 10, col);
  tft.drawRoundRect(x+1, y+11, w-2*1, h-10-2*1, 10, col);
  tft.drawRoundRect(x+2, y+12, w-2*2, h-10-2*2, 10, col);
  tft.fillRoundRect(x+3, y+13, w-2*3, h-10-2*3, 10, flColor);
  font.setFont(&rre_12x16);
  font.setScale(1);
  font.setColor(labColor/*, BLACK */);
  font.printStr(x+(w-font.strWidth(label))/2, y-(font.getHeight())/2-LETTER_SPACING_SIZE, label);
}

void showVal(void* value, byte x, byte y, uint8_t amountOfDecimals, uint16_t color, textTypes_t type) {
  char txt[14];
  setBigNumFont();
  if (type == TYPE_NONE) return;
  else if (type == TYPE_UINT) citoa(*(uint16_t*)value, txt, 10);
  else if (type == TYPE_INT) citoa(*(int32_t*)value, txt, 10);
  else if (type == TYPE_FLOAT) cftoa(*(float*)value, txt, amountOfDecimals);
  else if (type == TYPE_STRING) {
    font.setFont(&rre_12x16);
    font.setSpacing(LETTER_SPACING_SIZE);
    font.setScale(1,3); font.setBold(1);
    font.setCharMinWd(12);
    strncpy(txt, (char*)value, strlen((char*)value));
    txt[strlen((char*)value)] = '\0';
    y -= 2;
  }
  else if (type == TYPE_HIBRID) {
    uint8_t amountOfDigits = 0;
    strncpy(txt, (char*)value, strlen((char*)value));
    txt[strlen((char*)value)] = '\0';
    if (txt[0] == 'x') {
      for (byte idx = 1; idx < strlen(txt) - 1 && txt[idx] != '\0'; idx++) {
        Serial.println(txt[idx]);
        if (!isNumberExt(txt[idx])) return;
        amountOfDigits++;
      }
      if (amountOfDigits > MAX_MULTIPLIER_DIGITS) return;
      char *multiplierValue = &(txt[1]);
      char unit[2] = {'x', '\0'};
      RREFont fontUnits = font;
      fontUnits.setFont(&rre_12x16);
      fontUnits.setSpacing(LETTER_SPACING_SIZE);
      fontUnits.setScale(1,3); font.setBold(1);
      fontUnits.setCharMinWd(14);
      fontUnits.setColor(color);
      fontUnits.printStr(x+(FRAME_MIDDLE_CHARTS_SIZE-(font.strWidth(multiplierValue)+fontUnits.strWidth(unit)))/2, y-2, unit);
      font.printStr(x+(FRAME_MIDDLE_CHARTS_SIZE-(font.strWidth(multiplierValue)-fontUnits.strWidth(unit)))/2, y, multiplierValue);
    } else if (txt[strlen(txt) - 2] == 'H' && txt[strlen(txt) - 1] == 'z') {
      for (byte idx = 0; idx < strlen(txt) - 1 && txt[idx] != 'H'; idx++) {
        if (!isNumberExt(txt[idx])) return;
        amountOfDigits++;
      }
      if (amountOfDigits > MAX_FREQUENCY_DIGITS) return;
      char *frequencyValue = strtok(txt, "H");
      char unit[4] = {' ', 'H', 'z', '\0'};
      RREFont fontUnits = font;
      fontUnits.setFont(&rre_12x16);
      fontUnits.setSpacing(LETTER_SPACING_SIZE);
      fontUnits.setScale(1,3); font.setBold(1);
      fontUnits.setCharMinWd(14);
      fontUnits.setColor(color);
      font.printStr(x+(FRAME_FILL_OUT_CHARTS_SIZE-(fontUnits.strWidth(unit)+font.strWidth(frequencyValue)))/2 + 3, y, frequencyValue);
      fontUnits.printStr(x+(FRAME_FILL_OUT_CHARTS_SIZE-(fontUnits.strWidth(unit)-font.strWidth(frequencyValue)))/2 - 3, y-1, unit);
    }
    return;
  }
  font.setColor(color/*, BLACK */);
  font.printStr(x+(FRAME_MIDDLE_CHARTS_SIZE-font.strWidth(txt))/2, y, txt);
}

void showStats(char *name) {
  DrawImageFTest();
  font.setColor(WHITE);
  if (name == "12x16") { font.setScale(1,2); font.setBold(2); font.setSpacing(2); }
  else { font.setScale(2,3); font.setBold(2); font.setSpacing(1); }
  font.printStr(ALIGN_CENTER,8,"SETTINGS");
  font.setScale(1); font.setBold(1); // font.setSpacing(1);

  drawField(0, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "MULTIPLIER");
  drawField(MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_POSITION_TITLE_CHARTS(1), WIDTH_MIDDLE_CHARTS, HEIGHT_CHARTS, "SIG. TYPE");
  drawField(0, VERTICAL_POSITION_TITLE_CHARTS(2), WIDTH_FILL_OUT_CHARTS, HEIGHT_CHARTS, "FREQUENCY", ln2Color);

  /*
  int wv = font.strWidth("00000.00");
  font.setColor(v5Color); font.printStr(22+wv, 80*2+2+25-8*(2*0), " Hz");
  wv = font.strWidth("1111.1");
  int wv2 = font.strWidth("111");
  font.setFont(&rre_12x16);
  font.setScale(1,2);
  font.setColor(v4Color); font.printStr(21+120+wv+2,82+22+14,"m");
  */

  // int v3 = 33960; // -3396;
  // showVal(&v3, PADDING_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, v3Color, TYPE_INT);

  char* v3 = "x1000";
  char* v4 = "SQUARE";
  char* v5 = "12000000Hz";
  showVal(v3, HORIZONTAL_PADDING_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, v3Color, TYPE_HIBRID);
  showVal(v4, HORIZONTAL_PADDING_CHARTS + MARGIN_LEFT_CENTER_MIDDLE_CHARTS, VERTICAL_PADDING_CHARTS(1), 0, v4Color, TYPE_STRING);
  showVal(v5, HORIZONTAL_PADDING_CHARTS, VERTICAL_PADDING_CHARTS(2), 0, v5Color, TYPE_HIBRID);
  
  delay(7500); font.setSpacing(1);
}

void showFont(char *name) {
  tft.fillScreen(BLACK);
  // font.setCR(0);
  font.setColor(WHITE);
  font.printStr(ALIGN_CENTER,50+font.getHeight()*0*2,name);
  font.printStr(ALIGN_CENTER,50+font.getHeight()*1*2,"431.520789");
  font.printStr(ALIGN_CENTER,50+font.getHeight()*2*2,"Inductometer");
  font.printStr(ALIGN_CENTER,50+font.getHeight()*3*2,"ABCDEFGHIJ");
  font.printStr(ALIGN_CENTER,50+font.getHeight()*4*2,"abcdefghij");
  delay(1000);
}

void loop() {
  // use REDUCE_MEM in RREFont.h to free some flash memory
  
  // font.setFont(&rre_c64_7x8); showFont("[c64_7x8]");
  // font.setFont(&rre_c64_7x7); showFont("[c64_7x7]");
  // font.setFont(&rre_5x8); showFont("[5x8]");
  // font.setFont(&rre_6x8); showFont("[6x8]");
  // font.setFont(&rre_bold_6x8); showFont("[bold_6x8]");
  // font.setFont(&rre_7x12); showFont("[7x12]");
  // font.setFont(&rre_8x12); showFont("[8x12]");
  // font.setFont(&rre_vga_8x16); showFont("[vga_8x16]");
  
  // font.setFont(&rre_arialn_16); showFont("arialn_16");
  // font.setFont(&rre_arialb_16); showFont("arialb_16");
  // font.setFont(&rre_ariali_16); showFont("[ariali_16]");
  font.setFont(&rre_12x16); showFont("12x16"); showStats("12x16");
  // font.setFont(&rre_term_10x16); showFont("term_10x16");
  // font.setFont(&rre_nadine_16x16); showFont("[nadine_16x16]");
  // font.setFont(&rre_ocr_16x24); showFont("[ocr_16x24]");  
  // font.setFont(&rre_fjg_8x16); showFont("fjg_8x16");
  // font.setFont(&rre_kappa_8x16); showFont("kappa_8x16");

  // font.setFont(&rre_ncw_8x16); showFont("[ncw_8x16]");
  // font.setFont(&rre_c_8x16); showFont("[c_8x16]");
  //  !! font.setScale(2); font.setBold(2);
  //  !! font.setFont(&rre_fixed_8x16); showFont("[fixed_8x16]"); showStats("[fixed_8x16]");
  // font.setFont(&rre_x_8x16); showFont("[x_8x16]");
  // font.setFont(&rre_comp_8x16); showFont("[comp_8x16]");
  // font.setFont(&rre_script_8x16); showFont("[script_8x16]");
  
  // font.setFont(&rre_Bold13x20); showFont("13:20");
  // font.setFont(&rre_chicago_20x24); showFont("[chicago_20x24]");
  // font.setFont(&rre_digi_19x15); showFont("[digi_19x15]");

  // font.setFont(&rre_ubuntu_32); showFont("[ubuntu_32]");
  // font.setFont(&rre_ubuntub_32); showFont("[ubuntub_32]");
}
