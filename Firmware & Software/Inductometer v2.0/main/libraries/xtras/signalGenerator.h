#ifndef _SIGNAL_GENERATOR_H_
#define _SIGNAL_GENERATOR_H_

#include "comm.h"
#include "AD9833.h"

extern SPISettings AD9833_SPISettings;
extern AD9833 g_signalGenerator;

/*
#define WAVE_SINE     0x2000
#define WAVE_SQUARE   0x2028
#define WAVE_TRIANGLE 0x2002
*/

typedef enum {
  SIG_SINE = 0,
  SIG_SQUARE = 1,
  SIG_TRIANGLE = 2
} signal_type_t;

// void AD9833setup(void);
// void AD9833reset(void);
// void AD9833setFrequency(long frequency, int Waveform);
// void WriteRegister(int dat);
// void actualiza_func(void);
// void limpiaDigitos(void);

#endif // _SIGNAL_GENERATOR_H_
