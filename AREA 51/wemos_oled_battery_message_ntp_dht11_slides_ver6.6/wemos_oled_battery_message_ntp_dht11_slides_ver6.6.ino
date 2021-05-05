
/******************************************DECLARAÇÃO DE BIBLIOTECAS*********************************************************************************************/
#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <DHT.h>
#include "OLEDDisplayUi.h"
#include "images.h"
#include <WiFiClientSecure.h>
/*******************************************************CONFIGURAÇÃO DO DISPLAY OLED 0,96*************************************************************************************/
SSD1306Wire display(0x3c, 5, 4);
OLEDDisplayUi ui( &display);
int remainingTime;
#define PIR 15 //---->(D8)
/**************************************************************CONFIGURAÇÃO DO DHT22******************************************************************************************/
#define pinDHT 1 //----------------------------------------->PINO DO DHT (D10)
#define DHTTYPE DHT22
DHT dht(1, DHT22);
float celsius, umidade, fahrenheit;
/*************************************************************CONFIGURAÇÃO DO TELEGRAM****************************************************************************************/
#define BOTtoken "628987865:AAErowZo2_gCthCyIJoCJgrjOpPmQKg48ac"

/**********************************************************CONFIGURAÇÃO DO CLIENTE NTP****************************************************************************************/
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
String agora;
#define pinLED 16 //-----------------------------------------> PINO DO LED VERDE EMBUTIDO (D0)
unsigned long intervalo = 0;
/********************************************************DECLARAÇÃO DE VARIAVEIS GLOBAIS*************************************************************************************/
#define DEBOUNCETIME 10 //tempo máximo de debounce para o botão (ms)
char ssid []  = ")o,0(";
char password [] = "riko1234";

/*************************************************************CONFIGURAÇÃO DO DESPERTADOR***************************************************************************************/
int alarme[2] = {06, 00}; // Variavel do alarme
char* musica[] = {"Re", "Mi", "Fa", "Sol", "Do", "La", "Fa", "Do", "Fim"}; //--------------------> MUSICA TEMA DE GoT
int tempo[] = {500, 500, 500, 500, 500, 500, 500, 500};
//void tocar(char* musica[], int tempo[]);
#define pinBuzzer 3 //---------------------------------------->PINO BUZZER 3V3 (D9)


#define pinUP 12 //-----------------------------------------> PINO BOTAO "PARA CIMA" (D6)
#define pinDOWN 14 //-------------------------------------------> PINO BOTAO "PARA BAIXO" (D5)
#define pinCENTER 13 //---------------------------------------> PINO BOTAO "CENTRO" (D7)
/*************************************************************CONFIGURAÇÃO DOS FRAMES***************************************************************************************/
void clockOverlay (OLEDDisplay *display, OLEDDisplayUiState* state) { }
/*
  void relogioAnalog(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y)
  {
   int relogio_X = 128 / 2;
  int relogio_Y = ((64 - 16 ) / 2) + 16;
  int relogioRadius = 26;
  unsigned long data;
  const unsigned long seventyYears = 2208988800UL;
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

void calendar (OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  char semana[7][12] = {"DOMINGO", "SEGUNDA", "TERÇA", "QUARTA", "QUINTA", "SEXTA", "SÁBADO"}; //--------------------> VARIÁVEL QUE ARMAZENA O NOME DOS DIAS DA SEMANA
  //char meses[12][12] = {"JAN", "FEV", "MAR", "ABR, "MAI", "JUN", "JUL", "AGO", "SET", "NOV", "DEZ"};

  display->setTextAlignment(TEXT_ALIGN_LEFT); //----------------> ACEITAS OS VALORES TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT, TEXT_ALIGN_CENTER_BOTH
  display->setFont(ArialMT_Plain_24);
  display->drawString(10 + x, 20 + y, semana[ntp.getDay()]); //--------------------> EXIBE O DIA DA SEMANA DE 0 A 6, INICIANDO PELO DOMINGO

}

void relogio(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  // MESMO SEM USAR ESTRUTURAS DE REPETIÇÃO E DELAY DENTRO DO FRAME, A HORA SERÁ ATUALIZADA SEGUNDO POR SEGUNDO

  agora = ntp.getFormattedTime(); //----------------------------> VARIÁVEL QUE ARMAZENA A HORA ATUALIZADA NO FORMATO GREGORIANO
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_24);
  display->drawString(17 + x, 25 + y, agora);
}

void temperatura(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  // NÃO USAR DELAY E ESTRUTURAS CONDICIONAIS DENTRO DO FRAME, HAVENDO FALHA NA LEITURA DO DHT22 SURGIRÁ A EXPRESSÃO "ISNAN" NA TELA
  //NÃO UTILIZAR A FUNÇÃO DE LIMPAR A TELA
  celsius = dht.readTemperature();
String calor = celsius;
calor ;

  display->setFont(ArialMT_Plain_10);
  display->drawString(30, 9, "TEMPERATURA ");
  display->setFont(ArialMT_Plain_24);
  display->drawString(32, 25, += "ºC");
  

  //display->setFont(ArialMT_Plain_24);
  //display->setTextAlignment(TEXT_ALIGN_LEFT);
  //display->drawString(12 + x, 14 + y, String(celsius) + "ºC");
  //display->drawString(67 + x, 31 + y, String(umidade) + "%");
  //int umidadeWidth = display.getStringWidth(String(umidade));
  //display->drawfillCircle(50, 32, 12);
}

void humidade(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {

  // NÃO USAR DELAY E ESTRUTURAS CONDICIONAIS DENTRO DO FRAME, HAVENDO FALHA NA LEITURA DO DHT22 SURGIRÁ A EXPRESSÃO "ISNAN" NA TELA
  //NÃO UTILIZAR A FUNÇÃO DE LIMPAR A TELA
  umidade = dht.readHumidity();

  display->setFont(ArialMT_Plain_10);
  display->drawString(30, 9, "UMIDADE");
  display->setFont(ArialMT_Plain_24);
  display->drawString(32, 25, String(umidade) + "%");

}

FrameCallback frames[] = {calendar, relogio, temperatura, humidade};
int frameContador = 4;

OverlayCallback overlays [] = {clockOverlay};
int overlaysContador = 1;
/*************************************************************DEFINIÇÃO DAS FUNÇÕES OPCIONAISS***************************************************************************/



