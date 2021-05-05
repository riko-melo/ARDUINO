/******************************************DECLARAÇÃO DE BIBLIOTECAS*********************************************************************************************/
#include "TimeLib.h"
#include <Wire.h>
#include "SSD1306Wire.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <DHT.h>
#include "OLEDDisplayUi.h"
#include "images.h"
#include <WiFiClientSecure.h>
/*****************************************CONFIGURAÇÃO DO DISPLAY OLED 0,96****************************************************************************************/
SSD1306Wire display(0x3c, 5, 4);
OLEDDisplayUi ui( &display);
/*****************************************CONFIGURAÇÃO DO SENSOR DHT22*********************************************************************************************/
#define pinDHT 10
#define DHTTYPE DHT22
DHT dht(10, DHT22);
/*****************************************CONFIGURAÇÃO DO TELEGRAM*************************************************************************************************/
#define BOTtoken "628987865:AAErowZo2_gCthCyIJoCJgrjOpPmQKg48ac"
/*****************************************************************************************************
 ***************************CONFIGURAÇÃO DO CLIENTE NTP***********************************************
 *****************************************************************************************************/
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
String agora;
#define pinLED 16
/*****************************************************************************************************
 *******************************DECLARAÇÃO DE VARIAVEIS***********************************************
 *****************************************************************************************************/
#define DEBOUNCETIME 10 //tempo máximo de debounce para o botão (ms)
char ssid []  = ")o,0(";
char password [] = "riko1234";


/*
  int relogio_X = 128 / 2;
  int relogio_Y = ((64 - 16 ) / 2) + 16;
  int relogioRadius = 26;
  unsigned long data;
  const unsigned long seventyYears = 2208988800UL;
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

void clockOverlay (OLEDDisplay *display, OLEDDisplayUiState* state) {
}

void calendar (OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  char semana[7][12] = {"DOMINGO", "SEGUNDA", "TERÇA", "QUARTA", "QUINTA", "SEXTA", "SÁBADO"}; //----------> EXIBE O DIA DA SEMANA DE 0 A 6, INIICIANDO PELO DOMINGO
  //char meses[12][12] = {"JAN", "FEV", "MAR", "ABR, "MAI", "JUN", "JUL", "AGO", "SET", "NOV", "DEZ"}; //---->EXIBE OS MESES DO ANO DE 0 A 11, INICIANDO POR JANEIRO

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  // TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT, TEXT_ALIGN_CENTER_BOTH
  display->setFont(ArialMT_Plain_24);
  display->drawString(10 + x, 20 + y, semana[ntp.getDay()]);
}

void relogio(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  agora = ntp.getFormattedTime(); //----------------------------> VARIÁVEL QUE ARMAZENA A HORA ATUALIZADA NO FORMATO GREGORIANO
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_24);
  display->drawString(17 + x, 25 + y, agora);


}

void dht22(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {


  // NÃO UTILIZAR LAÇOES DE REPETIÇÃO DENTRO DO FRAME PELO MESMO MOTICO ACIMA
  // NÃO USAR "DELAY" DENTRO DO FRAME

  float celsius = dht.readTemperature();
  float umidade = dht.readHumidity();


  if (isnan(celsius) || isnan(umidade)) {
    display->invertDisplay();
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_16);
    display->drawString(25, 10, "THE DHT22");
    display->drawString(35, 30, "FAILED!"); //----------------------------> EXIBE A MENSAGEM DE ERRO
  }

  else {
    display->setFont(ArialMT_Plain_10);
    display->drawString(30, 9, "TEMPERATURA ");
    display->setFont(ArialMT_Plain_24);
    display->drawString(32, 25, String(celsius) + "ºC");
  }


  //display->setFont(ArialMT_Plain_24);
  //display->setTextAlignment(TEXT_ALIGN_LEFT);
  //display->drawString(12 + x, 14 + y, String(celsius) + "ºC");
  //display->drawString(67 + x, 31 + y, String(umidade) + "%");
  //int umidadeWidth = display.getStringWidth(String(umidade));
  //display->drawfillCircle(50, 32, 12);
}


FrameCallback frames[] = {calendar, relogio, dht22};
int frameContador = 3;

OverlayCallback overlays [] = {clockOverlay};
int overlaysContador = 1;

void displayAnimado()
{
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.drawRect(12, 12, 30, 30);//----------------------------------> DESENHA O QUADRADO VAZIO

  display.fillRect(20, 20, 35, 35); //---------------------------------> DESENHA O QUADRADO CHEIO

  for (int a = 1; a < 8; a++)
  {
    display.drawCircle(92, 32, a * 3);//--------------------------------> DESENHA O CIRCULO VAZIO
  }
  display.display();
  delay(3000);
  display.clear();
}

void ajustarHora() {
  digitalWrite(pinLED, !digitalRead(pinLED));
  delay(75);
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, password);
    ntp.forceUpdate();
  }
WiFi.disconnect();
}


void medirWLAN() {
  /****************************************************************************************************************
     O INDICADOR DE POTÊNCIA DO SINAL RECEBIDO OU, SIMPLESMENTE, RSSI MEDE A FORÇA DO SINAL DE UMA REDE SEM FIO EM
     UM AMBIENTE, DE ACORDO COM A NORMA IEEE 802.11. A ESCALA RSSI AVALIA A INTENSIDADE DO SINAL RECEBIDO, DE FORMA
     RELATIVA, EM UM DISPOSITIVO. QUANTO MAIOR O VALOR, MAIOR É A INTENSIDADE DO SINAL. A ESCALA VARIA ENTRE -120
     (SINAL INEXISTENTE) E 0 (MAIS FORTE). NORMALMENTE, O SINAL É UTILIZÁVEL QUANDO ACIMA DE  -75, SENDO QUE ACIMA
     DE -50 PODE SER CONSIDERADO PERFEITO.
   ****************************************************************************************************************/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  for (int h = 0; h < 30; h++) {
    display.drawString(30, 0, ssid);
    display.drawString(0, 20, "RSSI: " + String((WiFi.RSSI() + 120)) + "%");
    display.drawProgressBar(0, 41, 120, 10, int (WiFi.RSSI() + 120)); //----->DESENHA A GRÁFICO HORIZONTAL EM BARRA COM A POTÊNCIA DO SINAL
    display.display();
    delay(150);
    display.clear();
  }
  delay(500);
}

