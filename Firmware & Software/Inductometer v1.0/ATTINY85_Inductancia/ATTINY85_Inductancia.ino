#include <Wire.h>
#include <TM1637Display.h>
#define CLK 3   // define CLK pin (any digital pin)
#define DIO 4   // define DIO pin (any digital pin)
#define outImpulso 2  //Patita de la R
#define inImpulso  1  //Salida del OPAM
TM1637Display display(CLK, DIO);

//showNumberDec(numero, ceros precedentes /true o false/, longitud /1 a 4/, posicion /0 a 3/);
//showNumberDecEx(numero, puntos, ceros precedentes /true o false/, longitud /1 a 4/, posicion /0 a 3/);

unsigned long t = 0;
unsigned long iniPeriodo = 0;
unsigned long transcurrido = 0;
float Lx  = 0;
float fo  = 0;
float cap = 0.00000096;   //Valor del capacitor en paralelo a Lx (Ejemplo 883nF)
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
   display.setBrightness(4);
   pinMode(inImpulso, INPUT_PULLUP);
   pinMode(outImpulso, OUTPUT);
   pinMode(0, OUTPUT);
}

void loop(){
   if(inductancia() > 9999){
     int Divide = inductancia()/10;
     digitalWrite(0, HIGH);
     display.showNumberDecEx(Divide, 0b01000000);
     delay(150);
     display.clear();
   }
   else if(inductancia() > 99999){
     int MegaDivide = inductancia()/1000;
     digitalWrite(0, HIGH);
     display.showNumberDecEx(MegaDivide, 0b00000000);
     delay(150);
     display.clear();
   }
   else{
     digitalWrite(0, LOW);
     display.showNumberDecEx(inductancia(), 0b00000000);
     delay(150);
     display.clear();
   }
}
