/*************************************************************************************
   O SOFTWARE É FORNECIDO GRATUITAMENTE, SEM GARANTIA DE QUALQUER TIPO, EXPRESSA
   OU IMPLÍCITA, DE GARANTIA, RESSALVADAS ÀS GARANTIAS DE COMERCIALIZAÇÃO E NÃO
   VIOLAÇÃO DOS DIREIOTS AUTORAIS. EM NENHUMA CIRCUNSTÂNCIA, O TITULAR DOS DIREITOS
   AUTORAIS PODE SER RESPONSABILIZADO POR QUAISQUER DANOS OU UTILIZAÇÃO INDEVIDA
   DESTE.
  ************************************************************************************
   Embutido na placa, há um display OLED 0,96", 128x64, monocromático. Este possui
   comunicação I2C e está ligado ao ESP8266 Wemos por meio de 3 fios:
  *************************************************************************************
  ***************************DECLARAÇÃO DE BIBLIOTECAS*********************************
  *************************************************************************************/
#include <TimeLib.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <DHT.h>
#include "OLEDDisplayUi.h"
#include "images.h"
#include <brzo_i2c.h>
/**************************************************************************************
 ************************CONFIGURAÇÃO DO DISPLAY OLED 0,96*****************************
 **************************************************************************************/
SSD1306Wire display(0x3c, 5, 4);
OLEDDisplayUi ui( &display);

//PARAMETROS DO SENSOR DHT11
#define pinDHT 10
#define DHTTYPE DHT11
DHT dht(10, DHT11);

int tela_X = 128;
int tela_Y = 64;
int relogio_X = tela_X / 2;
int relogio_Y = ((tela_Y - 16 ) / 2) + 16;
int relogioRadius = 26;
/*
  String doisAlgarismos (int numeral)
  {
  if (numeral < 10)
  {
    String z = "0" + String(numeral);
    return z;
  }
  else
  {
    return String(numeral);
  }
  }

  void relogioAnalog(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y)
  {
  // display->drawCircle(relogio_X + x, relogio_Y + y, relogioRadius);
  display->drawCircle(relogio_X + x, relogio_Y + y, 2);
  for (int w = 0; w < 360; w = w + 30)
  {
    float angulo = w;
    angulo = (angulo / 57.29577951);
    int x2 = (relogio_X + (sin(angulo) * relogioRadius));
    int y2 = (relogio_Y - (cos(angulo) * relogioRadius));
    int x3 = (relogio_X + (sin(angulo) * (relogioRadius - (relogioRadius / 8))));
    int y3 = (relogio_Y - (cos(angulo) * (relogioRadius - (relogioRadius / 8))));
    display->drawLine(x2 + x, y2 + y, x3 + x, y3 + y);
  }
  float angulo = second() * 6;
  angulo = (angulo / 57.29577951);
  int x3 = (relogio_X + (sin(angulo) * (relogioRadius - (relogioRadius / 5))));
  int y3 = (relogio_Y - (cos(angulo) * (relogioRadius - (relogioRadius / 5))));
  display->drawLine(relogio_X + x, relogio_Y + y, x3 + x, y3 + y);

  float angulo = minute() * 6;
  angulo = (angulo / 57.29577951);
  int x3 = (relogio_X + (sin(angulo) * (relogioRadius - (relogioRadius / 4))));
  int y3 = (relogio_Y - (cos(angulo) * (relogioRadius - (relogioRadius / 4))));
  display->drawLine(relogio_X + x, relogio_Y + y, x3 + x, y3 + y);

  float angulo = hour() * 30 + int((minute()/12) * 6);
  angulo = (angulo / 57.29577951);
  int x3 = (relogio_X + (sin(angulo) * (relogioRadius - (relogioRadius / 2))));
  int y3 = (relogio_Y - (cos(angulo) * (relogioRadius - (relogioRadius / 2))));
  display->drawLine(relogio_X + x, relogio_Y + y, x3 + x, y3 + y);
  }
*/

// DECLARAÇÃO DE PINOS
#define pinLED 16

/**************************************************************************************
 ***************************CONFIGURAÇÃO DO CLIENTE NTP********************************
 **************************************************************************************/
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);


//DECLARAÇÃO DE VARIAVEIS
char ssid []  = ")o,0(";
char password [] = "riko1234";

void clockOverlay (OLEDDisplay *display, OLEDDisplayUiState* state) {
}

