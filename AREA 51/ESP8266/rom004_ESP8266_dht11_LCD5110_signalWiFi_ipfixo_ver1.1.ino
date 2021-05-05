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
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>

// biblioteca com funções para o WiFi do ESP6288
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// biblioteca com funções para o sensor DHT11
#include <DHT.h>



Adafruit_PCD8544 display = Adafruit_PCD8544(D8, D7, D6, D5, D2);
/* Adafruit_PCD8544 nome = Adafruit_PCD8544(CLK, DIN, DC, CE, RST)
*/
/*
   Não é reomendado a alteração do tipo de variaveis acima sob pena de erro compilando para a placa NodeMCU em função da biblioteca adafruit_PCD8544,
   então siga  o esquema de ligação abaixo para o LCD Nokia 5110 (cor vermelha) no ESP8266:
   Pino 1 - RST → D2 (GPIO4) - Saída do ESP para redefinir a exibição
   Pino 2 - CE → D5 (GPIO11) - Saída do ESP para selecionar chip / ativar exibição
   Pino 3 - DC → D6 (GPIO12) (MISO) - Saída dos dados de exibição / comando para ESP
   Pino 4 - DIN → D7 (GPIO13) (MOSI) - DIN do ESP SPI MOSI para exibir entrada de dados
   Pino 5 - CLK → D8(GPIO14) - saídas CLK do relógio ESP SPI
   Pino 6 - VCC → 3.3V - Alimentação do LCD para ligar o LCD
   Pino 7 - LIGHT ou BACKLIGHT → GND
   Pino 8 - GROUND → GND
*/


const unsigned char logo_arduino [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
  0xC0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0xFC, 0xFE, 0xFF, 0x3F, 0x1F, 0x0F, 0x07, 0x07, 0x07, 0x03,
  0x03, 0x03, 0x07, 0x07, 0x07, 0x0F, 0x0F, 0x1F, 0x3F, 0x7E, 0xFE, 0xFC, 0xF8, 0xF0, 0xC0, 0xE0,
  0xF8, 0xFC, 0xFC, 0x7E, 0x3F, 0x1F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x83, 0x83, 0x03, 0x07, 0x07,
  0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFE, 0xFC, 0xF8, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x80, 0x00, 0x00,
  0x00, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF3,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0xE1, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x7F,
  0x7F, 0x1E, 0x0E, 0x0E, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F,
  0x1F, 0x3F, 0x3E, 0x7E, 0x7C, 0x7C, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0x78, 0x7C, 0x7C, 0x7E, 0x3F,
  0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x1F, 0x3F, 0x3E, 0x7C,
  0x7C, 0x78, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0x78, 0x7C, 0x7C, 0x3E, 0x3F, 0x1F, 0x0F, 0x07, 0x03,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xE0, 0x78, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0x98, 0xF8, 0xF0, 0x60, 0x00,
  0xF8, 0xF8, 0x18, 0x18, 0xF0, 0xF0, 0x00, 0x00, 0xF8, 0xF0, 0x00, 0x00, 0xF8, 0xF0, 0x00, 0x18,
  0x18, 0xF8, 0xF8, 0x18, 0x18, 0x00, 0xF0, 0xF8, 0x78, 0xE0, 0x80, 0xF8, 0xF8, 0x00, 0xE0, 0xF0,
  0x18, 0x18, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x01, 0x01, 0x07, 0x07, 0x00, 0x00, 0x07, 0x03, 0x01,
  0x07, 0x07, 0x04, 0x00, 0x07, 0x07, 0x06, 0x06, 0x07, 0x03, 0x00, 0x00, 0x03, 0x07, 0x06, 0x06,
  0x07, 0x01, 0x00, 0x06, 0x06, 0x07, 0x07, 0x06, 0x06, 0x00, 0x03, 0x07, 0x00, 0x00, 0x03, 0x07,
  0x07, 0x00, 0x03, 0x07, 0x06, 0x06, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

long rssi;

int8_t graph[83];

uint8_t i, col, pos = 0;

bool scroll = false;

// Pinagem do sensor DHT11 no ESP8266
#define DHTPIN D3

// Indicação do modelo de sensor, no caso DHT 11
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
// incluir o pino aqui e tipo de sensor

// Indicação da credencial de acesso a rede WiFi
const char* ssid     = ")o,0(";
const char* password = "riko1234";

unsigned long tempo;

void setup()
{
  //Parâmetros da conexão por IP fixo. Você pode alterar conforme a sua rede.
  IPAddress ip(192, 168, 0, 155);
  IPAddress gateway(192, 168, 0, 155);
  IPAddress subnet(255, 255, 255, 0);

  // Inicialização do Monitor LCD
  display.begin();
  display.display();
  display.setContrast(60);  // Adjust for your display
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.println("O monitor foi inicializado!");

  display.setTextColor(BLACK);
  display.print("Connecting to IP fixo:");
  display.print(ip);
  display.display();
  delay(2000);
  display.print("Connecting of rede WiFi:");
  display.println(ssid);
  display.display();
  delay(2000);
  /* Inicialização do monitor serial
    Serial.begin(115200);
    Serial.println("O monitor serial foi inicializado!");
    Serial.println();
    Serial.println();
    delay(2000);
    Serial.print("Connecting to IP fixo:");
    Serial.println(ip);
    delay(2000);
    Serial.print("Connecting of rede WiFi:");
    Serial.println(ssid);
    delay(2000);
  */
  // Inicialização da placa WiFi
  WiFi.config(ip, gateway, subnet);
  delay (2000);
  // Autenticação na rede WiFi para monitor serial"."
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    /*Serial.print(".");
      delay(500);
    */
    display.setTextColor(BLACK);
    display.setCursor(0, 10);
    display.print("Conectando ...");
    delay(500);
    display.print(".");
  }
  /*Serial.println("");
    Serial.println("Esp8266 connected to WiFi");

  */
  display.setTextColor (WHITE);
  display.setCursor ( 0 , 10 );
  display.println ( " Conectando ... " );
  display.setTextColor (BLACK);
  display.setCursor ( 0 , 10 );
  display.println("Esp8266 connected to WiFi");
  display.display();
  delay(2000);

  dht.begin();


}

