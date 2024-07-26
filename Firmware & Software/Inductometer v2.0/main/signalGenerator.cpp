#include "signalGenerator.h"

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

/*

void actualiza_func(void) {
   if(!digitalRead(b_FUNC)){
      delay(50);
      if(!digitalRead(b_FUNC)){ 
         if(func_ant == 0)
            func = 1;
         if(func_ant == 1)
            func = 2;
         if(func_ant == 2)
            func = 0;
         func_ant = func;
      }
   }
   if(func == 0){
      lcd.setCursor(6, 1);
      lcd.print("  SENOIDAL");
      function = WAVE_SINE;
   }
   if(func == 1){
      lcd.setCursor(6, 1);
      lcd.print("TRIANGULAR");
      function = WAVE_TRIANGLE;
   }
   if(func == 2){
      lcd.setCursor(6, 1);
      lcd.print("  CUADRADA");
      function = WAVE_SQUARE;
   }
   if(counter_ant != counter || function_ant != function){
      AD9833setFrequency(counter, function);
   }
   counter_ant = counter;
   function_ant = function;
}

void limpiaDigitos(void) {
   if(counter < 100){
      lcd.setCursor(2, 0);
      lcd.print("      ");  
   }else{
      if(counter < 1000){
         lcd.setCursor(3, 0);
         lcd.print("     "); 
      }else{
         if(counter < 10000){
            lcd.setCursor(4, 0);
            lcd.print("    "); 
         }else{
            if(counter < 100000){
               lcd.setCursor(5, 0);
               lcd.print("   "); 
            }else{
               if(counter < 1000000){
                  lcd.setCursor(6, 0);
                  lcd.print("  "); 
               }else{
                  if(counter < 10000000){
                     lcd.setCursor(7, 0);
                     lcd.print(" "); 
                  }
               }
            }
         }
      }                
   }
}

*/