void dht11(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  float celsius = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);
  float umidade = dht.readHumidity();

  if (isnan(umidade) || isnan(celsius) || isnan(fahrenheit) ) {

    Serial.println(F("Falhou a leitura do sensor DHT!"));
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0, 5, "Erro, verifique o sensor DHT!");
    display->display();
    delay(3000);
    display->clear();
    //return;
  }
  else {
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0, 5, "TEMPERATURA: ");
    display->setFont(ArialMT_Plain_24);
    display->drawCircle(42, 52, 20 * 3);
    display->drawString(5, 25, String(celsius) + "ºC");
    display->display();
    delay(5000);
    display->clear();

    display->setFont(ArialMT_Plain_10);
    display->drawString(0, 5, "TEMPERATURA: ");
    display->setFont(ArialMT_Plain_24);
    display->drawString(5, 25, String(fahrenheit) + "ºF");
    display->display();
    delay(5000);
    display->clear();

    display->setFont(ArialMT_Plain_10);
    display->drawString(0, 5, "UMIDADE: ");
    display->setFont(ArialMT_Plain_24);
    display->drawString(5, 25, String(umidade) + "%");
    display->display();
    delay(5000);
    display->clear();

    //Exibição de dados do sensor DHT11 no monitor serial
    Serial.print(F("UMIDADE: "));
    Serial.print(umidade);
    Serial.println(F("%"));
    Serial.println ("");
    Serial.print ("TEMPERATURA: ");
    Serial.print(celsius);
    Serial.println(F("°C"));
    Serial.println ("");
    Serial.print(fahrenheit);
    Serial.println(F("°F"));
    delay(5000);
    Serial.println ("");
  }
}

void buscarWLAN (OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(0, 10, "BUSCANDO...");
  int redes = WiFi.scanNetworks();
  //display.display();
  delay(1000);
  //display->clear();

  for (int l = 0; l < redes; l++)
  {
    display->drawString(0, 5, "WLAN: " + String(WiFi.SSID(l)));
    display->drawString(0, 21, "RSSI: " + String(WiFi.RSSI(l) + 115) + "%");
    //DESENHA A BARRA DE PROGRESSO
    display->drawProgressBar(0, 41, 120, 10, int (WiFi.RSSI(l) + 115));
    //display.display();
    delay(3000);
    //display.clear();
  }
}

void calendario (OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  for (int g = 0; g < 120; g++)
  {
    Serial.print(ntp.getHours());
    Serial.print(":");
    Serial.print(ntp.getMinutes());
    Serial.print(":");
    Serial.println(ntp.getSeconds());
  }

  //EXIBE A DIA DA SEMANA DE 0 A 6, INIICIANDO PELO DOMINGO (0)
  char semana[7][12] = {"DOMINGO", "SEGUNDA", "TERÇA", "QUARTA", "QUINTA", "SEXTA", "SÁBADO"};

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
  display->drawString(0, 25, semana[ntp.getDay()]);
}


void relogioDigital(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  String agora = ntp.getFormattedTime();
  for (int g = 0; g < 120; g++)
  {
    Serial.print(ntp.getHours());
    Serial.print(":");
    Serial.print(ntp.getMinutes());
    Serial.print(":");
    Serial.println(ntp.getSeconds());

    // ARMAZENA A HORA ATUALIZADA NO FORMATO GREGORIANO
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->setFont(ArialMT_Plain_24);
    display->drawString(0, 25, agora);

    if (agora == "07:00:00") {
      ajustarHora();
    }
  }
}

void exibirSMS(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);  //Define fonte e tamanho da letra
  display->drawString (0, 5, "MENSAGEM");
  display->drawString (0, 22, "URGENTE!");
  display->display();
  delay(3000);
  display->clear();

  display->setFont(ArialMT_Plain_10);
  display->drawString (32, 0, "DE");
  display->setFont(ArialMT_Plain_16);
  display->drawString (21, 15, "HIGO");
  display->setFont(ArialMT_Plain_10);
  display->drawString (30, 30, "PARA");
  display->setFont(ArialMT_Plain_16);
  display->drawString (10, 45, "REBECA");
  display->display();
  delay(3000);
  display->clear();

  for (uint8_t i = 0; i < 3; i++)
  {
    for (int16_t j = -127; j < 192; j++)
    {
      if (j == 164)
      {
        display->setFont(ArialMT_Plain_16);
        display->drawString(j - 164, 12 * i, "SAUDADES!!!");
        display->display();
        delay(3000);
        display->clear();
      }
      if (j == 10)
      {
        display->setFont(ArialMT_Plain_10);
        display->drawString(j, 11, "DESDE QUE NASCEU,");
        delay(2000);
        display->drawString(j + 17, 22, "MEU CORAÇÃO");
        delay(2000);
        display->drawString(j + 35, 33, "É SEU!");
        display->display();
        delay(4000);
        display->clear();
      }
      else
      {
        display->setFont(ArialMT_Plain_16);
        display->drawString(j, 31, "EU TE AMO!");
        display->display();
        delay(17);
        display->clear();
      }
    }
  }
}

FrameCallback frames[] = {calendario, relogioDigital, dht11, buscarWLAN, exibirSMS};
int frameContador = 5;

OverlayCallback overlays [] = {clockOverlay};
int overlaysContador = 1;

void displayAnimado()
{
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //DESENHA O QUADRADO VAZIO
  display.drawRect(12, 12, 30, 30);
  delay(2000);
  //DESENHA O QUADRADO CHEIO
  display.fillRect(20, 20, 35, 35);
  delay(2000);
  //DESENHA O CIRCULO VAZIO
  for (int a = 1; a < 8; a++)
  {
    display.drawCircle(92, 32, a * 3);
  }
  display.display();
  delay(3000);
  display.clear();
}

