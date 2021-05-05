/**********************************************************************************************************
  DATA: 25/05/2019
  ===========================================================================================================
  |                                          TERMOS DA LICENÇA                                             |
  ===========================================================================================================
  |  ESTE CÓDIGO É CEDIDO, DE FORMA GRATUITA E NÃO EXCLUSIVA, PARA USO A QUEM POSSA INTERESSAR, RESSALVADOS |
  |  OS DIREITOS AUTORAIS E DE COMERCIALIZAÇÃO. AO USAR O CÓDIGO, PARCIAL OU TOTALMENTE, VOCÊ CONCORDA      |
  |  OS TERMOS DESSA LICENÇA PARA NÃO ALUGAR, ARRENDAR, SUBLICENCIAR, DISTRIBUIR, DIVULGAR, FORNECER,       |
  |  TRANSFERIR, A TITULO ONEROSO, A QUALQUER TERCEIRO O PRESENTE OBJETO.                                   |
  |                                          ISENÇÃO DE GARANTIA                                            |
  |  O TITULAR DOS DIREITOS AUTORAIS NÃO CONFERE QUALQUER TIPO DE GARANTIA SOBRE O PRODUTO, CONTRATUAL      |
  |  OU LEGAL, BEM COMO, EM NENHUMA CIRCUNSTANCIA, TEM OBRIGAÇÃO DE MODIFICAR, ATUALIZAR, CORRIGIR DE       |
  |  ERROS, MAS PODE FAZÊ-LO A SEU CRITÉRIO EXCLUSIVO.                                                      |
  |                                      EXCLUSÃO DE RESPONSABILIDADE                                       |
  |  O AUTOR NÃO SE RESPONSABILIZA POR QUASIQUER DANOS ESPECIAIS, DIRETOS OU INDIRETOS RELACIONADOS COM ESTE|
  |  ESTE ACORDO E/OU COM O PRODUTO, INCLUINDO, SEM LIMITAÇÃO, PERDA DE LUCROS, DE EQUIPAMENTOS E DE DADOS. |
  |  O USUÁRIO RECONHECE QUE LEU ESTE CONTRATO E CONCORDA COM TODOS OS SEUS TERMOS, COMPROMETENDO-SE AO FIEL|
  |  BOM CUMPRIMENTO DESTE INSTRUMENTO.                                                                     |
  |                                    COPYRIGHT (c) 2018 by Higo Melo                                      |
  ===========================================================================================================
***********************************************************************************************************
                                        DECLARAÇÃO DE BIBLIOTECAS
**********************************************************************************************************/
#include <ESP8266WiFi.h>
#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <DHT.h>
#include "OLEDDisplayUi.h"
#include "images.h"
#include <WiFiClientSecure.h>
/*****************************************************************************************************
                                     CONFIGURAÇÃO DO DISPLAY OLED 0,96
 *****************************************************************************************************/
SSD1306Wire display(0x3c, 5, 4);
OLEDDisplayUi ui( &display);
/*****************************************************************************************************
                                        CONFIGURAÇÃO DO SENSOR DHT22
 *****************************************************************************************************/
#define pinDHT 10
#define DHTTYPE DHT22
DHT dht(10, DHT22);
/*****************************************************************************************************
                                      CONFIGURAÇÃO DO RELOGIO ANALOGICO
 *****************************************************************************************************/
// Dimensão da tela OLED 0,96" 128x64
int relogio_X = 128 / 2;
int relogio_Y = ((64 - 16 ) / 2) + 16;
int relogioRadius = 26;
unsigned long data;
const unsigned long seventyYears = 2208988800UL;
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
/**************************************CONFIGURAÇÃO DO API TELEGRAM**************************************************************************************/
#define pinLED 16
#define BOTtoken "628987865:AAErowZo2_gCthCyIJoCJgrjOpPmQKg48ac"
#define KeyOpenWeatherMAP "78cd6050d8905eea4dffa768043dec48"

/***************************************CONFIGURAÇÃO DO CONSTRUTOR NTP**********************************************************************************/
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
String agora;

/***************************************DECLARAÇÃO DE VARIAVEIS GLOBAIS*********************************************************************************/
#define DEBOUNCETIME 10 //tempo máximo de debounce para o botão (ms)
char ssid []  = ")o, 0(";
char password [] = "riko1234";
unsigned long tempo;

void clockOverlay (OLEDDisplay *display, OLEDDisplayUiState* state) { }

