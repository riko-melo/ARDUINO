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
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "SSD1306Wire.h"
#include <DHT.h>

// Configuração do Display OLED
SSD1306Wire  display(0x3c, 5, 4);

#define pinLED 16

#define pinDHT 10
#define DHTTYPE DHT11

DHT dht(10, DHT11);
float umidade, celsius, fahrenheit;
// Definição da conexão Wi-Fi

char ssid []  = ")o,0(";
char password [] = "riko1234";


// Definição do Servidor NTP
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
//Dia da semana de 0 a 6, sendo 0 o domingo
char semana[7][12] = {"DOMINGO", "SEGUNDA", "TERÇA", "QUARTA", "QUINTA", "SEXTA", "SÁBADO"};

//Time stamp do NTP se encontra nos primeiros 48 bytes da mensagem
//const int NTP_PACKET_SIZE = 48;
//Buffer para armazenar os pacotes transmitidos e recebidos
//byte NTPBuffer[NTP_PACKET_SIZE];


String tempo, data;

int redes;
int contador = 0;

void setup() {
  // Inicializaçaõ do monitor serial e definição da velocidade de transmissão
  Serial.begin(115200);
  delay(15);
  pinMode(pinLED, LOW);
  // Inicialização do display OLED e definição do modo de exibição
  displayAnimado();
  delay(15);
  conectarWLAN();
  delay(15);
  //Iniciar o sensor DHT11
  dht.begin();
  delay(15);
  //Iniciar o cliente NTP e ajustar a hora
  ntp.begin();
  delay(15);
  ntp.update();
  //sendNTPpacket(timeServerIP);
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
  for (int h = 0; h < 20; h++)
  {
    display.drawString(25, 0, ssid);
    display.drawString(0, 20, "RSSI: ");
    //display.drawString(15, 20, String (WiFi.RSSI());

    //Desenha a barra de progresso
    display.drawProgressBar(0, 32, 120, 10, 100);
    display.drawString(64, 15, String(WiFi.RSSI() - 100) + "%");
    display.display();
    delay(150);
    display.clear();
  }
}

void ajustarHora()
{
  digitalWrite(pinLED, LOW);
  delay(100);
  digitalWrite(pinLED, HIGH);
  delay(250);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(pinLED, LOW);
    delay(100);
    digitalWrite(pinLED, HIGH);
    delay(100);

    int e = 0;
    while (!ntp.update())
    {
      Serial.print(".");

      display.drawString(e, 10, ".");
      e += 5;
      display.display();
      delay(100);
      display.clear();
      digitalWrite(pinLED, LOW);
      delay(100);
      digitalWrite(pinLED, HIGH);
      delay(100);
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
      /*
         i ++;
        delay(500);
        Serial.print(".");
        display.drawString((3 + i * 2), 35, "." );
        display.display();
      */
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
  grafico();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Wi-Fi ");
  display.drawString(10, 15, "conectado!");
  display.display();
  delay(2000);
  display.clear();
  medirWLAN();
  display.display();
  delay(500);
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
}

void exibirSMS()
{
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);  //Define fonte e tamanho da letra
  display.drawString (0, 5, "MENSAGEM");
  display.drawString (0, 22, "URGENTE!");
  display.display();
  delay(3000);
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString (30, 0, "DE");
  display.setFont(ArialMT_Plain_16);
  display.drawString (0, 15, "HIGO");
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

void buscarWLAN()
{
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display. drawString(0, 10, "BUSCANDO...");
  redes = WiFi.scanNetworks();
  display.display();
  delay(1000);
  display.clear();

  for (int l = 0; l < redes; l++)
  {
    display.drawString(0, 5, "WLAN: ");
    display.drawString(0, 15, WiFi.SSID(l));
    display.drawString(0, 30, "RSSI: ");
    display.drawString(20, 30, String(WiFi.RSSI(l)));
    display.display();
    delay(2000);
    display.clear();
  }
}

void dht11()
{

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

  celsius = dht.readTemperature();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 5, "TEMPERATURA: ");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 35, String(celsius));
  display.drawString (25, 35, "ºC");
  display.display();
  delay(5000);
  display.clear();

  fahrenheit = dht.readTemperature(true);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 5, "TEMPERATURA: ");
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 35, String(fahrenheit));
  display.drawString (25, 35, "ºF");
  display.display();
  delay(5000);
  display.clear();

  umidade = dht.readHumidity();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 5, "UMIDADE: ");
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 35, String(umidade));
  display.drawString (25, 35, "%");
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
  contador++;
  /*

    int resultado = DHT.acquireAndWait(0);
    switch (resultado)
    {
    case DHTLIB_OK:
      Serial.println("OK");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Error\n\r\tChecksum error");
      break;
    case DHTLIB_ERROR_ISR_TIMEOUT:
      Serial.println("Error\n\r\tISR time out error");
      break;
    case DHTLIB_ERROR_RESPONSE_TIMEOUT:
      Serial.println("Error\n\r\tResposta time out error");
      break;
    case DHTLIB_ERROR_DATA_TIMEOUT:
      Serial.println("Error\n\r\tData time out error");
      break;
    case DHTLIB_ERROR_ACQUIRING:
      Serial.println("Error\n\r\tAquisição");
      break;
    case DHTLIB_ERROR_DELTA:
      Serial.println("Error\n\r\tVariação de tempo muito curto");
      break;
    case DHTLIB_ERROR_NOTSTARTED:
      Serial.println("Error\n\r\tNão iniciado");
      break;
    default:
      Serial.println("Erro desconhecido");
      break;
    }
  */
  if (contador == 10) {
    dht11();
    contador = 0;
    if (tempo == "07:00:00" or "19:00:00")
    {
      ajustarHora();
    }
    // Variável que armazena hora atualizada no formato gregoriano
    tempo = ntp.getFormattedTime();


    Serial.print(semana[ntp.getDay()]);
    Serial.println(", ");

    display.setFont(ArialMT_Plain_10);
    display.drawRect(5, 0, 30, 15);
    display.drawString(10, 0, semana[ntp.getDay()]);
    display.setFont(ArialMT_Plain_24);
    display.drawString(22, 20, String(data));
    display.display(); // Escreve as informações da lista de escrita no display
    delay(5000);
    display.clear();


    for (int g = 0; g < 120; g++)
    {
      Serial.print(ntp.getHours());
      Serial.print(":");
      Serial.print(ntp.getMinutes());
      Serial.print(":");
      Serial.println(ntp.getSeconds());
      display.drawLine(15, 22, 120, 22); // Adiciona uma linha à lista de escrita do display
      display.setFont(ArialMT_Plain_24);
      display.drawString(22, 20, ntp.getFormattedTime());
      display.drawLine(15, 50, 120, 50); // (x0, y0, x1, y1) Adiciona uma linha à lista de escrita do display
      display.display(); // Escreve as informações da lista de escrita no display
      delay(500);
      display.clear();
    }

    exibirSMS();
    buscarWLAN();
    //ESP.deepSleep(1 * 60000000);//Dorme por 1 Minuto (Deep-Sleep em Micro segundos).
  }
}
