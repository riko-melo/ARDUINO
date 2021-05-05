#include <VirtualWire.h>

#define pinPot A0
#define pinRF  12
#define pinLED 13

struct tipoPacote {
  int valor1;
  byte valor2;
  int valor3;
};

tipoPacote pacote; 

void setup() {

  vw_set_tx_pin(pinRF);
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  pinMode(pinLED, OUTPUT);

  //Serial.begin(9600);
}

void loop() {
  digitalWrite(pinLED, HIGH); 
  
  int  leitura1 = analogRead(pinPot);
  byte leitura2 = map(leitura1, 0, 1023, 0, 255);
  int  leitura3 = map(leitura1, 0, 1023, 120, 0);

  pacote.valor1 = leitura1;
  pacote.valor2 = leitura2;
  pacote.valor3 = leitura3;

  /*
  Serial.print(pacote.valor1);
  Serial.print(" | ");
  Serial.print(pacote.valor2);
  Serial.print(" | ");
  Serial.println(pacote.valor3);
  */
  
  vw_send((uint8_t *)&pacote, sizeof(pacote));
  vw_wait_tx(); 
  
  digitalWrite(pinLED, LOW);
  delay(200);
}
