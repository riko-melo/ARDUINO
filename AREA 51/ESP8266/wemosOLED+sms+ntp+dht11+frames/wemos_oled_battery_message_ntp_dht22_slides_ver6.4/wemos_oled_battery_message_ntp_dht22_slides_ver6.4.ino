/*
  DATA: 25 / 05 / 2019
  == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
  |                                          TERMOS DA LICENÇA                                             |
  == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
    |  ESTE CÓDIGO É CEDIDO, DE FORMA GRATUITA E NÃO EXCLUSIVA, PARA USO A QUEM POSSA INTERESSAR, RESSALVADOS |
    |  OS DIREITOS AUTORAIS E DE COMERCIALIZAÇÃO. AO USAR O CÓDIGO, PARCIAL OU TOTALMENTE, VOCÊ CONCORDA      |
    |  OS TERMOS DESSA LICENÇA PARA NÃO ALUGAR, ARRENDAR, SUBLICENCIAR, DISTRIBUIR, DIVULGAR, FORNECER,       |
    |  TRANSFERIR, A TITULO ONEROSO, A QUALQUER TERCEIRO O PRESENTE OBJETO.                                   |
    |                                          ISENÇÃO DE GARANTIA                                            |
    |  O TITULAR DOS DIREITOS AUTORAIS NÃO CONFERE QUALQUER TIPO DE GARANTIA SOBRE O PRODUTO, CONTRATUAL      |
    |  OU LEGAL, BEM COMO, EM NENHUMA CIRCUNSTANCIA, TEM OBRIGAÇÃO DE MODIFICAR, ATUALIZAR, CORRIGIR DE       |
    |  ERROS, MAS PODE FAZÊ -LO A SEU CRITÉRIO EXCLUSIVO.                                                      |
    |                                      EXCLUSÃO DE RESPONSABILIDADE                                       |
    |  O AUTOR NÃO SE RESPONSABILIZA POR QUASIQUER DANOS ESPECIAIS, DIRETOS OU INDIRETOS RELACIONADOS COM ESTE |
    |  ESTE ACORDO E / OU COM O PRODUTO, INCLUINDO, SEM LIMITAÇÃO, PERDA DE LUCROS, DE EQUIPAMENTOS E DE DADOS. |
    |  O USUÁRIO RECONHECE QUE LEU ESTE CONTRATO E CONCORDA COM TODOS OS SEUS TERMOS, COMPROMETENDO - SE AO FIEL |
    |  BOM CUMPRIMENTO DESTE INSTRUMENTO.                                                                     |
    |                                    COPYRIGHT (c) 2018 by Higo Melo                                      |
    == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
*/
/******************************************DECLARAÇÃO DE BIBLIOTECAS*********************************************************************************************/
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

/*************************************CONFIGURAÇÃO DO RELOGIO ANALOGICO**********************************************************************************/
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

void humidade (OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  // AVISO IMPORTANTE: NÃO UTILIZE FOR, WHILE, DO.. WHILE, NESSA FUNÇÃO, SOB PENA DE EMBARAÇAR OS FRAMES OU TRAVAR O CÓDIGO

  float umidade = dht.readHumidity();

  if  (isnan(umidade)) {
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_16);
    display->drawString(0, 15, "No DHT22 Detected!");
  }

  else if (umidade < 65) {
    display->invertDisplay();
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_16);
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
    display->drawString(67 + x, 31 + y, String(umidade) + " % ");
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

void setup()
{
 

}

void medirWLAN() {
 
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

void loop()
{
  
  
}



