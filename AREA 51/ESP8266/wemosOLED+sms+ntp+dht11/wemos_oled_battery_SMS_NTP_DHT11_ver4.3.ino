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
#include <Wire.h>
#include "SSD1306Wire.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <DHT.h>

/**************************************************************************************
 ************************CONFIGURAÇÃO DO DISPLAY OLED 0,96*****************************
 **************************************************************************************/
SSD1306Wire display(0x3c, 5, 4);

// DECLARAÇÃO DE PINOS
#define pinLED 16

/**************************************************************************************
 ***************************CONFIGURAÇÃO DO CLIENTE NTP********************************
 **************************************************************************************/
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
//Dia da semana de 0 a 6, sendo 0 o domingo
char semana[7][12] = {"DOMINGO", "SEGUNDA", "TERÇA", "QUARTA", "QUINTA", "SEXTA", "SÁBADO"};
String tempo, data;

//PARAMETROS DO SENSOR DHT11
#define pinDHT 10
#define DHTTYPE DHT11
DHT dht(10, DHT11);
float umidade, celsius, fahrenheit;

//DECLARAÇÃO DE VARIAVEIS
int redes;
char ssid []  = ")o,0(";
char password [] = "riko1234";



void setup() {
  //INICIALIZAÇÃO DO MONITOR SERIAL E DEFINIÇÃO DA VELOCIDADE\\
  Serial.begin(115200);

  pinMode(pinLED, OUTPUT);

  // INICIALIZAÇÃO DO DISPLAY OLED E DEFINIÇÃO MODO DE EXIBIÇÃO
  display.init();
  delay(50);
  displayAnimado();

  conectarWLAN();

  //Iniciar o sensor DHT11
  dht.begin();

  //Iniciar o cliente NTP e ajustar a hora
  ntp.begin();
  ntp.update();

}

/*void graficos()
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
*/

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
    display.drawString(30, 0, ssid);
    display.drawString(0, 20, "RSSI:");
    display.drawString(55, 15, String(WiFi.RSSI() + 115) + "%");
    //Desenha a barra de progresso
    display.drawProgressBar(0, 40, 120, 10, int (WiFi.RSSI() + 115));

    delay(150);
    display.clear();
  }
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
      Serial.print(".");

      digitalWrite(pinLED, LOW);
      delay(100);
      digitalWrite(pinLED, HIGH);
      delay(100);

      display.drawString(e, 10, ".");
      e += 5;

      delay(100);
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
  display.drawString(0, 15, "CONECTANDO...");

  delay(2000);
  display.clear();

  int b = 0;
  int c = 10;
  while (WiFi.status() != WL_CONNECTED)
  {

    {
      Serial.print(".");

      display.drawString(b, 10, ".");
      b += 5;

      delay(500);

      if (b == 50)
      {
        c += 5;
      }
      if (c == 3)
      {

        display.clear();
        delay(50);
        display.drawString(0, 0, "REBOOTING...");

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

    delay(2000);
    display.clear();
    medirWLAN();

    delay(500);
    display.clear();
  }
  display.display();
}

void displayAnimado()
{

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //Desenha um quadrado
  display.drawRect(12, 12, 30, 30);
  delay(100);//Desenha um quadrado cheio
  display.fillRect(20, 20, 35, 35);
  //Desenha circulos
  delay(100);
  for (int a = 1; a < 8; a++)
  {
    display.drawCircle(92, 32, a * 3);
    delay(3000);

  }
  display.display();
  delay(1000);
  display.clear();
}

void exibirSMS()
{
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);  //Define fonte e tamanho da letra
  display.drawString (0, 5, "MENSAGEM");
  display.drawString (0, 22, "URGENTE!");

  delay(3000);
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString (30, 0, "DE");
  display.setFont(ArialMT_Plain_16);
  display.drawString (18, 15, "HIGO");
  display.setFont(ArialMT_Plain_10);
  display.drawString (30, 30, "PARA");
  display.setFont(ArialMT_Plain_16);
  display.drawString (10, 45, "REBECA");

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

        delay(4000);
        display.clear();
      }
      else
      {
        display.setFont(ArialMT_Plain_16);
        display.drawString(j, 31, "EU TE AMO!");

        delay(17);
        display.clear();
      }
      display.display();
    }
  }
  display.display();
}

void buscarWLAN()
{
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display. drawString(0, 10, "BUSCANDO...");
  redes = WiFi.scanNetworks();

  delay(1000);
  display.clear();

  for (int l = 0; l < redes; l++)
  {
    display.drawString(0, 5, "WLAN: " + String(WiFi.SSID(l)));
    display.drawString(0, 20, "RSSI: " + String(WiFi.RSSI(l) + 115) + "%");
    //DESENHA A BARRA DE PROGRESSO
    display.drawProgressBar(0, 40, 120, 10, int (WiFi.RSSI(l) + 115));

    delay(3000);
    display.clear();
  }
  display.display();
}

void dht11()
{
  celsius = dht.readTemperature();
  fahrenheit = dht.readTemperature(true);
  umidade = dht.readHumidity();

  //Teste de funcionalidade do DHT11
  if (isnan(umidade) || isnan(celsius) || isnan(fahrenheit) )
  {
    Serial.println(F("Falhou a leitura do sensor DHT!"));

    display.println("Falhou a leitura do sensor DHT!");
    display.display();
    delay(3000);
    display.clear();
    return;
  }

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 5, "TEMPERATURA: ");
  display.setFont(ArialMT_Plain_24);
  display.drawString(5, 25, String(celsius) + "ºC");

  delay(5000);
  display.clear();


  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 5, "TEMPERATURA: ");
  display.setFont(ArialMT_Plain_24);
  display.drawString(5, 25, String(fahrenheit) + "ºF");

  delay(5000);
  display.clear();


  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 5, "UMIDADE: ");
  display.setFont(ArialMT_Plain_24);
  display.drawString(5, 25, String(umidade) + "%");
  display.display();
  delay(5000);
  display.clear();

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

void loop()
{
  if (tempo == "07:00:00")
  {
    ajustarHora();
  }
  // Variável que armazena hora atualizada no formato gregoriano

  tempo = ntp.getFormattedTime();

  Serial.print(semana[ntp.getDay()]);
  Serial.println(", ");

  for (int g = 0; g < 120; g++)
  {
    Serial.print(ntp.getHours());
    Serial.print(":");
    Serial.print(ntp.getMinutes());
    Serial.print(":");
    Serial.println(ntp.getSeconds());

    display.setFont(ArialMT_Plain_10);
    display.drawRect(5, 0, 54, 15);
    // (x0, y0, x1, y2)
    display.drawString(10, 0, semana[ntp.getDay()]);
    // Adiciona uma linha à lista de escrita do display
    display.drawLine(15, 20, 120, 20);

    display.setFont(ArialMT_Plain_24);
    display.drawString(22, 20, ntp.getFormattedTime());
    //Adiciona uma linha à lista de escrita do display
    display.drawLine(15, 47, 120, 47);
    // (x0, y0, x1, y1)
    // Escreve as informações da lista de escrita no display

    delay(500);
    display.clear();
  }

  dht11();
  buscarWLAN();
  exibirSMS();
  display.display();
  //ESP.deepSleep(1 * 60000000);//Dorme por 1 Minuto (Deep-Sleep em Micro segundos).

}
