// #include <U8g2lib.h>
// U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Descomentar según el sensor usado ya sea de 5, 20 0 30 Amperes
float sensibilidad = 0.185; //Ajuste de la sensibilidad para sensor de 5 Amperes
// float sensibilidad = 0.100; //Ajuste de la sensibilidad para sensor de 20 Amperes
// float sensibilidad = 0.66; //Ajuste de la sensibilidad para sensor de 30 Amperes
float I = 0.00;
float ajuste = 0.05; // Solo si es necesario, se usa para corregir posibles desvios en la lectura que se comparará con un amperímetro

// Función para generar 500 muestas
float promedio_I(int muestras_I) {
  float sensorA0;
  float intencidad = 0;
  for (int i = 0; i < muestras_I; i++) {
    sensorA0 = analogRead(A0) * (5.0 / 1023.0); // Leemos el sensor de corriente
    intencidad = intencidad + (sensorA0 - 2.5) / sensibilidad; // Calculo para obtener el valor de la corriente
  }
  intencidad = intencidad / muestras_I; //dividimos por 500
  return (intencidad);
}

void setup() {
  Serial.begin(115200); // Velocidad del puerto serial
  // u8g2.begin(); // Inicializamos el display
}

void loop() {
  I = promedio_I(500); // Promedio de 500 muestras para mejorar la presición (llamamos a la función promedio_I()
  Serial.print("Intencidad: ");
  if (I >= 0.01) { // Filtro para eliminar pequeñas oscilasciones(ruido)
    I = ((I * 10) + ajuste), 2;
    Serial.println(I);//Imprime el valor de la corriente consumida
    // oled();//Función para mostrar los datos obtenidos en el display
    delay(100);
  } else {
    Serial.println("0.00");
    I = 0.00;
    // oled();//Función para mostrar en el display 0.00
    delay(100);
  }
}

/*
//Función para mostrar datos en el display oled
void oled() {
  u8g2.clearBuffer(); // Borra el contenido anterior del buffer de pantalla
  u8g2.setFont(u8g2_font_ncenB14_tr); // Selecciona una fuente grande
  u8g2.setCursor(0, 40); // Establece la posición de la posición del texto
  u8g2.print("I:");//Se muestra I: en el display
  u8g2.print(I);//Se muestra el valor de corriente obtenido
  u8g2.setCursor(60, 40); // Establece la posición de la posición del texto
  u8g2.print("A");//Se muestra A en el display
  u8g2.sendBuffer(); // Envia el contenido del buffer a la pantalla
}
*/
