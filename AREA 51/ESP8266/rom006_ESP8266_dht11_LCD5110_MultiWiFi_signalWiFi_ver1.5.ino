/**
   O nodeMCU emprega display Nokia 5110 através da biblioteca SPI para comunicação,
   além de versão modificada da biblioteca PCD8544 e da GFX, ambas da Adafruit.

   Copyright (c) 2019  Higo Melo

   O SOFTWARE É FORNECIDO "COMO ESTÁ", SEM GARANTIA DE QUALQUER TIPO, EXPRESSA OU
   IMPLÍCITA, INCLUINDO, MAS NÃO SE LIMITANDO ÀS GARANTIAS DE COMERCIALIZAÇÃO,
   ADEQUAÇÃO A UM DETERMINADO FIM E NÃO VIOLAÇÃO. EM NENHUMA CIRCUNSTÂNCIA
   OS AUTORES OU OS TITULARES DOS DIREITOS AUTORAIS SERÃO RESPONSABILIZADOS POR
   QUAISQUER REIVINDICAÇÕES, DANOS OU OUTRAS RESPONSABILIDADE, SEJA EM AÇÃO DE
   CONTRATO, DELITO OU OUTRO, DECORRENTE DE, FORA OU EM CONEXÃO COM O SOFTWARE
   OU O USO OU OUTRAS NEGOCIAÇÕES NO SOFTWARE.

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
  B00000000, B11111000, B11111111, B00111111, B10000111, B10000000,
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

//Cria uma instancia da classe ESP8266WiFiMulti para conectar as redes WiFi
ESP8266WiFiMulti wifiMulti;

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
long rssi;
int8_t graph[83];
uint8_t i, col, pos = 0;
bool scroll = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("O monitor serial foi inicializado!");
  Serial.println("\r\n");

  // Inicialização do Monitor LCD
  display.begin();
  display.setContrast(60);  // Adjust for your display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Carregando as diretivas do  sistema!");
  display.display();
  delay(3000);
  display.clearDisplay();

  // Inicialização da placa WiFi
  //Inicia a comunicacao com os hothospts configurados
  startWiFi();

  dht.begin();

}

void loop()
{
  display.clearDisplay();

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
  for (int i = 0; i < 1; i++)
  {
    /*
      Teste de funcionalidade do DHT11
      if (isnan(umidade&) || isnan(celsius&) || isnan(fahrenheit))
      {
      display.clearDisplay();
      display.println(F("Failed to read from DHT sensor!"));
      // Serial.println(F("Failed to read from DHT sensor!"));
      return;
      display.display();
      delay(5000);
      }
    */
    display.setTextColor(BLACK);
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
    String umidade = " UMIDADE:      " ;
    umidade += String (dht.readHumidity());
    umidade += " % " ;
    display. println (umidade);
    display.display();
    delay(14000);
    /* Exibição dos dados colhidos pelo DHT11
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
    */

  }


  for (int i = 0; i < 90; i++)
  {
    /*
      Graph your WiFi signal strength RSSI (dBm) on a Nokia 5110 PCD8544 LCD module.
      Signal strength (RSSI) is the usable strength of the radio waves, expressed in decibels (db) between 0db (strongest) and -120db (weakest). The closer it is to zero, the stronger/cleaner the signal is. For WiFi, the normal range is -45db to -87db. Anything below -85db is generally unusable, and over -50db can be considered perfect.
      RSSI
      Quality > -50db
      Fantastic ~= -50db
      Great ~= -75db
      Average ~= -85db
      Poor ~= -95db
      Unusable
    */
    display.setTextColor(BLACK);
    rssi = WiFi.RSSI();  // eg. -63
    // Convert to scale -48 to 0 eg. map(rssi, -100, 0, 0, -48);
    // I used -100 instead of -120 because <= -95 is unusable
    // Negative number so we can draw n pixels from the bottom in black

    graph[pos] = (1.0 - (rssi / -100.0)) * -48.0;
    // Draw the RSSI eg. -63db
    display.clearDisplay();
    display.printf("%ddb\n", rssi);
    // Draw the graph left to right until 84 columns visible
    // After that shuffle the graph to the left and update the right most column
    if (!scroll) {
      for (i = 0; i <= pos; i++) {
        display.drawFastVLine(i, LCDHEIGHT - 1, graph[i], BLACK);
      }
    }
    else {
      for (i = 0; i < LCDWIDTH; i++) {
        col = (i + pos + 1) % LCDWIDTH;
        display.drawFastVLine(i, LCDHEIGHT - 1, graph[col], BLACK);
      }
    }
    display.display();
    // After the first pass, scroll the graph to the left
    if (pos == 83) {
      pos = 0;
      scroll = true;
    }
    else {
      pos++;
    }
    delay(200);  // Adjust this to change graph speed
  }
  delay(1000);
}
void startWiFi()
{
  Serial.print("Connecting...");
  display.print("Connecting...");
  delay(1000);
  //Coloque aqui as redes wifi necessarias
  wifiMulti.addAP(")o,0(", "riko1234");
  wifiMulti.addAP("NOME_DA_REDE_WIFI_2", "SENHA_DA_REDE_WIFI_2");
  wifiMulti.addAP("NOME_DA_REDE_WIFI_3", "SENHA_DA_REDE_WIFI_3");

  while (wifiMulti.run() != WL_CONNECTED)
  {
    //Aguarda a conexao da rede wifi
    delay(250);
    Serial.print('.');

    display.setTextColor(BLACK);
    display.clearDisplay();
    display.setCursor(0, 0);
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

  display.setTextColor(BLACK);
  display.println("Connected of the rede:\t");
  display.print(WiFi.SSID());
  display.println("IP adress:\t");
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