void displayAnimado() {
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.drawRect(12, 12, 30, 30);//----------------------------------> DESENHA O QUADRADO VAZIO

  display.fillRect(20, 20, 35, 35); //---------------------------------> DESENHA O QUADRADO CHEIO

  for (int a = 1; a < 8; a++)
  {
    display.drawCircle(92, 32, a * 3);//--------------------------------> DESENHA O CIRCULO VAZIO
  }
  display.display();
  delay(3000);
  display.clear();
}

void ajustarHora() {
  digitalWrite(pinLED, !digitalRead(pinLED));
  delay(100);
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, password);
    ntp.forceUpdate();
  }
  WiFi.disconnect();
  delay(100);
  digitalWrite(pinLED, !digitalRead(pinLED));
}

void medirWLAN() {
  /**********************************************************************************************************************************************
     O INDICADOR DE POTÊNCIA DO SINAL RECEBIDO OU, SIMPLESMENTE, RSSI MEDE A FORÇA DO SINAL DE UMA REDE SEM FIO EM UM AMBIENTE, DE ACORDO COM A
     NORMA IEEE 802.11. A ESCALA RSSI AVALIA A INTENSIDADE DO SINAL RECEBIDO, DE FORMA RELATIVA, EM UM DISPOSITIVO. QUANTO MAIOR O VALOR, MAIOR
     É A INTENSIDADE DO SINAL. A ESCALA VARIA ENTRE -120 (SINAL FRACO) E 0 (MAIS FORTE). NORMALMENTE, O SINAL É UTILIZÁVEL QUANDO ACIMA DE  -75,
     SENDO QUE ACIMA DE -50 PODE SER CONSIDERADO PERFEITO.
   **********************************************************************************************************************************************/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  for (int h = 0; h < 30; h++) {
    display.drawString(30, 0, ssid);
    display.drawString(0, 20, "RSSI: " + String((WiFi.RSSI() + 120)) + "%");
    display.drawProgressBar(0, 41, 120, 10, int (WiFi.RSSI() + 120)); //------------>DESENHA A GRÁFICO HORIZONTAL EM BARRA COM A POTÊNCIA DO SINAL
    display.display();
    delay(150);
    display.clear();
  }
  delay(500);
}

void conectarWLAN() {

  //INICIALIZAÇÃO DA CONEXÃO WLAN
  String tentativas;
  int b = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(5);

  while (WiFi.status() != WL_CONNECTED) {
    tentativas += ".";
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawXbm (34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.drawString(b, 32, tentativas);
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
    tentativas = "";
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "Wi-Fi ");
    display.drawString(10, 15, "conectado!");
    display.drawString(0, 35, WiFi.localIP().toString());
    display.display();
    delay(2000);
    display.clear();
    display.drawString(0, 0, "GATEWAY:");
    display.drawString(25, 15, WiFi.gatewayIP().toString());
    display.drawString(0, 30, "SUBMÁSCARA:");
    display.drawString(5, 45, WiFi.subnetMask().toString());
    display.display();
    delay(2000);
    display.clear();

    medirWLAN();
  }
}

