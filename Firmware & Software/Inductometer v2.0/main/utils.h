#ifndef _UTILS_H_
#define _UTILS_H_

#include <Arduino.h>
#include "hardwareConfig.h"
#include <SPI.h>

#define SPI_PERIPHERAL_SPEED (36000000UL)

#define ERR_SD_NOT_FOUND "MICRO SD CARD\\nNOT FOUND"
#define ERR_SECTION_NOT_AVAILABLE "NOT AVAILABLE"

#define FLOAT_DECIMALS_PRECISION 4
#define SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION 4

static bool isNumberExt(uint8_t character);
static void reverse(char *str, byte length);
static uint8_t intToStr(int32_t num, char *str, int decimals);
static char *cftoa(float num, char *str, uint8_t decimals);
static char *citoa(int32_t num, char *str, uint8_t base);

static void spiTransaction(SPISettings settings, void (*operation)());

static bool isNumberExt(uint8_t character) {
  return isdigit(character) || character=='-' || character=='+' || character=='.' || character==' ';
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

static void spiTransaction(SPISettings settings, void (*operation)()) {
    SPI.endTransaction();
    SPI.beginTransaction(settings);
    operation();
}

#endif // _UTILS_H_
