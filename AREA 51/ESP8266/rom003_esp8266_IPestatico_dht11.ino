// biblioteca com funções para o WiFi do ESP6288
#include <ESP8266WiFi.h>

// biblioteca com funções para o sensor DHT11
#include <DHT.h>


// Utilização da GPIO12 do ESP8266 e D6 do NodeMCU
#define DHTPIN D6

// Definição do tipo de sensor, no caso DHT 11
#define DHTTYPE DHT11

//Definição de objeto DHT
DHT dht(DHTPIN, DHTTYPE);

// Nome da Rede
const char* ssid     = ")o,0(";

// Password da rede
const char* password = "riko1234";

unsigned long tempo;

void setup()
{
  // Inicialização do monitor serial
  Serial.begin(115200);
  Serial.println("O monitor serial foi inicializado!");
  Serial.println();
  Serial.println();
  delay(2000);

  //Parâmetros da conexão por IP fixo. Você pode alterar conforme a sua rede
  IPAddress ip(192, 168, 0, 155);
  IPAddress gateway(192, 168, 0, 155);
  IPAddress subnet(255, 255, 255, 0);

  //Mensagem apresentada no monitor serial
  Serial.print("Connecting to IP fixo:");
  Serial.println(ip);
  delay(2000);

  // Apresenta o nome da rede WiFi no monitor serial
  Serial.print("Connecting of rede WiFi:");
  Serial.println(ssid);
  delay(2000);

  // Inicialização da placa WiFi
  WiFi.config(ip, gateway, subnet);
  conectar();
  delay (2000);


  Serial.println("");

  // Se ocorrer autenticação na rede wireless, esta mensagem será exibida no monitor serial
  Serial.println("Esp8266 connected to WiFi");
  delay(2000);

  dht.begin();

}

void dht11()
{
  //Definição de variavéis para uso pelo DHT11
  float celsius = dht.readTemperature();
  float umidade = dht.readHumidity();// Leitura Humidade
  float fahrenheit = dht.readTemperature(true);

  //Teste de funcionalidade do DHT11
  if (isnan(umidade) || isnan(celsius) || isnan(fahrenheit))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
    delay(5000);
  }

  //Exibição dos dados colhidos pelo DHT11
  Serial.print(F("UMIDADE: "));
  Serial.print(umidade);
  Serial.println(F("%"));
  delay (2000);
  Serial.println ("");
  Serial.print ("TEMPERATURA: ");
  Serial.print(celsius);
  Serial.println(F("°C "));
  delay (2000);
  Serial.println ("");
  Serial.print ("TEMPERATURA: ");
  Serial.print(fahrenheit);
  Serial.println(F("°F "));
  delay(5000);
  Serial.println ("");
}

void loop()
{
  //Faz a verificaçao das funçoes a cada 5s
  if ( millis()  -  tempo  > 5000)
  {
    dht11();
    conectar();
    //Reseta o tempo
    tempo  = millis();
  }
}

void conectar()
{
  // Autenticação na rede WiFi para monitor serial"."
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(1000);
    Serial.print(".");
  }

}
