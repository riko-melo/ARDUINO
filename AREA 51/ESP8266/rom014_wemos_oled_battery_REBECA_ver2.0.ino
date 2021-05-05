/*
  #include <ArduinoJson.h>

   #if ARDUINOJSON_VERSION_MAJOR != 5
   The software was build using ArduinoJson v5.x
   version 6 is still in beta at the time of writing
   go to tools -> manage libraries, search for ArduinoJSON and install the latest version 5
   #error Please upgrade/downgrade ArduinoJSON library to version 5!
   #endif
*/

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <WiFiUdp.h>
#include <NTPClient.h>


#include "SSD1306Wire.h"
#include "images.h"
// Configuração do Display OLED
SSD1306Wire  display(0x3c, 5, 4);

// Definição da conexão Wi-Fi
char ssid []  = ")o,0(";
char password [] = "riko1234";


// Definição do Servidor NTP
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
char days[7][12] = {"DOMINGO", "SEGUNDA", "TERÇA", "QUARTA", "QUINTA", "SEXTA", "SÁBADO"};


String tempo;
int b = 0;
#define pinLED 16
int rssi;


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(pinLED, OUTPUT);


  // Iniciar display e configurar interface
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);


  //Iniciar a conexão com a rede WLAN
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(500);

  Serial.print("CONECTANDO...");
  Serial.println(ssid);

  delay(500);
  display.drawString(0, 5, "CONECTANDO...");
  display.display();
  delay(1500);
  display.clear();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    display.drawString(b, 10, ".");
    b += 5;
    display.display();
    delay(500);
    display.clear();
  }

  Serial.println("Done!");
  Serial.println("Wi-Fi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println();

  display.drawString(0, 20, "Wi-Fi conectado");
  display.drawString(35, 45, ssid);
  display.display();
  delay(3000);
  display.clear();

  ntp.begin();
  delay(500);
  ntp.update();

}


void loop()
{
  display.clear(); // Limpa o conteúdo do display

  ntp.forceUpdate();
  for (int c = 0; c < 20; c++)
  {
    Serial.print(days[ntp.getDay()]);
    Serial.print(", ");
    Serial.print(ntp.getHours());
    Serial.print(":");
    Serial.print(ntp.getMinutes());
    Serial.print(":");
    Serial.println(ntp.getSeconds());

    delay(150);
    display.setFont(ArialMT_Plain_16);
    display.drawString(10, 0, days[ntp.getDay()]);
    display.drawLine(15, 50, 120, 50); // Adiciona uma linha à lista de escrita do display
    display.setFont(ArialMT_Plain_24);
    display.drawString(22, 20, ntp.getFormattedTime());
    display.display(); // Escreve as informações da lista de escrita no display
    delay(500);
    display.clear();
  }
  
  Serial.println("Acordando o LED...");
  delay(500);
  for (uint8_t d = 0; d < 20; d++) {
    digitalWrite(pinLED, LOW);
    delay(50);
    digitalWrite(pinLED, HIGH);
    delay(50);
    display.setFont(ArialMT_Plain_16);
    display.drawString (0, 0, "MENSAGEM");
    display.drawString (0, 20, "URGENTE:");
    display.drawString (40, 45, "REBECA, ");
    display.display();
  }

  delay(2000);
  display.clear(); // Limpa o conteúdo do display

  for (uint8_t f = 0; f < 3; f++)
  {
    for (int16_t g = -127; g < 192; g++)
    {
      if (g == 164) {
        display.setFont(ArialMT_Plain_16);
        display.drawString(g - 164, 25, "SAUDADES!!!");
        display.display();
        delay(3000);
        display.clear();
      }
      if (g == 10)
      {
        display.setFont(ArialMT_Plain_10);
        display.drawString(g, 10, "DESDE QUE NASCEU,");
        delay(2000);
        display.drawString(g + 17, 30, "MEU CORAÇÃO");
        delay(2000);
        display.drawString(g + 35, 50, "É SEU!");
        display.display();
        delay(4000);
        display.clear();
      }
      else
      {
        display.setFont(ArialMT_Plain_16);
        display.drawString(g, 31, "EU TE AMO!");
        display.display();
        delay(15);
        display.clear();
      }
    }
  }
}
