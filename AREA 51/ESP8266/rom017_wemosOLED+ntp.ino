/*************************************************************************************
   O SOFTWARE É FORNECIDO GRATUITAMENTE, SEM GARANTIA DE QUALQUER TIPO, EXPRESSA
   OU IMPLÍCITA, DE GARANTIA, RESSALVADAS ÀS GARANTIAS DE COMERCIALIZAÇÃO E NÃO
   VIOLAÇÃO DOS DIREIOTS AUTORAIS. EM NENHUMA CIRCUNSTÂNCIA, O TITULAR DOS DIREITOS
   AUTORAIS PODE SER RESPONSABILIZADO POR QUAISQUER DANOS OU UTILIZAÇÃO INDEVIDA
   DESTE.
  ************************************************************************************
   Embutido na placa, há um display OLED 0,96", 128x64, monocromático. Este possui
   comunicação I2C e está ligado ao ESP8266 Wemos por meio de 3 fios:
   SDA no GPIO4   (para dados)
   SCL no GPIO15  (para o clock)
   RST no GPIO16 (para o reset e inicio do display)

  **************************************BIBLIOTECAS************************************
  *************************************************************************************/
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include "SSD1306Wire.h"

// Configuração do Display OLED
SSD1306Wire  display(0x3c, 5, 4);

#define pinLED 16

#define pinDHT 10
#define DHTTYPE DHT11

DHT dht(10, DHT11);

// Definição da conexão Wi-Fi
char ssid []  = ")o,0(";
char password [] = "riko1234";


// Definição do Servidor NTP
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
char days[7][12] = {"DOMINGO", "SEGUNDA", "TERÇA", "QUARTA", "QUINTA", "SEXTA", "SÁBADO"};

String tempo;

void setup() {
  // Inicializaçaõ do monitor serial e definição da velocidade de transmissão
  Serial.begin(115200);
  delay(50);
 NTP e ajustar a hora
  ntp.begin();
  delay(50);
  ntp.update();
  delay(50);
  // Inicialização do display OLED e definição do modo de exibição
  displayAnimado();
  //Inicializaçaõ da conexão com a rede WLAN
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(50);

  Serial.println();
  Serial.println();
  Serial.print("CONECTANDO...");
  Serial.println(ssid);
  delay(1500);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 15, "CONECTANDO...");
  display.display();
  delay(2000);
  display.clear();
  int b = 0;
  int c = 10;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");

    display.drawString(b, c, ".");
    b += 5;
    if (b == 50)
    {
      c += 5;
    }

  }
  Serial.println();
  Serial.println("Done!");
  Serial.println("Wi-Fi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(2000);
  grafico();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Wi-Fi ");
  display.drawString(10, 15, "conectado!");
  display.display();
  delay(2000);
  display.clear();
  
}


void displayAnimado()
{
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  //display.drawXbm(posX, posY, frame1_width, frame1_height, frame1_bits);
  display.display();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //Desenha um quadrado
  display.drawRect(12, 12, 30, 30);
  //Desenha um quadrado cheio
  display.fillRect(20, 20, 35, 35);
  //Desenha circulos
  for (int a = 1; a < 8; a++)
  {
    display.drawCircle(92, 32, a * 3);
    delay(3000);
    display.clear();
  }
  //   void conectar()   {   }
  display.display();
  delay(500);
  display.clear();
}

void grafico()
{
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  for (int d = 0; d <= 100; d++)
  {
    //Desenha a barra de progresso
    display.drawProgressBar(0, 32, 120, 10, d);
    display.drawString(64, 15, String(d) + "%");
    display.display();
    delay(19);
    display.clear();
  }
}


void ajustarHora()
{
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  if (WiFi.status() == WL_CONNECTED)
  {
    int e = 0;
    while (!ntp.update())
    {
      Serial.print(".");

      display.drawString(e, 10, ".");
      e += 5;
      display.display();
      delay(500);
      display.clear();
      ntp.forceUpdate();
    }
  }
  else
  {
    display.drawString(0, 0, "Falha ao obter a hora!");
    delay(2000);
    display.drawString(0, 20, "Erro de conexão...");
    delay (10000);
    return;
  }
}



void loop() {
  if (tempo == "07:00:00")
  {
    ajustarHora();
  }
  else if (tempo == "19:00:00")
  {
    ajustarHora();
  }
  // Variável que armazena hora atualizada no formato gregoriano
  tempo = ntp.getFormattedTime();

  for (int f = 0; f < 10; f++)
  {
    Serial.print(days[ntp.getDay()]);
    Serial.println(", ");

    display.setFont(ArialMT_Plain_16);
    display.drawString(10, 0, days[ntp.getDay()]);
    display.drawLine(15, 50, 120, 50); // Adiciona uma linha à lista de escrita do display
    display.setFont(ArialMT_Plain_24);
    //display.drawString(22, 20, data);
    display.display(); // Escreve as informações da lista de escrita no display
    delay(500);
    display.clear();
  }

  for (int g = 0; g < 100; g++)
  {
    Serial.print(ntp.getHours());
    Serial.print(":");
    Serial.print(ntp.getMinutes());
    Serial.print(":");
    Serial.println(ntp.getSeconds());

    display.drawLine(15, 50, 120, 50); // Adiciona uma linha à lista de escrita do display
    display.setFont(ArialMT_Plain_24);
    display.drawString(22, 20, ntp.getFormattedTime());
    display.display(); // Escreve as informações da lista de escrita no display
    delay(500);
    display.clear();
  }
  
}
