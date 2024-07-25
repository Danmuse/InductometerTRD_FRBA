#ifndef _UTILS_H_
#define _UTILS_H_

#include <Arduino.h>
#include "hardwareConfig.h"

#define FLOAT_DECIMALS_PRECISION 4
#define SCIENTIFIC_NOTATION_EXPONENTIAL_POSITION 4

/*
bool isNumberExt(uint8_t character) {
  return isdigit(character) || character=='-' || character=='+' || character=='.' || character==' ';
}

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

*/

#endif // _UTILS_H_
