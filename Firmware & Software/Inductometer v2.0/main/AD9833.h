#ifndef _AD9833_H_
#define _AD9833_H_

#include "utils.h"

#define FSYNC 4

#define WAVE_SINE     0x2000
#define WAVE_SQUARE   0x2028
#define WAVE_TRIANGLE 0x2002

void AD9833setup(void);
void AD9833reset(void);
void AD9833setFrequency(long frequency, int Waveform);
void WriteRegister(int dat);
// void actualiza_func(void);
// void limpiaDigitos(void);

#endif // _AD9833_H_
