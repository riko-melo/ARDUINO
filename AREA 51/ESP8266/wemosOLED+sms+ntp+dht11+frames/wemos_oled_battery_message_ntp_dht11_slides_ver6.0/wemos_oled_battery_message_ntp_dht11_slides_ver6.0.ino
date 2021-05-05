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
#include "TimeLib.h"
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

// Dimensão da tela OLED 0,96" 128x64
int relogio_X = 128 / 2;
int relogio_Y = ((64 - 16 ) / 2) + 16;
int relogioRadius = 26;


unsigned long data;
const unsigned long seventyYears = 2208988800UL;

#define DEBOUNCETIME 10 //tempo máximo de debounce para o botão (ms)
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

// DECLARAÇÃO DE PINOS
const byte btLEFT = 0;
const int btUP = 12;
const byte btDOWN = 13;
const byte btCENTER = 14;
#define pinLED 16
volatile int state = 0;
/**************************************************************************************
 ***************************CONFIGURAÇÃO DO CLIENTE NTP********************************
 **************************************************************************************/
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);


//DECLARAÇÃO DE VARIAVEIS
char ssid []  = ")o,0(";
char password [] = "riko1234";

void clockOverlay (OLEDDisplay *display, OLEDDisplayUiState* state) {
}

void calendario (OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  //EXIBE A DIA DA SEMANA DE 0 A 6, INIICIANDO PELO DOMINGO (0)
  char semana[7][12] = {"DOMINGO", "SEGUNDA", "TERÇA", "QUARTA", "QUINTA", "SEXTA", "SÁBADO"};


  display->setTextAlignment(TEXT_ALIGN_LEFT);
  // TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT, TEXT_ALIGN_CENTER_BOTH
  display->setFont(ArialMT_Plain_16);
  display->drawString(30 + x, 20 + y, semana[ntp.getDay()]);
  //display->setFont(ArialMT_Plain_16);
  //unsigned long novoTempo = ntp.getEpochTime();
  //data = novoTempo - 2208988800UL;
  //display->drawString(15, 40, String (data));
  //mirrorScreen();
  //display->setBrightness(60);
  //display->invertDisplay();

}

void relogioDigital(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  /*Serial.print(ntp.getHours());
    Serial.print(":");
    Serial.print(ntp.getMinutes());
    Serial.print(":");
    Serial.println(ntp.getSeconds());
  */
  // ARMAZENA A HORA ATUALIZADA NO FORMATO GREGORIANO
  String agora = ntp.getFormattedTime();
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_24);
  display->drawString(12 + x, 25 + y, agora);

  if (agora == "07:00:00") {
    ajustarHora();
  }
}

void dht11(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  float celsius = dht.readTemperature();
  float umidade = dht.readHumidity();
  float celsiusMini = 0;
  float celsiusMaxi = 0;

  /*
    if (isnan(umidade) || isnan(celsius) ) {

    Serial.println(F("Falhou a leitura do sensor DHT!"));
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_16);
    display->drawString(0 + x, 10 + y, "Erro, verifique o sensor DHT!");
    //drawStringMaxWidth(int16_t x, int16_t y, uint16_t maxLineWidth, String text);
    //return;
    }
    else {
  */

  display->setFont(ArialMT_Plain_16);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(5 + x, 10 + y, String(celsius) + "ºC");
  display->drawString(65 + x, 33 + y, String(umidade) + "%");
  //int umidadeWidth = display.getStringWidth(String(umidade));
  //display->drawfillCircle(50, 32, 12);
  //drawXbm(int16_t x, int16_t y, int16_t width, int16_t height, const uint8_t *xbm);

  /*
       LoadingDrawFunction loadingDrawFunction       = [](OLEDDisplay *display, LoadingStage* stage, uint8_t progress) {
       display->setTextAlignment(TEXT_ALIGN_CENTER);
       display->setFont(ArialMT_Plain_10);
       display->drawString(64, 18, stage->process);
       display->drawProgressBar(4, 32, 120, 8, progress);
     };


    //Exibição de dados do sensor DHT11 no monitor serial
    Serial.print(F("UMIDADE: "));
    Serial.print(umidade);
    Serial.println(F("%"));
    Serial.println ("");
    Serial.print ("TEMPERATURA: ");
    Serial.print(celsius);
    Serial.println(F("°C"));
    Serial.println ("");

    delay(5000);
    Serial.println ("");
  */

}
void exibirSMS(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);  //Define fonte e tamanho da letra
  display->drawString (0 + x, 10 + y, "MENSAGEM");
  display->drawString (15 + x, 25 + y, "URGENTE!");
  display->display();
  delay(3000);
  display->clear();

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString (41, 0, "DE");
  display->setFont(ArialMT_Plain_16);
  display->drawString (29, 13, "HIGO");
  display->setFont(ArialMT_Plain_10);
  display->drawString (35, 32, "PARA");
  display->setFont(ArialMT_Plain_16);
  display->drawString (15, 45, "REBECA");
  display->display();
  delay(3000);
  display->clear();

  for (int16_t j = -127; j < 192; j++)
  {
    int i = -125;
    i++;
    if (j == 164)
    {
      display->setFont(ArialMT_Plain_16);
      display->drawString(j - 164, i, "SAUDADES!!!");
      display->display();
      delay(3000);
      display->clear();
    }
    if (j == 10)
    {
      display->setFont(ArialMT_Plain_10);
      display->drawString(j, 11, "DESDE QUE NASCEU,");
      delay(2000);
      display->drawString(j + 17, 22, "MEU CORAÇÃO");
      delay(2000);
      display->drawString(j + 35, 33, "É SEU!");
      display->display();
      delay(4000);
      display->clear();
    }
    else
    {
      display->setFont(ArialMT_Plain_16);
      display->drawString(j, 31, "EU TE AMO!");
      display->display();
      delay(17);
      display->clear();
    }
  }
}

