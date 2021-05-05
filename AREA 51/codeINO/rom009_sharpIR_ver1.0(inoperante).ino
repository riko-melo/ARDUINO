#include <SharpIR.h>


SharpIR sharp(1, A3);
#define pinLED 5
#define pinPIR 6


void setup()
{
  Serial.begin(9600);
  pinMode(pinPIR, INPUT);
  pinMode(pinMelodia, OUTPUT);
  pinMode(pinLED, OUTPUT);




}
void loop()
{
  int distancia = sharp.getDistance();
  Serial.print("VOLUME: ");
  Serial.print(distancia);
  Serial.println(" %");
  delay (1000);
  if (distancia <= 40) {
    Serial.println("Alerta: A caixa d'água está quase vazia!");
    int n = 0;
    while (n < 9) {
      digitalWrite(pinLED, !digitalRead(pinLED));
      delay(1000);
      n ++;
    }
    Serial.println(":(");
  }
}