void conectarWLAN() {

  //INICIALIZAÇÃO DA CONEXÃO WLAN
   String tentativas;
  int b = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(5);
 
  while (WiFi.status() != WL_CONNECTED) {
    tentativas += ".";
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawXbm (34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.drawString(b, 32, tentativas);
    b += 5;
    display.display();
    delay(500);
    display.clear();

    if (b == 125) {
      display.drawString(0, 35, "REINICIANDO...");
      display.display();
      delay(2000);
      display.clear();
      ESP.reset();
    }
    tentativas = "";
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "Wi-Fi ");
    display.drawString(10, 15, "conectado!");
    display.drawString(0, 35, WiFi.localIP().toString());
    display.display();
    delay(2000);
    display.clear();
    display.drawString(0, 0, "GATEWAY:");
    display.drawString(25, 15, WiFi.gatewayIP().toString());
    display.drawString(0, 30, "SUBMÁSCARA:");
    display.drawString(5, 45, WiFi.subnetMask().toString());
    display.display();
    delay(2000);
    display.clear();

    medirWLAN();
  }
}

void setup() {
  pinMode(pinLED, OUTPUT);//--------------------------------------------------> DEFINE O MODO DE OPERAÇÃO DO PINO 16 COMO SAÍDA
  delay(5);
  digitalWrite(pinLED, HIGH);  //----------------------------------------------------------> DEFINE O ESTADO DO LED COMO DESLIGADO
  delay(5);

  //DEFINIÇÃO DO OLEDDISPLAY UI
  ui.setTargetFPS(60);//------------------------------------------------------------> DEFINE A QUANTIDADE DE QUADROS POR SEGUNDO
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);
  ui. setIndicatorPosition(TOP); //--------------------> RECEBE OS VALORES LEFT, RIGHT TOP, BOTTOM

  ui.setIndicatorDirection(RIGHT_LEFT);
  ui.setFrameAnimation(SLIDE_RIGHT);
  ui.setFrames(frames, frameContador);
  ui.setOverlays(overlays, overlaysContador);
  ui.init();

  // INICIALIZAÇÃO DO DISPLAY OLED E DEFINIÇÃO MODO DE EXIBIÇÃO
  display.init(); //------------------------------------------> INICIALIZAÇÃO O DISPLAY OLED EMBUTIDO NA PLACA
  delay(5);
  displayAnimado();
  delay(5);
  conectarWLAN();

  dht.begin();  //----------------------->INICIALIZAÇÃO O SENSOR DHT22
  delay(5);
  ntp.begin(); //----------------------->INICIALIZAÇÃO O OBJETO NTP
  delay(5);
  ntp.update();

}

void loop() {
  int remainingTime = ui.update();
  if (remainingTime > 0) {
    delay(remainingTime);
  }
  if ((agora == "07:00:00") || (agora == "13:00:00" || (agora == "20:00:00")))   {
    ajustarHora();
  }
}

