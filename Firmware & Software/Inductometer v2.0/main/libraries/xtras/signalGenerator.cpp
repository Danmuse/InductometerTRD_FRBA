#include "signalGenerator.h"

SPISettings AD9833_SPISettings(SPI_PERIPHERAL_SPEED, MSBFIRST, SPI_MODE2, DATA_SIZE_8BIT);
AD9833 g_signalGenerator(AD9833_FSYNC); //, AD9833_DATA, AD9833_CLOCK);

/*
void AD9833setup(void) {
   pinMode(FSYNC, OUTPUT);
   digitalWrite(FSYNC, HIGH);
   SPI.begin();
   delay(50); 
   AD9833reset();
}

void AD9833reset(void) {
   WriteRegister(0x100);
   delay(10);
}

void AD9833setFrequency(long frequency, int Waveform) {
   long FreqWord = (frequency * pow(2, 28)) / 25.0E6;
   int MSB = (int)((FreqWord & 0xFFFC000) >> 14);
   int LSB = (int)(FreqWord & 0x3FFF);
   LSB |= 0x4000;
   MSB |= 0x4000; 
   WriteRegister(0x2100);   
   WriteRegister(LSB);
   WriteRegister(MSB);
   WriteRegister(0xC000);
   WriteRegister(Waveform);
}

void WriteRegister(int dat) { 
   SPI.setDataMode(SPI_MODE2);        
   digitalWrite(FSYNC, LOW);
   delayMicroseconds(10); 
   SPI.transfer(dat>>8);
   SPI.transfer(dat&0xFF);
   digitalWrite(FSYNC, HIGH);
   SPI.setDataMode(SPI_MODE0);
}

*/
