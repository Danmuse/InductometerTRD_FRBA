#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#define outImpulso 2
#define inImpulso  3
unsigned long t = 0;
unsigned long iniPeriodo = 0;
unsigned long transcurrido = 0;
float Lx  = 0;
float fo  = 0;
float cap = 0.000000951;   //Valor del capacitor en paralelo a Lx (Ejemplo 883nF)
float pi  = 3.14159;

float inductancia(void){
   digitalWrite(outImpulso, HIGH);
   delay(1);
   digitalWrite(outImpulso, LOW);
   t = pulseIn(inImpulso, LOW);
   fo = 1000000 / (2 * t);
   Lx = (1 / (4 * pow(pi,2) * pow(fo,2) * cap)) * 1000000;
   return Lx;
}

void setup(){
   Serial.begin(9600);
   lcd.begin(16, 2); lcd.setBacklight(HIGH);
   pinMode(inImpulso, INPUT_PULLUP);
   pinMode(outImpulso, OUTPUT);
}

void loop(){
   if(inductancia() < 10){
      lcd.setCursor(7, 0);
      lcd.print("      ");
   }
   else{
      if(inductancia() < 100){
         lcd.setCursor(8, 0);
         lcd.print("      ");
      }
   }
   Serial.println(inductancia(),0);
   lcd.setCursor(0, 0);
   lcd.print("Ind.: ");
   lcd.setCursor(6, 0);
   lcd.print(inductancia(),0);
   lcd.setCursor(14, 0);
   lcd.print("uH");
}
