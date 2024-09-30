#include <LiquidCrystal.h>
// const int rs = 2, en = 3, d4 = 8, d5 = 5, d6 = 6, d7 = 7;
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include "AD9833_DDS.h"
#include "MCP4725Ax.h"

uint32_t counter = 1000;
long int counter_ant = 0;
signal_type_t function;
long int function_ant = 0;
int multi = 1;
int func = 0;
int last_func = 0;

bool newCommand = true;

void setup() {
  Serial.begin(115200);
  g_signalGenerator.begin();
  g_signalGenerator.configDDS(3333, SIG_SINE);
  
  /*
  pinMode(b_UP, INPUT_PULLUP);
  pinMode(b_DOWN, INPUT_PULLUP);
  pinMode(b_MULTI, INPUT_PULLUP);
  pinMode(b_FUNC, INPUT_PULLUP);
  pinMode(b_NEW_CMD, INPUT_PULLUP);
  pinMode(b_END_CMD, INPUT_PULLUP);
  */

  initMCP();
  !(setVoltage(analogMCPA0_agnd, 360, FALSE)) ? Serial.println("OK") : Serial.println("FAIL");
  exit(0);
}

void actualiza_func() {
  if (!digitalRead(b_FUNC)) {
    delay(50);
    if (!digitalRead(b_FUNC)) {
      if (last_func == 0)
        func = 1;
      if (last_func == 1)
        func = 2;
      if (last_func == 2)
        func = 0;
      last_func = func;
    }
  }
  if (func == 0) {
    Serial.println("SINE");
    function = SIG_SINE;
  }
  if (func == 1) {
    Serial.println("TRIANGLE");
    function = SIG_TRIANGLE;
  }
  if (func == 2) {
    Serial.println("SQUARE");
    function = SIG_FST_SQUARE;
  }
  if (counter_ant != counter || last_func != function) {
    g_signalGenerator.configDDS(counter, function);
  }
  counter_ant = counter;
  last_func = function;
}

/*
  void limpiaDigitos(){
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

void loop() {
  if (newCommand) {
    switch (multi) {
      case 0:
        if (!digitalRead(b_MULTI)) {
          delay(50);
          if (!digitalRead(b_MULTI)) {
            multi = 1;
          }
        }
        if (!digitalRead(b_UP)) {
          delay(50);
          if (!digitalRead(b_UP)) {
            counter++;
          }
        }
        if (!digitalRead(b_DOWN)) {
          delay(50);
          if (!digitalRead(b_DOWN)) {
            if (counter > 0)
              counter--;
          }
        }
        /*
          lcd.setCursor(0, 1);
          lcd.print("10^0");
          lcd.setCursor(14, 0);
          lcd.print("Hz");
          lcd.setCursor(0, 0);
          lcd.print(counter);
        */
        // limpiaDigitos();
        Serial.println("Multiplier 10^0");
        Serial.print(counter);
        Serial.println(" Hz\n");
        actualiza_func();
        break;
      case 1:
        if (!digitalRead(b_MULTI)) {
          delay(50);
          if (!digitalRead(b_MULTI)) {
            multi = 2;
          }
        }
        if (!digitalRead(b_UP)) {
          delay(50);
          if (!digitalRead(b_UP)) {
            counter = counter + 1000;
          }
        }
        if (!digitalRead(b_DOWN)) {
          delay(50);
          if (!digitalRead(b_DOWN)) {
            if (counter > 1000)
              counter = counter - 1000;
          }
        }
        /*
          lcd.setCursor(0, 1);
          lcd.print("10^3");
          lcd.setCursor(14, 0);
          lcd.print("Hz");
          lcd.setCursor(0, 0);
          lcd.print(counter);
        */
        // limpiaDigitos();
        Serial.println("Multiplier 10^3");
        Serial.print(counter);
        Serial.println(" Hz\n");
        actualiza_func();
        break;
      case 2:
        if (!digitalRead(b_MULTI)) {
          delay(50);
          if (!digitalRead(b_MULTI)) {
            multi = 0;
          }
        }
        if (!digitalRead(b_UP)) {
          delay(50);
          if (!digitalRead(b_UP)) {
            if (counter < 12500000)
              counter = counter + 1000000;
          }
        }
        if (!digitalRead(b_DOWN)) {
          delay(50);
          if (!digitalRead(b_DOWN)) {
            if (counter > 1000000)
              counter = counter - 1000000;
          }
        }
        /*
          lcd.setCursor(0, 1);
          lcd.print("10^6");
          lcd.setCursor(14, 0);
          lcd.print("Hz");
          lcd.setCursor(0, 0);
          lcd.print(counter);
        */
        // limpiaDigitos();
        Serial.println("Multiplier 10^6");
        Serial.print(counter);
        Serial.println(" Hz\n");
        actualiza_func();
        break;
    }
  }
  if (!digitalRead(b_NEW_CMD)) newCommand = true;
  if (!digitalRead(b_END_CMD)) newCommand = false;
}
