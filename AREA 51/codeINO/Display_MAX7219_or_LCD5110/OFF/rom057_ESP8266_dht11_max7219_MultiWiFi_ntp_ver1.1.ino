#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SPI.h>

#include <MAX7219_Dot_Matrix.h>

#include "Arduino.h"

#include <String.h>
#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>
//#include <Timezone.h>


// CONFIGURAÇÃO DO ESP8266-12E
const byte modules = 8; //number of matrix in the display
#define pinDIN 15  // D8
#define pinCS  13  // D7
#define pinCLK 12  // D6

//DEFINIÇÃO DE OBJETO "display"
MAX7219_Dot_Matrix display (modules, pinCS, pinDIN, pinCLK);

//Cria uma instancia da classe ESP8266WiFiMulti para conectar as redes WiFi
ESP8266WiFiMulti wifiMulti;

WiFiClient client;

WiFiUDP udp;

NTPClient ntp(udp, "a.st1.ntp.br", -3, * 3600, 60000);
uint32_t tempoAtual = 0;
uint32_t horaCerta = 0;

String data, t, cidade;
const char * days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"} ;
const char * months[] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"} ;
const char * ampm[] = {"AM", "PM"} ;

float umidade, temperatura;

// =======================================================================
char message1 [] = "This is a testing display. Designed by Md. Khairul Alam";
char weather_message[300];
char time_msg[50];
char date_msg[50];

int LED = 2;

void setup() {
  Serial.begin(115200);
  startWiFi();
  ntp.begin();
  ntp.update();
  display.begin ();
  display.setIntensity (1);


pinMode(LED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

void updateDisplay (char *msg)
{
  //String message = msg;
  display.sendSmooth (msg, messageOffset);

  // next time show one pixel onwards
  if (messageOffset++ >= (int) (strlen (msg) * 8))
    messageOffset = - chips * 8;
}  // end of updateDisplay

long last = millis();
long lastTime = millis();
int count_sec = 0;

void loop() {
 
  if (millis() - lastTime >= 1000) {
    count_sec++;
    if (count_sec > 70)
      count_sec = 0;
    lastTime = millis();
  }

  if ((millis() - last) > 1000 * 60 * 15) { //update weather every 15 minutes
    last = millis();
  }

  // update display if time is up
  if (count_sec > 0 && count_sec < 50) {
    if (millis () - lastMoved >= MOVE_INTERVAL)
    {
      updateDisplay(weather_message);
      lastMoved = millis ();
    }
  }

  if (count_sec > 50 && count_sec < 60) {
    read_time_date();
    String s = data;
    int str_len = s.length() + 1;
    s.toCharArray(date_msg, str_len) ;
    if (millis () - lastMoved >= MOVE_INTERVAL)
    {
      updateDisplay(date_msg);
      lastMoved = millis ();
    }
  }
  if (count_sec > 60) {
    read_time_date();
    String ti = t;
    int str_len = ti.length() + 1;
    ti.toCharArray(time_msg, str_len) ;
    if (millis () - lastMoved >= MOVE_INTERVAL)
    {
      updateDisplay(time_msg);
      lastMoved = millis ();
    }
  }
}
 
void clockUpdate() {

  ntp.forceUpdate();
  horaCerta = millis();//Tempo atual em ms
  //Lógica de verificação do tempo
  if (horaCerta - tempoAtual > 500)
  {
    tempoAtual = horaCerta; // Salva o tempo atual
  }
  String hora = "HORA: ";
  hora += String (ntp.getFormattedTime());
  display.println(hora);
  display.setCursor(0, 20);
  String rssi = "WiFi:" ;
  rssi += String (WiFi.RSSI());
  rssi += "dBm";
  display.println(rssi);
 
  delay(3000);
 
}
void startWiFi()
{
  display.println();
  display.print("Conectando...");
  display.display();
  delay(2000);
  
  //Coloque aqui as redes wifi necessarias
  wifiMulti.addAP(")o,0(", "riko1234");
  wifiMulti.addAP("iPhone de HIGO", "riko2019");

  Serial.print("Conectando...");

  WiFi.mode(WIFI_STA);

  while (wifiMulti.run() != WL_CONNECTED)
  {
    //Aguarda a conexao da rede wifi
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  display.print("Connected:\t");
  display.print(WiFi.SSID());
  display.println();
  display.print(WiFi.localIP());
  
  delay(3000);
  
}
