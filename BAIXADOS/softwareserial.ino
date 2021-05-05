#include <SoftwareSerial.h>

SoftwareSerial serial1(10, 11); // RX, TX

String textoRecebido = "";
unsigned long delay1 = 0;

void setup() {

  serial1.begin(9600);
  Serial.begin(9600);

}

void loop() {
  char caracter;
  
  if (Serial.available()) {
     caracter = Serial.read();
     textoRecebido += caracter;
     delay1 = millis();
  }

  if (((millis() - delay1) > 10) && (textoRecebido != "")) {
     serial1.print(textoRecebido);   
     textoRecebido = "";
  }


  if (serial1.available()) {
     caracter = serial1.read();
     Serial.print(caracter);  
  }
  
}
