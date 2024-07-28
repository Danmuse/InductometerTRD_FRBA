#ifndef _SIGNAL_GENERATOR_H_
#define _SIGNAL_GENERATOR_H_

#include "comm.h"
#include "AD9833.h"

extern SPISettings AD9833_SPISettings;
extern AD9833 g_signalGenerator;

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
