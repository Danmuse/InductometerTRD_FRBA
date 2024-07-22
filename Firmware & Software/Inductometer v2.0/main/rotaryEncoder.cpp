/*/*!
 * @file RotaryEncoder.cpp
 * @par Author & Doxygen Editor
 *  Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 16/03/2024 16:05:47
 * @version 1.0
 */

#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(byte clockPin, byte dataPin) :
m_clockwiseRotationStatus{true}, m_counterclockwiseRotationStatus{true}, m_buttonUsage{false}, m_clockPin{clockPin}, m_dataPin{dataPin}, m_position{0} {
  pinMode(this->m_clockPin, INPUT);
  pinMode(this->m_dataPin, INPUT);
  this->m_counterclockwiseRotationStatus = digitalRead(this->m_dataPin);
  this->changeRange();
}

RotaryEncoder::RotaryEncoder(byte buttonPin, byte clockPin, byte dataPin) :
m_clockwiseRotationStatus{true}, m_counterclockwiseRotationStatus{true}, m_buttonUsage{true}, m_buttonPin{buttonPin}, m_clockPin{clockPin}, m_dataPin{dataPin}, m_position{0} {
  pinMode(this->m_buttonPin, INPUT_PULLUP);
  pinMode(this->m_clockPin, INPUT);
  pinMode(this->m_dataPin, INPUT);
  this->m_counterclockwiseRotationStatus = digitalRead(this->m_dataPin);
  this->changeRange();
}

void RotaryEncoder::rotaryEncoderCallback(void) {
  static uint32_t previousMillis = 0;
  uint32_t currentMillis = millis();
  this->m_clockwiseRotationStatus = digitalRead(this->m_clockPin);
  if ((currentMillis - previousMillis) > 5) {
    previousMillis = currentMillis;
    if (this->m_clockwiseRotationStatus != this->m_counterclockwiseRotationStatus /* && this->m_clockwiseRotationStatus == true */) {
      if (digitalRead(this->m_dataPin) != this->m_clockwiseRotationStatus) { if (this->m_position < this->m_upperLimit) this->m_position++; } 
      else { if (this->m_position > this->m_lowerLimit) this->m_position--; }
    }
    this->m_counterclockwiseRotationStatus = this->m_clockwiseRotationStatus;
  }
}

void RotaryEncoder::setPosition(int16_t position) {
  if (position < this->m_lowerLimit) this->m_position = this->m_lowerLimit;
  else if (position > this->m_upperLimit) this->m_position = this->m_upperLimit;
  else this->m_position = position;
}

int16_t RotaryEncoder::getPosition(void) const {
  return this->m_position;
}

bool RotaryEncoder::buttonPressed(void) const {
#if defined(ESP8266) || (__STM32F1__)
    return this->m_buttonUsage ? !digitalRead(this->m_buttonPin) : false;
#else
    return this->m_buttonUsage ? digitalRead(this->m_buttonPin) : false;
#endif
}

void RotaryEncoder::changeRange(int16_t lowerLimit, int16_t upperLimit) {
  this->m_lowerLimit = lowerLimit;
  this->m_upperLimit = upperLimit;
  if (this->m_position < lowerLimit) this->m_position = lowerLimit;
  else if (this->m_position > upperLimit) this->m_position = upperLimit;
}
