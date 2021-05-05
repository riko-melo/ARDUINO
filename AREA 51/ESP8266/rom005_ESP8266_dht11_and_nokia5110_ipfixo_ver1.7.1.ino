/**
   COPYRIGHT (c) 2019  HIGO MELO

   O SOFTWARE É FORNECIDO GRATUITAMENTE, SEM GARANTIA DE QUALQUER TIPO, EXPRESSA OU
   IMPLÍCITA, DE GARANTIA, RESSALVADAS ÀS GARANTIAS DE COMERCIALIZAÇÃO E NÃO VIOLAÇÃO
   DOS DIREIOTS AUTORAIS. EM NENHUMA CIRCUNSTÂNCIA, O TITULAR DOS DIREITOS AUTORAIS PODE
   SER RESPONSABILIZADO POR QUAISQUER DANOS OU UTILIZAÇÃO INDEVIDA, SEJA EM AÇÃO DE
   CONTRATO, DELITO OU OUTRO, DECORRENTE DE, FORA OU EM CONEXÃO COM O SOFTWARE.

   Este sketch é destinado ao nodeMCU v1.0 com display LCD monocromático (sem tons de
   cinza) conhecido como "Nokia 5110" com controlador    Adafruit_PCD8544, utilizando
   ainda    protocolo SPI para comunicação, através de 5 pinos, porém dá para eliminar
   um e usar apenas os 4 pinos da SPI.

   A resolução é de 84×48 pixels.
 **/

/*
   A biblioteca Adafruit_GFX deve antecede a biblioteca Adafruit_PCD8544 sob pena de erro na compilação do programa.
   Inspirado no modelo disponível em https://github.com/michielvanhoorn/thingspeak-nodemcu-with-nokia-5110-LCD/blob/master/thingspeak.ino
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


//Cria uma instancia da classe Adafruit_PCD8544 para enviar dados ao LCD
Adafruit_PCD8544 display = Adafruit_PCD8544(D8, D7, D6, D5, D2);
/*                                       (CLK, DIN, DC, CE, RST)
*/
/*
   Não é reomendado a alteração do tipo de variaveis acima sob pena de erro compilando para a placa NodeMCU em função da biblioteca adafruit_PCD8544,
   então siga  o esquema de ligação abaixo para o LCD Nokia 5110 (cor vermelha) no ESP8266:
   PINAGEM       LCD         ESP-12E
   Pino 1 -----> RST ----->   D2 (GPIO4) - Saída do ESP para redefinir a exibição
   Pino 2 -----> CE  ----->   D5 (GPIO11) - Saída do ESP para selecionar chip / ativar exibição
   Pino 3 -----> DC  ----->   D6 (GPIO12) (MISO) - Saída dos dados de exibição / comando para ESP
   Pino 4 -----> DIN ----->   D7 (GPIO13) (MOSI) - DIN do ESP SPI MOSI para exibir entrada de dados
   Pino 5 -----> CLK ----->   D8(GPIO14) - saídas CLK do relógio ESP SPI
   Pino 6 -----> VCC ----->   3.3V - Alimentação do LCD para ligar o LCD
   Pino 7 -----> LIGHT ----->  GND
   Pino 8 -----> GND  ----->  GND
*/
static const unsigned char PROGMEM Logo[] =
{ B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B11111000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B11111111, B00000000, B00000000,
  B00000000, B00000011, B00000000, B11111111, B11000000, B00000000,
  B00000000, B00001110, B00000000, B00111111, B11110000, B00000000,
  B00000000, B00111110, B00000000, B10000000, B01111100, B00000000,
  B00000000, B01111100, B00000000, B11000000, B00111110, B00000000,
  B00000000, B11111100, B00000000, B00000000, B00001111, B00000000,
  B00000001, B11111000, B11111111, B00111111, B10000111, B10000000,
  B00000011, B11111000, B11111111, B11111111, B11000011, B11000000,
  B00000111, B11110001, B11111111, B11111111, B11100000, B11100000,
  B00000111, B11100001, B11111111, B11100011, B11111000, B01100000,
  B00000000, B00000011, B11100000, B00001001, B11111100, B00000000,
  B00000000, B00000111, B11100000, B00011001, B11111110, B00000000,
  B00000000, B00000111, B11000000, B00000001, B11111111, B10000000,
  B00011111, B11111111, B11000111, B11100011, B11111111, B11111000,
  B00111111, B11111111, B10001111, B11111111, B11111111, B11111100,
  B00111111, B11111111, B00011111, B11111111, B11111111, B11111100,
  B00111111, B11111111, B00011111, B11111111, B11111111, B11111100,
  B00111111, B11111110, B00111111, B00111111, B11111111, B11111110,
  B01111111, B11111110, B00111110, B00000000, B01111111, B11111100,
  B01111111, B11111100, B01111100, B11000000, B00000000, B00000000,
  B00000000, B11111100, B01111110, B10000000, B00000000, B00000000,
  B00000000, B11111111, B11111110, B00111111, B11111111, B11111110,
  B00000000, B00000001, B11111111, B11111111, B11111111, B11111110,
  B01111111, B11111111, B11111111, B11111111, B11111111, B11111110,
  B01111111, B11111111, B11111111, B11111111, B11111111, B11111110,
  B01111111, B11111111, B11000111, B11111111, B11111111, B11111110,
  B00111111, B11111111, B10000011, B11111110, B00000000, B00000000,
  B00111111, B11111111, B10110011, B11111000, B00000000, B00000000,
  B00111111, B11111111, B10000001, B11100000, B00000000, B00000000,
  B00111111, B11111111, B11000000, B10000001, B11111111, B11111100,
  B00000000, B00011111, B11111000, B00000111, B11111111, B11111000,
  B00000000, B00000111, B11111110, B00011111, B11111111, B11111000,
  B00000000, B00000001, B11111111, B01111111, B11111111, B11110000,
  B00001111, B11100000, B11111111, B11111111, B11111111, B11110000,
  B00000111, B11111000, B00001111, B11111111, B11000000, B00000000,
  B00000011, B11111100, B00100111, B11111111, B00000000, B00000000,
  B00000011, B11111111, B00110111, B11111100, B00000000, B00000000,
  B00000001, B11111111, B10000111, B11011000, B00111111, B10000000,
  B00000000, B11111111, B11001111, B10000000, B11111111, B00000000,
  B00000000, B01111111, B11111111, B10110001, B11111110, B00000000,
  B00000000, B00011111, B11111111, B10110111, B11111100, B00000000,
  B00000000, B00001111, B11111111, B10000111, B11110000, B00000000,
  B00000000, B00000011, B11111111, B11111111, B11000000, B00000000,
  B00000000, B00000000, B11111111, B11111111, B00000000, B00000000,
  B00000000, B00000000, B00001111, B11110000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
};


