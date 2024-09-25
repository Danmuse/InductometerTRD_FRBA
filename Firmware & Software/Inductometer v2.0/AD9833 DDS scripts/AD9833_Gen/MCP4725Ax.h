/*!
 * @file MCP4725Ax.h
 * @par Author & Doxygen Editor
 *  Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 20/04/2023 13:36:24
 * @version 1.0
 * @brief MCP4725Ax API
 * @details @par Description
 *  The <b>MCP4725Ax</b> library provides a selection of different types of Digital-Analog Converter (<tt>DAC</tt>) that are available in the <tt>MCP4725</tt> Integrated Circuit. @n
 *  Functions in this library allows configure the voltage values of each output.
 * @see <a target = "__blank" href = "http://ww1.microchip.com/downloads/en/DeviceDoc/22039d.pdf"><strong>MCP4725</strong></a> Datasheet.
 */

#ifndef _MCP4725A_H_
#define _MCP4725A_H_

// Includes Genereric Standard Arduino and Inter-IC (<tt>I2C</tt>) communication methods.
#include <Arduino.h>
#include <Wire.h>

//! @hideinitializer Writes data to the Digital-Analog Converter (<tt>DAC</tt>).
#define MCP4725_WRITEDAC (0x40) //!< <pre><strong>Value:</strong> 0x40
//! @hideinitializer Writes data to the Digital-Analog Converter (<tt>DAC</tt>) and the Electrically Erasable Programmable Read-Only Memory (<tt>EEPROM</tt>) to persist the assigned value after reboot.
#define MCP4725_WRITEDAC_EEPROM (0x60) //!< <pre><strong>Value:</strong> 0x60

//! @hideinitializer Defines the speed of Inter-IC (<tt>I2C</tt>) communication for the <a target = "__blank" href = "https://reference.arduino.cc/reference/en/language/functions/communication/wire/"><tt><b><Wire.h></b></tt></a> library. The value is defined in bits per second (<tt>bps</tt>) and determines the maximum data transfer rate.
#define WIRE_SPEED (400000UL) //!< <pre><strong>Value:</strong> 400000

//! @brief <b>statusI2C_t</b> enumeration indicates the status and faults of Inter-IC (<tt>I2C</tt>) communication.
typedef enum {
  I2C_SUCCESS     = 0, //!< Successful operation.
  I2C_BUFFER      = 1, //!< Indicates that the data to be transmitted was too long to fit in the transmit buffer.
  I2C_ADDR_NACK   = 2, //!< Indicates that the slave device did not acknowledge the transmit of the address byte.
  I2C_DATA_NACK   = 3, //!< Indicates that the slave device did not acknowledge the transmit of a data byte.
  I2C_OTHER_ERROR = 4  //!< Other error has occurred.
} statusI2C_t;

//! @brief <b>flag_t</b> enumeration indicates whether a certain states or conditions in the program.
typedef enum {
  FALSE = 0, //!< Indicates that a condition is <tt>false</tt>.
  TRUE  = 1  //!< Indicates that a condition is <tt>true</tt>.
} flag_t;

//! @brief <b>analogStatus_t</b> enumeration indicates the status of the Digital-Analog Converter (<tt>DAC</tt>) output.
typedef enum {
  ANALOG_IDLE = 0, //!< Analog signal is not specified.
  ANALOG_LOW  = 1, //!< Analog signal is not being used.
  ANALOG_HIGH = 2  //!< Analog signal is being used.
} analogStatus_t;

//! @brief <b>analogMCP_t</b> enumeration provides information on the control byte that determines which Digital-Analog Converter (<tt>DAC</tt>) output will be selected.
typedef enum {
  analogMCPA0_agnd = 0b1100000, //!< Analog Output <tt>0</tt> tied to GND of the <tt>MCP4725A0</tt>.
  analogMCPA0_avdd = 0b1100001, //!< Analog Output <tt>1</tt> tied to VCC of the <tt>MCP4725A0</tt>.
  analogMCPA1_agnd = 0b1100010, //!< Analog Output <tt>0</tt> tied to GND of the <tt>MCP4725A1</tt>.
  analogMCPA1_avdd = 0b1100011, //!< Analog Output <tt>1</tt> tied to VCC of the <tt>MCP4725A1</tt>.
  analogMCPA2_agnd = 0b1100100, //!< Analog Output <tt>0</tt> tied to GND of the <tt>MCP4725A2</tt>.
  analogMCPA2_avdd = 0b1100101  //!< Analog Output <tt>1</tt> tied to VCC of the <tt>MCP4725A2</tt>.
} analogMCP_t;

/*!
 * @brief Sets the initial conditions for <tt>MCP4725</tt> Integrated Circuit.
 * @par Parameters
 *   None.
 * @return Nothing.
 */
void initMCP(void);

/*!
 * @brief Selects any preference Digital-Analog Converter (<tt>DAC</tt>) output of the <tt>MCP4725</tt> Integrated Circuit. @n
 *  A specified voltage value is configured from the selected output.
 * @param MCP_address Indicates which analog output will be selected.
 * @param analogValue Number of counts with 12 bits of resolution that represents the voltage value to be configured.
 * @param writeEEPROM Indicates whether to store the voltage value in the Electrically Erasable Programmable Read-Only Memory (<tt>EEPROM</tt>).
 * @return Status of Inter-IC (<tt>I2C</tt>) communication.
 */
statusI2C_t setVoltage(analogMCP_t MCP_address, uint16_t analogValue, flag_t writeEEPROM);

#endif // _MCP4725A_H_
