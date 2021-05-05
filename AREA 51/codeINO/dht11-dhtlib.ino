#include <DHT.h>


dht DHT;

void setup()
{
  Serial.begin (9600);
  Serial.println("Carrregando o sistema...");
  delay (1000);
  Serial.println("Inicializando o sensor de temperatura e umidade");
 
}
void loop()
{
  // A leitura da temperatura e umidade pode levar 250ms!
 DHT.read11(A2); /
 float umidade = dht.readHumidity();
  float celsius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);
  Serial.print(DHT.humidity);
  Serial.print(" %\n");
   delay(1000);
  
  Serial.println("Temperatura: ");
  Serial.print(DHT.temperature);
   Serial.println(" *C ");
  /*
   * 
 bool dhtReader = true;
  if (isnan(umidade) || isnan(celsius) || isnan (fahrenheit))
  {
    Serial.println("O sensor DHT não está funcionando!");

    Serial.println (":(");
    dht12Read = false;
    if (dhtReader)
    {
      Serial. println("Umidade: ");
      Serial. print(umidade);
 
     
     
      
    
    }
  }
*/
}
