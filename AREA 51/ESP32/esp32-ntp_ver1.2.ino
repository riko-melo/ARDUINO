/******************************************************************************************
   COPYRIGHT (c) 2019  HIGO MELO
 *                                                                                        *
        O SOFTWARE É FORNECIDO GRATUITAMENTE, SEM GARANTIA DE QUALQUER TIPO, EXPRESSA OU
   IMPLÍCITA, DE GARANTIA, RESSALVADAS ÀS GARANTIAS DE COMERCIALIZAÇÃO E NÃO VIOLAÇÃO DOS
   DIREIOTS AUTORAIS. EM NENHUMA CIRCUNSTÂNCIA, O TITULAR DOS DIREITOS AUTORAIS PODE SER
   RESPONSABILIZADO POR QUAISQUER DANOS OU UTILIZAÇÃO INDEVIDA DESTE.
   https://www.curtocircuito.com.br/blog/protocolo-ntp-para-ESP32/
 *                                                                                        *
*****************************************************************************************/
#include <WiFi.h>
#include <NTPClient.h>

//-------- PARAMETROS DA CONEXAO WIRELESS-----------
const char* ssid = ")o,0(";
const char* password = "riko1234";
int pinLED = 2;

//-------- PARAMETROS DO RELOGIO NTP-----------
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
String horaCerta;
//int portUDP = 3333;

unsigned long tempo = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, 1);

  //----INICIALIZACAO DA CONEXAO ----
  WiFi.begin(ssid, password);
  Serial.printf("Conectando em %s ", ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(500);                                 // Aguarda meio segundo
    digitalWrite(pinLED, !digitalRead(pinLED));
  }
  Serial.println("Esp32 conectado!"); // Conectado
  ntp.begin();
  ntp.forceUpdate();
}

void startClock()
{
  horaCerta = ntp.getFormattedTime();
  Serial.print("HORA: ");
  Serial.print(horaCerta);
  delay(500);
}

void loop()
{
  startClock();
  Serial.println();
  delay(1000);
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    delay(1000);
    while (millis() - tempo > 300000)
    {
      if (!ntp.update())
      {
        ntp.forceUpdate();
      }
   horaCerta = ntp.getFormattedTime();
      for (int x = 0; x < 6; x++)
      {
        digitalWrite(pinLED, 1);
        delay(50);
        digitalWrite(pinLED, 0);
      }
   
      tempo = millis();
    }

  }
  else
  {
    Serial.println("Verifique a conexão com a Internet!");
    Serial.println("A hora pode estar imprecisa!");
    delay(1000);
    return;
    //ESP.restart();
  }


}
