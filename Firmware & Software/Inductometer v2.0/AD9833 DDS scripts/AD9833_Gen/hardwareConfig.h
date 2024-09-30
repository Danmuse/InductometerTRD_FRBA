#ifndef _HARDWARE_CONFIG_H_
#define _HARDWARE_CONFIG_H_

#if (__STM32F1__)
// DATA PIN:PA7, CLOCK PIN:PA5, FSYNC PIN:PA2
#define AD9833_FSYNC PA2
#define b_UP    PA8
#define b_DOWN  PB15
#define b_MULTI PB14
#define b_FUNC  PB11
#define b_NEW_CMD  PB13
#define b_END_CMD  PB12
#else
// DATA PIN:11, CLOCK PIN:13, FSYNC PIN:9
#define FSYNC 9
#define b_UP    A0
#define b_DOWN  A1
#define b_MULTI A2
#define b_FUNC  A3
#define b_NEW_CMD  A4
#define b_END_CMD  A5
#endif

#endif // _HARDWARE_CONFIG_H_