void setup() {

  pinMode(pinLED, OUTPUT); //---------------------------------> DEFINE O MODO DE OPERAÇÃO DO PINO D0 COMO SAÍDA
  delay(5);
  digitalWrite(pinLED, HIGH);  //-----------------------------> DEFINE O ESTADO DO LED COMO DESLIGADO
  pinMode(pinDOWN, INPUT_PULLUP); //---------------------------------> DEFINE O MODO DE OPERAÇÃO DO PINO D5 COMO ENTRADA
  delay(5);
  digitalWrite(pinDOWN, HIGH);  //-----------------------------> DEFINE O ESTADO DO BOTÃO "PARA CIMA" COMO DESLIGADO
  delay(5);
  pinMode(pinUP, INPUT_PULLUP); //---------------------------------> DEFINE O MODO DE OPERAÇÃO DO PINO D6 COMO ENTRADA
  delay(5);
  digitalWrite(pinUP, HIGH);  //-----------------------------> DEFINE O ESTADO DO BOTÃO "PARA BAIXO" COMO DESLIGADO
  delay(5);
  pinMode(pinCENTER, INPUT_PULLUP); //---------------------------------> DEFINE O MODO DE OPERAÇÃO DO PINO D7 COMO ENTRADA
  delay(5);
  digitalWrite(pinCENTER, HIGH);  //-----------------------------> DEFINE O ESTADO DO BOTÃO CENTRAL COMO DESLIGADO
  delay(5);
  pinMode(pinBuzzer, OUTPUT); //---------------------------------> DEFINE O MODO DE OPERAÇÃO DO PINO D9 COMO SAÍDA
  delay(5);
  digitalWrite(pinBuzzer, HIGH);  //-----------------------------> DEFINE O ESTADO DO LED COMO DESLIGADO
  delay(5);
  pinMode(pinDHT, INPUT); //---------------------------------> DEFINE O MODO DE OPERAÇÃO DO PINO D10 COMO ENTRADA
  delay(5);
  digitalWrite(pinDHT, LOW);  //-----------------------------> DEFINE O ESTADO DO LED COMO LIGADO

  //DEFINIÇÃO DO OLEDDISPLAY UI
  ui.setTargetFPS(60); //-------------------------------------> DEFINE A QUANTIDADE DE QUADROS POR SEGUNDO
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);
  ui. setIndicatorPosition(TOP); //---------------------------> RECEBE OS VALORES LEFT, RIGHT TOP, BOTTOM

  ui.setIndicatorDirection(RIGHT_LEFT);
  ui.setFrameAnimation(SLIDE_RIGHT);
  ui.setFrames(frames, frameContador);
  ui.setOverlays(overlays, overlaysContador);
  ui.init();

  display.init(); //------------------------------------------> INICIALIZA O DISPLAY OLED EMBUTIDO NA PLACA
  delay(5);
  displayAnimado();
  delay(5);
  conectarWLAN(); //-------------------------------------------> EXECUTA A FUNÇÃO DE MESMO NOME

  dht.begin();  //--------------------------------------------->INICIALIZA O SENSOR DHT22
  delay(5);
  ntp.begin(); //---------------------------------------------->INICIALIZA O OBJETO NTP
  delay(5);
  ntp.update(); //---------------------------------------------->ABRE REQUISIÇÃO AO SERVIDOR NTP
  delay(5);

}

void buscarWLAN(){
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display. drawString(0, 10, "BUSCANDO...");
  int redes = WiFi.scanNetworks();
  display.display();
  delay(1000);
  display.clear();

  for (int l = 0; l < redes; l++)
  {
    display.drawString(0, 5, "WLAN: " + String(WiFi.SSID(l)));
    display.drawString(0, 21, "RSSI: " + String(WiFi.RSSI(l) + 115) + "%");
    //DESENHA A BARRA DE PROGRESSO
    display.drawProgressBar(0, 41, 120, 10, int (WiFi.RSSI(l) + 115));
    display.display();
    delay(3000);
    display.clear();
  }
}

void tocar(char* musica[], int tempo[]) {
  int tom = 0;
  for (int i = 0; musica[i] != "Fim"; i++) {
    if (musica[i] == "Do") tom = 262;
    if (musica[i] == "Re") tom = 294;
    if (musica[i] == "Mi") tom = 330;
    if (musica[i] == "Fa") tom = 349;
    if (musica[i] == "Sol") tom = 392;
    if (musica[i] == "La") tom = 440;
    if (musica[i] == "Si") tom = 494;
    if (musica[i] == "Do#") tom = 528;
    if (musica[i] == "Re#") tom = 622;
    if (musica[i] == "Fa#") tom = 370;
    if (musica[i] == "Sol#") tom = 415;
    if (musica[i] == "La#") tom = 466;
    if (musica[i] == "Pausa") tom = 0;
    tone(pinBuzzer, tom, tempo[i]);
    delay(tempo[i]);
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

  remainingTime = ui.update();
  if (remainingTime > 0) {
    delay(remainingTime);
  }
  if (agora == "07:00:00")   {
    tocar(musica, tempo);
  }
  if (millis() - intervalo > 3600000) {
    ajustarHora();
    intervalo = millis();
  }
  if (digitalRead(pinUP) == LOW) {
     buscarWLAN();
  }
  if (digitalRead(pinDOWN) == LOW) {
   exibirSMS();
  }
  if (digitalRead(pinCENTER) == LOW) {
    medirWLAN();
  }

}

