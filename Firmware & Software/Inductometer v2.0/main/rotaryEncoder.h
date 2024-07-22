/*!
 * @file RotaryEncoder.h
 * @par Author & Doxygen Editor
 *  Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 16/03/2024 16:05:47
 * @version 1.0
 * @brief Class for controlling KY040 Rotary Encoder
 * @details @par Description
 *  The <b>RotaryEncoder</b> class provides a set of functions for interfacing with and controlling the KY040 Rotary Encoder. @n
 *  This class enables users to easily monitor and handle rotational movements of the encoder, allowing for precise input control in various applications. @n
 *  It abstracts the low-level interactions with the rotary encoder, providing a high-level interface for detecting changes in position, handling rotation events, @n
 *  and implementing custom functionality based on the encoder's input.
 */

#ifndef _ROTARY_ENCODER_H_
#define _ROTARY_ENCODER_H_

#include "utils.h"

#define DEFAULT_LOWER_LIMIT 0
#define DEFAULT_UPPER_LIMIT 100

class RotaryEncoder {
  public:
    RotaryEncoder() = delete;
    RotaryEncoder(byte clockPin, byte dataPin);
    RotaryEncoder(byte buttonPin, byte clockPin, byte dataPin);

    void rotaryEncoderCallback(void);
    void setPosition(int16_t position);
    int16_t getPosition(void) const;
    bool buttonPressed(void) const;
    void changeRange(int16_t lowerLimit = DEFAULT_LOWER_LIMIT, int16_t upperLimit = DEFAULT_UPPER_LIMIT);
    
    virtual ~RotaryEncoder() = default;
  private:
    bool m_clockwiseRotationStatus, m_counterclockwiseRotationStatus, m_buttonUsage;
    byte m_buttonPin, m_clockPin, m_dataPin;
    int16_t m_lowerLimit, m_upperLimit, m_position;
};

#endif // _ROTARY_ENCODER_H_