void setup()
{
  //INICIALIZAÇÃO DO MONITOR SERIAL E DEFINIÇÃO DA VELOCIDADE
  Serial.begin(115200);
  delay(5);

  //DEFINIÇÃO DO LED VERDE DA PLACA
  pinMode(pinLED, OUTPUT);
  delay(15);
  digitalWrite(pinLED, HIGH);

  //DEFINIÇÃO DE FRAMES NO DISPLAY
  ui.setTargetFPS(60);
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);
  ui. setIndicatorPosition(TOP);
  ui.setIndicatorDirection(RIGHT_LEFT);
  ui.setFrameAnimation(SLIDE_RIGHT);
  ui.setFrames(frames, frameContador);
  ui.setOverlays(overlays, overlaysContador);
  ui.init();

  // INICIALIZAÇÃO DO DISPLAY OLED E DEFINIÇÃO MODO DE EXIBIÇÃO
  display.init();
  delay(25);
  displayAnimado();
  delay(25);
  conectarWLAN();

  //Iniciar o sensor DHT11
  dht.begin();
  //Iniciar o cliente NTP e ajustar a hora
  ntp.begin();
  delay(25);
  ntp.update();



}

void medirWLAN()
{

  /********************************************************************************************
     O Indicador de Potência do Sinal Recebido ou, simplesmente, RSSI mede a força do sinal
     de uma rede sem fio dentro de ambiente fechado ou aberto, segundo a norma IEEE 802.11.
     A escala RSSI representa a qualidade relativa do sinal recebido no dispositivo. Quanto
     maior o valor, maior é a intensidade do sinal. A escala varia entre -120 (mais fraco ou
     sinal inexistente) e 0 (mais forte). Normalmente, o sinal é utilizável quando acima de
     -75, sendo que acima de -50 pode ser considerado perfeito.
   ********************************************************************************************/

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  for (int h = 0; h < 30; h++)
  {
    int sinal = ((WiFi.RSSI() - 120) / 5) % 100;
    display.drawString(30, 0, ssid);
    display.drawString(0, 20, "RSSI:" + String(sinal) + "%");

    //DESENHA A GRÁFICO HORIZONTAL EM BARRA COM A POTÊNCIA DO SINAL
    display.drawProgressBar(0, 40, 120, 10, sinal);
    display.display();
    delay(150);
    display.clear();
  }
  delay(500);
}

void ajustarHora()
{
  digitalWrite(pinLED, LOW);
  delay(75);
  digitalWrite(pinLED, HIGH);
  delay(75);

  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(pinLED, LOW);
    delay(80);
    digitalWrite(pinLED, HIGH);
    delay(80);

    int e = 0;
    while (!ntp.update())
    {
      digitalWrite(pinLED, LOW);
      delay(100);
      digitalWrite(pinLED, HIGH);
      delay(100);

      Serial.print(".");

      display.drawString(e, 10, ".");
      e += 5;
      display.display();
      delay(100);
      display.clear();

      ntp.forceUpdate();
    }
    unsigned long novoTempo = millis();
    unsigned long velhoTempo = 2208988800UL;
    //unsigned long epoch = novoTempo - velhoTempo * SECS_PER_HOUR;
    //setTime(epoch);
  }
  else
  {
    display.drawString(0, 0, "Falha ao obter a hora!");
    delay(2000);
    display.drawString(0, 20, "Erro de conexão...");
    delay (10000);
    display.display();
    return;
  }
}

void conectarWLAN()
{

  //INICIALIZAÇÃO DA CONEXÃO WLAN\\
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);


  Serial.println();
  Serial.println();
  Serial.print("CONECTANDO...");
  Serial.println(ssid);
  delay(1500);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawXbm (34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  display.display();
  delay(2000);
  display.clear();

  display.drawString(0, 15, "CONECTANDO...");
  display.display();
  delay(2000);
  display.clear();

  int b = 0;
  int c = 10;
  while (WiFi.status() != WL_CONNECTED)
  {

    Serial.print(".");

    display.drawString(b, 10, ".");
    b += 5;
    display.display();
    delay(500);


    if (b == 50)
    {
      c += 5;
    }
    if (c == 3)
    {
      display.drawString(0, 0, "REBOOTING...");
      display.display();
      delay(1000);
      display.clear();
      delay(10);
      display.clear();
      ESP.restart();
    }

  }
  Serial.println();
  Serial.println("Done!");
  Serial.println("Wi-Fi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Máscara de rede: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  delay(2000);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Wi-Fi ");
  display.drawString(10, 15, "conectado!");
  //display.drawString(10, 30, "IP Adress:" + String(WiFi.localIP());
  display.display();
  delay(2000);
  display.clear();
  medirWLAN();
}



void loop()
{
  int remainingTime = ui.update();
  if (remainingTime > 0)
  {
    delay(remainingTime);
  }

  //dht11();
  //buscarWLAN();
  //exibirSMS();

  //ESP.deepSleep(1 * 60000000);//Dorme por 1 Minuto (Deep-Sleep em Micro segundos).

}
