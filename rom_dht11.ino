#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin (9600);
  Serial.println("Carrregando o sistema...");
  delay (1000);
  Serial.println("Inicializando o sensor de temperatura e umidade");
  dht.begin();

}
void loop()
{
  // A leitura da temperatura e umidade pode levar 250ms!
  float umidade = dht.readHumidity();
  float celsius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);
  bool dhtReader = true;
  if (isnan(umidade) || isnan(celsius) || isnan (fahrenheit))
  {
    Serial.println("O sensor DHT não está funcionando!");

    Serial.println (":(");
   
    if (dhtReader)
    {
      Serial. println("Umidade: ");
      Serial. print(umidade);
      delay(1000);
      Serial.print(" %\n");
      Serial.println("Temperatura: ");
      Serial.print(celsius);
      Serial.println(" *C ");
      delay(1000);
      Serial.print(fahrenheit);
      Serial.print(" *F ");
      delay(1000);
    }
   dhtReader = false;
  }

  delay (5000);
}
