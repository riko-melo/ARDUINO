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

// DECLARAÇÃO DE PINOS
#define pinLED 16
const int pushButton = 4;

/**************************************************************************************
 ***************************CONFIGURAÇÃO DO CLIENTE NTP********************************
 **************************************************************************************/
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);


//DECLARAÇÃO DE VARIAVEIS
char ssid []  = ")o,0(";
char password [] = "riko1234";
int stateButton;
boolean verificador = true;
int x = 1;
unsigned long debounce;


void dht11() { }

void buscarWLAN() { }

void calendar () { }


void relogio() { }


void setup()
{
  //DEFINIÇÃO DO LED VERDE DA PLACA
  pinMode(pinLED, OUTPUT);
  delay(15);
  digitalWrite(pinLED, HIGH);
  pinMode(pushButton, INPUT_PULLUP);


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

void medirWLAN() {

}

void ajustarHora() {

}


void conectarWLAN() {

  medirWLAN();
}

void displayAnimado() {
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

void exibirSMS() {
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);  //Define fonte e tamanho da letra
  display.drawString (0, 5, "MENSAGEM");
  display.drawString (0, 22, "URGENTE!");
  display.display();
  delay(3000);
  display.clear();

  display.setFont(ArialMT_Plain_10);
  display.drawString (32, 0, "DE");
  display.setFont(ArialMT_Plain_16);
  display.drawString (21, 15, "HIGO");
  display.setFont(ArialMT_Plain_10);
  display.drawString (30, 30, "PARA");
  display.setFont(ArialMT_Plain_16);
  display.drawString (10, 45, "REBECA");
  display.display();
  delay(3000);
  display.clear();

  for (uint8_t i = 0; i < 3; i++)
  {
    for (int16_t j = -127; j < 192; j++)
    {
      if (j == 164)
      {
        display.setFont(ArialMT_Plain_16);
        display.drawString(j - 164, 12 * i, "SAUDADES!!!");
        display.display();
        delay(3000);
        display.clear();
      }
      if (j == 10)
      {
        display.setFont(ArialMT_Plain_10);
        display.drawString(j, 11, "DESDE QUE NASCEU,");
        delay(2000);
        display.drawString(j + 17, 22, "MEU CORAÇÃO");
        delay(2000);
        display.drawString(j + 35, 33, "É SEU!");
        display.display();
        delay(4000);
        display.clear();
      }
      else
      {
        display.setFont(ArialMT_Plain_16);
        display.drawString(j, 31, "EU TE AMO!");
        display.display();
        delay(17);
        display.clear();
      }
    }
  }
}

void loop() {
  if (verificador == true) {
    switch (x) {
      case 1: {
          dht11();
          break;
        }
      case 2:
        {
          medirWLAN();
          break;
        }

      case 3:
        {
          calendar ();
          break;
        }

      case 4:
        {
          relogio();
          break;
        }

      case 0:
        {
          buscarWLAN();
          break;
        }
    }
  }

  if (stateButton == HIGH) {
    x++;
    if (x > 4) {
      x = 0;
    }
  }
}