void calendar (OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  // AVISO IMPORTANTE: NÃO UTILIZE FOR, WHILE, DO.. WHILE, NESSA FUNÇÃO, SOB PENA DE EMBARAÇAR OS FRAMES OU TRAVAR O CÓDIGO

  char semana[7][12] = {"DOMINGO", "SEGUNDA", "TERÇA", "QUARTA", "QUINTA", "SEXTA", "SÁBADO"}; //-----> EXIBE O DIA DA SEMANA DE 0 A 6, INIICIANDO PELO DOMINGO
  //char meses[12][12] = {"JAN", "FEV", "MAR", "ABR, "MAI", "JUN", "JUL", "AGO", "SET", "NOV", "DEZ"}; //-->EXIBE OS MESES DO ANO DE 0 A 11, INICIANDO POR JANEIRO

  display->setTextAlignment(TEXT_ALIGN_LEFT);//-----------> RECEBE OS VALORES TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT, TEXT_ALIGN_CENTER_BOTH
  display->setFont(ArialMT_Plain_24);
  display->drawString(11 + x, 20 + y, semana[ntp.getDay()]);
  //display->setFont(ArialMT_Plain_16);
  //unsigned long novoTempo = ntp.getEpochTime();
  //data = novoTempo - 2208988800UL;
  //display->drawString(15, 40, String (data));
}

void relogio (OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  // AVISO IMPORTANTE: NÃO UTILIZE FOR, WHILE, DO.. WHILE, NESSA FUNÇÃO, SOB PENA DE EMBARAÇAR OS FRAMES OU TRAVAR O CÓDIGO

  agora = ntp.getFormattedTime(); //----------------------------> VARIÁVEL QUE ARMAZENA A HORA ATUALIZADA NO FORMATO GREGORIANO
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_24);
  display->drawString(17 + x, 25 + y, agora);

  Serial.print(ntp.getHours());
  Serial.print(": ");
  Serial.print(ntp.getMinutes());
  Serial.print(": ");
  Serial.println(ntp.getSeconds());
}

void temperatura (OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  // AVISO IMPORTANTE: NÃO UTILIZE FOR, WHILE, DO.. WHILE, NESSA FUNÇÃO, SOB PENA DE EMBARAÇAR OS FRAMES OU TRAVAR O CÓDIGO

  float celsius = dht.readTemperature();
  float celsiusMini = 0;
  float celsiusMax = 0;
  if (celsius > celsiusMax) {
    celsiusMax = celsius;
  }
  if (celsius < celsiusMini) {
    celsiusMini = celsius;
  }

  if (isnan(celsius)) {
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_16);
    display->drawString(0, 15, "No DHT22 Detected!"); // -------------------------------------------------> BUG: TTEXTO CORTADO, ENTÃO MUDAR FUNÇÃO
  }
  else {
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(35, 15, "TEMPERATURA ");
    display->setFont(ArialMT_Plain_24);
    display->drawString(25, 25, String(celsius) + "ºC");
  }
  display->clear();
}

void humidade (OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  // AVISO IMPORTANTE: NÃO UTILIZE FOR, WHILE, DO.. WHILE, NESSA FUNÇÃO, SOB PENA DE EMBARAÇAR OS FRAMES OU TRAVAR O CÓDIGO

  float umidade = dht.readHumidity();

  if  (isnan(umidade)) {
    //display->invertDisplay();
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0, 15, "No DHT22 Detected!");
  }

  else if (umidade < 65) {
    //display->invertDisplay();
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0, 15, "A UMIDADE ESTÁ BAIXA!");
    display->setFont(ArialMT_Plain_16);
    display->drawString(0, 30, "BEBA ÁGUA, JÁ!");
  }

  else {
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(45, 10, "UMIDADE: ");
    display->setFont(ArialMT_Plain_24);
    display->drawString(67 + x, 31 + y, String(umidade) + "%");
  }
}

FrameCallback frames[] = {calendar, relogio, temperatura, humidade};
int framesContador = 4;

OverlayCallback overlays [] = {clockOverlay};
int overlaysContador = 1;