// biblioteca com funções para o WiFi do ESP6288
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <NTPClient.h>
#include <WiFiUdp.h>



//Cria uma instancia da classe ESP8266WiFiMulti para conectar as redes WiFi
ESP8266WiFiMulti wifiMulti;

WiFiUDP ntpUDP;
int16_t utc = -3; //UTC -3:00 Brazil
uint32_t horaAtual = 0;
uint32_t horaCerta = 0;

NTPClient timeClient(ntpUDP, "a.st1.ntp.br", utc * 3600, 60000);

// biblioteca com funções para o sensor DHT11
#include <DHT.h>
// Pinagem do sensor DHT11 no ESP8266
#define DHTPIN D3

// Indicação do modelo de sensor, no caso DHT 11
#define DHTTYPE DHT11

//Cria uma instancia da classe WifiUDP para enviar e receber dados
DHT dht(DHTPIN, DHTTYPE);
// incluir o pino aqui e tipo de sensor



unsigned long tempo;
String umidade, celsius, fahrenheit;


void setup()
{
  // Inicialização do Monitor serial
  Serial.begin(115200);
  Serial.println("O monitor serial foi inicializado!");
  Serial.println("\r\n");

  // Inicialização do LCD
  display.begin();
  delay(1000);
  display.setContrast(60);  // Adjust for your display
  display.clearDisplay();
  display.setTextSize(1);
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
  display.drawRect(0, 0, 84, 48, BLACK); //Desenha o retangulo da borda
  display.drawRect(3, 3, 78, 42, BLACK); //Desenha o retangulo da borda
  display.drawRect(5, 5, 74, 38, BLACK); //Desenha o retangulo da borda
  display.drawRect(7, 7, 70, 34, BLACK); //Desenha o retangulo da borda
  display.drawRect(9, 9, 64, 30, BLACK); //Desenha o retangulo da borda
  display.drawRect(11, 11, 60, 26, BLACK); //Desenha o retangulo da borda
  display.display();
  delay(3000);
  display.clearDisplay();

  //Inicialização da conexão WiFi
  startWiFi();

  timeClient.begin();
  timeClient.update();

  // Inicialização do sensor DHT11
  dht.begin();

}