FrameCallback frames[] = {calendario, relogioDigital, dht11};
int frameContador = 3;

OverlayCallback overlays [] = {clockOverlay};
int overlaysContador = 1;



void displayAnimado()
{
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
  //INICIALIZAÇÃO DO MONITOR SERIAL E DEFINIÇÃO DA VELOCIDADE
  Serial.begin(115200);
  delay(5);

  //DEFINIÇÃO DE PINAGEM
  /* delay(5);
    //  pinMode(btLEFT, INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(btLEFT), interrupt0, HIGH);
    delay(5);
    // pinMode(btUP, INPUT_PULLUP);
    delay(5);
    //  attachInterrupt(digitalPinToInterrupt(btUP), buscarWLAN, CHANGE);
    delay(5);
    //pinMode(btDOWN, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(btDOWN), mostrarVoltagem, CHANGE);
    delay(5);
    //pinMode(btCENTER, INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(btCENTER), interrupt14, HIGH);
    delay(5);
  */
  pinMode(pinLED, OUTPUT);
  delay(5);
  digitalWrite(pinLED, HIGH);
  delay(5);

  //DEFINIÇÃO DE FRAMES NO DISPLAY
  ui.setTargetFPS(60);
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);
  ui. setIndicatorPosition(TOP);
  // ACEITA OS VALORES LEFT, RIGHT TOP, BOTTOM
  ui.setIndicatorDirection(RIGHT_LEFT);
  ui.setFrameAnimation(SLIDE_RIGHT);
  ui.setFrames(frames, frameContador);
  ui.setOverlays(overlays, overlaysContador);
  ui.init();

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
    display.drawString(0, 20, "RSSI: " + String((WiFi.RSSI() + 120)) + "%");
    //DESENHA A GRÁFICO HORIZONTAL EM BARRA COM A POTÊNCIA DO SINAL
    display.drawProgressBar(0, 41, 120, 10, int (WiFi.RSSI() + 120));
    display.display();
    delay(150);
    display.clear();

  }
  delay(500);
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
      digitalWrite(pinLED, LOW);
      delay(100);
      digitalWrite(pinLED, HIGH);
      delay(100);

      Serial.print(".");

      display.drawString(e, 10, ".");
      e += 5;
      display.display();
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
    display.display();
    //return;
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
  display.drawXbm (34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  display.display();
  delay(2000);
  display.clear();

  int b = 0;
  int c = 10;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    display.drawString(0, 15, "Conectando...");
    display.drawString(b, c, ".");
    b += 5;
    display.display();
    delay(500);


    if (b == 50)
    {
      c += 5;
    }
    if (c == 25)
    {
      display.drawString(0, 0, "Reiniciando...");
      display.display();
      delay(1000);
      display.clear();
      delay(10);
      display.clear();
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
  display.drawString(0, 35, WiFi.localIP().toString());
  display.display();
  delay(2000);
  display.clear();
  medirWLAN();
}

void loop()
{
  int remainingTime = ui.update();
  if (remainingTime > 0)
  {
    delay(remainingTime);
  }

  // ESP.deepSleep(1 * 360000000);//Dorme por 1 Minuto (Deep-Sleep em Micro segundos).
  /*
     Para colocar o NodeMcu em modo sleep, existem dois comandos: node.sleep(), que é para o modo “light sleep” (sono leve);
     e node.dsleep(), que é para o modo “deep sleep” (sono profundo). A diferença entre os dois é que, o primeiro pode ser
     configurado para sair do sleep a partir de uma interrupção, enquanto o segundo só sai do sleep depois de um tempo definido.
     /Além disso, teoricamente, o segundo comando abaixa mais o consumo de corrente do que o primeiro.

  */
}

/*void interrupt0() // Botão "PARA ESQUERDA"
  {
  medirWLAN();
  display.drawString(0, 48, WiFi.localIP().toString());
  delay(1500);
  }

  void buscarWLAN() // Botão "PARA CIMA"
  {
  delay(150);
  digitalWrite(btUP, state);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "BUSCANDO...");
  int redes = WiFi.scanNetworks();
  display.display();
  delay(1000);
  display.clear();

  for (int l = 0; l < redes; l++)
  {
    display.drawString(0, 5, "WLAN: " + String(WiFi.SSID(l)));
    display.drawString(0, 21, "RSSI: " + String(WiFi.RSSI(l) + "%"));
    //DESENHA A BARRA DE PROGRESSO
    display.drawProgressBar(0, 41, 128, 10, int (WiFi.RSSI(l)));
    display.display();
    delay(3000);
    display.clear();
  }
  }

  void mostrarVoltagem() // Botão "PARA BAIXO"
  {
  delay(150);
  digitalWrite(btDOWN, state);

  int tensao = ESP.getVcc();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "Tensão:" + String (tensao / 1024));
  display.drawVerticalLine(40, 0, 20);
  }
  void interrupt14() // Botão Central
  {

  }
*/