void displayAnimado()  {
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

void setup(){
  pinMode(pinLED, OUTPUT); //------------------------------------------> DEFINE O MODO DE OPERAÇÃO DO PINO 16 COMO SAÍDA
  delay(5);
  digitalWrite(pinLED, HIGH); //-------------------------------------------------> DEFINE O ESTADO DO LED COMO DESLIGADO
  delay(5);

  //DEFINIÇÃO DE FRAMES NO DISPLAY
  ui.setTargetFPS(60); //----------------------------------------------------> DEFINE A QUANTIDADE DE QUADROS POR SEGUNDO
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);
  ui. setIndicatorPosition(TOP); //-------------------------------------------> ACEITA OS VALORES LEFT, RIGHT TOP, BOTTOM

  ui.setIndicatorDirection(RIGHT_LEFT);
  ui.setFrameAnimation(SLIDE_RIGHT);
  ui.setFrames(frames, framesContador);
  ui.setOverlays(overlays, overlaysContador);
  ui.init();

  // INICIALIZAÇÃO DO DISPLAY OLED E DEFINIÇÃO MODO DE EXIBIÇÃO
  display.init(); //----------------------------------------------------> INICIALIZAÇÃO O DISPLAY OLED EMBUTIDO NA PLACA
  delay(25);
  displayAnimado();
  delay(25);


  //Iniciar o sensor DHT11
  dht.begin();
  //Iniciar o cliente NTP e ajustar a hora
  ntp.begin();
  delay(5);
  ntp.update();

  //WiFi.mode(WIFI_STA); //--------------------------------------------------------------------------->INICIALIZAÇÃO DA CONEXÃO WLAN
  WiFi.begin(ssid, password);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawXbm (34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  display.display();
  delay(2000);
  display.clear();

  int b = 0;
  
  while (WiFi.status() != WL_CONNECTED) {

    display.drawString(0, 15, "CONECTANDO...");
    display.drawString(b, 22, ".");
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
  }

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Wi - Fi ");
  display.drawString(10, 15, "conectado!");
  display.drawLine(10, 25, 117, 25); // Adiciona uma linha à lista de escrita do display
  display.drawString(0, 35, WiFi.localIP().toString());
  display.display();
  delay(2000);
  display.clear();

  //display.drawString(0, 20, WiFi.gatewayIP().toString());
  //display.drawString(0, 20, WiFi.subnetMask().toString());
  //display.display();
  //delay(2000);
  //display.clear();

  medirWLAN();

}

void medirWLAN() {
  /**************************************************************************************************************
    O INDICADOR DE POTÊNCIA DO SINAL RECEBIDO OU, SIMPLESMENTE, RSSI MEDE A FORÇA DO SINAL DE UMA REDE SEM FIO EM
    UM AMBIENTE, DE ACORDO COM A NORMA IEEE 802.11. A ESCALA RSSI AVALIA A INTENSIDADE DO SINAL RECEBIDO, DE FORMA
    RELATIVA, EM UM DISPOSITIVO. QUANTO MAIOR O VALOR, MAIOR É A INTENSIDADE DO SINAL. A ESCALA VARIA ENTRE -120
    (SINAL INEXISTENTE) E 0 (MAIS FORTE). NORMALMENTE, O SINAL É UTILIZÁVEL QUANDO ACIMA DE  -75, SENDO QUE ACIMA
    DE -50 PODE SER CONSIDERADO PERFEITO.
  ***************************************************************************************************************/

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  for (int h = 0; h < 30; h++)
  {
    display.drawString(30, 0, ssid);
  }
  display.drawString(0, 20, "RSSI: " + String((WiFi.RSSI() + 120)) + " % ");
  if (WiFi.RSSI() > - 120 && WiFi.RSSI() < - 75) {
    display.drawString(0, 20, "SINAL RUIM!");
  }
  else  if (WiFi.RSSI() > - 74 && WiFi.RSSI() < - 49) {
    display.drawString(0, 20, "SINAL REGULAR!");
  }
  else {
    display.drawString(0, 20, "SINAL PRFEITO!");
  }

  //display.drawProgressBar(0, 41, 120, 10, int (WiFi.RSSI() + 120)); //----------> //DESENHA A BARRA EM PROGRESSO COM A POTÊNCIA DO SINAL
  display.display();
  delay(150);
  display.clear();
  delay(500);
}


void ajustarHora() {
  if (WiFi.status() == WL_CONNECTED)  {
    digitalWrite(pinLED, !digitalRead(pinLED));
    delay(75);
    digitalWrite(pinLED, !digitalRead(pinLED));
    delay(75);

    int e = 0;
    while (!ntp.update()) {

      digitalWrite(pinLED, !digitalRead(pinLED));
      delay(75);
      digitalWrite(pinLED, digitalRead(pinLED));
      delay(75);

      display.drawString(e, 10, ".");
      e += 5;
      display.display();
      delay(100);
      display.clear();

      ntp.forceUpdate();
    }
  }
  else  {
    display.drawString(0, 0, "FALHA AO OBTER A HORA!");
    delay(2000);
    display.drawString(0, 20, "SEM ACESSO A INTERNET!");
    delay (10000);
    display.display();
    //return;
  }
}

void conectarWLAN() { }

void loop()
{
  int remainingTime = ui.update();
  if (remainingTime > 0) {
    delay(remainingTime);
  }
  if (agora == "07:00:00" || agora == "13:00:00" || agora == "20:00:00") {
    ajustarHora();
  }

}


