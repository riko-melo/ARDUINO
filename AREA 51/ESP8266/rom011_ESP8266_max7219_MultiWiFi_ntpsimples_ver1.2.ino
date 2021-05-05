#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <SPI.h>

#include <MAX7219_Dot_Matrix.h>

#include "Arduino.h"

#include <String.h>
#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>


// CONFIGURAÇÃO DO ESP8266-12E
const byte modules = 8; //number of matrix in the display
#define pinDIN 15  // D8
#define pinCS  13  // D7
#define pinCLK 12  // D6

//Declaração de objeto display com o seu controlador MAX7219_Dot_Matrix
MAX7219_Dot_Matrix display (modules, pinCS, pinDIN, pinCLK);

//Cria uma instancia da classe ESP8266WiFiMulti para conectar as redes WiFi
ESP8266WiFiMulti wifiMulti;

//Declaração de objeto udp para acessar o servidor NTP através de rede WiFi, conexão UDP e porta 123.
WiFiUDP udp;

//Declaração de objeto ntp que utiliza o objeto UDP e assim obter as informações pretendidas do servidor NTP
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
/*           (objeto, servidor, fuso horario, intervalo de atualização) */

String data, hora, cidade;
const char * days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"} ;
const char * months[] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"} ;
const char * ampm[] = {"AM", "PM"} ;




void setup()
{
  Serial.begin(115200);
  startWiFi();
  ntp.begin();
  ntp.update();
  display.begin ();
  display.setIntensity (1);
}

void updateDisplay ()
{
  String hora = ntp.getFormattedTime();
}

void loop()
{
  updateDisplay ();
  delay(3000);
  updateClock();
}



void updateClock()
{
  ntp.forceUpdate();
  String hora = ntp.getFormattedTime(); // Salva o tempo atual
}
/* String hora = "HORA: ";
  hora += String (ntp.getFormattedTime());
  display.println(hora);
  display.setCursor(0, 20);
*/


void startWiFi()
{
  Serial.print("CONECTANDO...");
  display.println();
  display.print("CONECTANDO...");

  delay(2000);

  //Coloque aqui as redes wifi necessarias
  wifiMulti.addAP(")o,0(", "riko1234");
  wifiMulti.addAP("iPhone de HIGO", "riko2019");

  WiFi.mode(WIFI_STA);

  while (wifiMulti.run() != WL_CONNECTED)
  {

    Serial.println("CONEXÃO FALHOU!");
    delay(2000);
    Serial.println("REINICIANDO...");
    delay(3000);
    ESP.restart();
  }

  display.print("CONECTADO: ");
  display.print(WiFi.SSID());
  display.println();
  display.print(WiFi.localIP());
  delay(3000);
}