void dht11()
{
  //Definição de variavéis para uso pelo DHT11

  //Teste de funcionalidade do DHT11
  /*
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

  //Exibição dos dados colhidos pelo DHT11
  String celsius = " TEMPERATURA: " ;
  celsius += String (dht.readTemperature());
  celsius += char ( 247 );
  celsius += " C " ;
  display. setTextSize ( 1 );
  display. setCursor ( 0 , 10 );
  display. println (celsius);
  display. setTextSize ( 1 );

  String umidade = " UMIDADE: " ;
  umidade += float (dht.readHumidity());
  umidade += " % " ;
  display. setCursor ( 0 , 20 );
  display. println (umidade);

  /*
     String fahrenheit = " TEMPERATURA: " ;
    celsius + = String (dht. readTemperature(true));
    celsius + = char ( 247 );
    celsius + = " F " ;
    display. setCursor ( 0 , 30 );
    display. println (fahrenheit);
  */

  delay(5000);
}

void grafico()
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



  delay(100);  // Adjust this to change graph speed
}

void loop()
{
  display.clearDisplay();
  for (int i = 0; i < 25; i++)
  {
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 0);
    display.print("Aguarde alguns instantes...");
    display.setTextColor(BLACK);
    delay(100);
  }
  dht11();
  if ( millis()  -  tempo  > 5000)
  {
    display.clearDisplay();
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 0);
    display.print("Aguarde alguns instantes...");
    display.setTextColor(BLACK);


    //Resetar o tempo
    tempo = millis();
  }
}

/*
   https://www.youtube.com/watch?v=9Ew3R8Qgv5w
   Acessado em 21/04/2019
*/