void atualizar(void) {
  timeClient.forceUpdate();
  horaCerta = millis();//Tempo atual em ms
  //Lógica de verificação do tempo
  if (horaCerta - horaAtual > 500)
  {
    horaAtual = horaCerta; // Salva o tempo atual
    Serial.println(timeClient.getFormattedTime());
    display.setTextSize(2);
    display.setCursor(0, 0);
    for (int i = 0; i > 15; i++)
    {
      String horario = (timeClient.getFormattedTime());
      display.println(horario);
      display.display();
      delay(50);
      display.clearDisplay();
    }

  }
}


void dht11()
{
  for (int i = 0; i < 1; i++)
  {

    //Teste de funcionalidade do DHT11
    if (isnan(umidade && celsius && fahrenheit))
    {

      display.println(F("Failed to read from DHT sensor!"));
      // Serial.println(F("Failed to read from DHT sensor!"));
      return;
      display.display();
      delay(5000);
      display.clearDisplay();
      ESP.reset();
    }

    display.setTextSize(1);
    display.setCursor(0, 0);

    //Exibição dos dados colhidos pelo DHT11
    String celsius = " TEMPERATURA: " ;
    celsius += String (dht.readTemperature());
    celsius += char (247);
    celsius += " C " ;
    display.println (celsius);
    String fahrenheit = String (dht.readTemperature(true)) ;
    fahrenheit += char (247);
    fahrenheit += " F " ;
    display.println (fahrenheit);

    display. println();
    String umidade = " UMIDADE:     " ;
    umidade += String (dht.readHumidity());
    umidade += " % " ;
    display. println (umidade);
    display.display();
    delay(15000);

    //Exibição de dados do sensor DHT11 no monitor serial
    Serial.print(F("UMIDADE: "));
    Serial.print(umidade);
    Serial.println(F("%"));
    Serial.println ("");
    Serial.print ("TEMPERATURA: ");
    Serial.print(celsius);
    Serial.println(F("°C "));
    Serial.println ("");
    Serial.print ("TEMPERATURA: ");
    Serial.print(fahrenheit);
    Serial.println(F("°F "));
    delay(5000);
    Serial.println ("");


  }
}

void loop()
{
  display.clearDisplay();
  atualizar();
  display.clearDisplay();
  dht11();
  /*Mostra o titulo na parte superior do display
    display.print("NTP Clock ESP8266");
    String horario = ("HORA ATUAL: ");
    horario += String(getHours(actualTime));
    horario += " : ";
    horario += String(getMinutes(actualTime));
    horario += " : ";
    horario += String(getSeconds(actualTime));
    display.println (horario);
    delay(3000);
    display.display();
    display.clearDisplay();
  */
}

void startWiFi()
{
  Serial.print("Connecting...");

  display.print("Connecting...");
  display.display();
  delay(2000);
  display.clearDisplay();

  //Coloque aqui as redes wifi necessarias
  wifiMulti.addAP(")o,0(", "riko1234");
  wifiMulti.addAP("NOME_DA_REDE_WIFI_2", "SENHA_DA_REDE_WIFI_2");
  wifiMulti.addAP("NOME_DA_REDE_WIFI_3", "SENHA_DA_REDE_WIFI_3");

  while (wifiMulti.run() != WL_CONNECTED)
  {
    //Aguarda a conexao da rede wifi
    delay(250);
    Serial.print('.');


    display.clearDisplay();
    display.setCursor(0, 2);
    display.println(".");
    display.display();
    delay(500);
  }
  Serial.println("\r\n");
  Serial.print("Connected of the rede:\t ");
  Serial.println(WiFi.SSID());
  Serial.print("IP adress:\t");
  Serial.print(WiFi.localIP());
  Serial.println("\r\n");


  display.println("Connected of  the rede:     ");
  display.print(WiFi.SSID());
  display.display();
  delay(3000);
  display.clearDisplay();


  display.println("IP address:    ");
  display.print(WiFi.localIP());
  display.display();
  delay(3000);
  display.clearDisplay();
}


/*
   https://www.arduinoecia.com.br/2018/04/relogio-nodemcu-esp8266-display-oled-ntp.html
   https://www.youtube.com/watch?v=9Ew3R8Qgv5w
   Acessado em 21/04/2019
*/
