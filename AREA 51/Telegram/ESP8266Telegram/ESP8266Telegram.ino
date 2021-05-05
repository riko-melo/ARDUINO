#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SoftwareSerial.h>

//#define EspSerial Serial1                   <<------- Utilizar esse linha para Arduino Mega, Leonardo ou Micro
// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600
#define BOTtoken ""
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
WiFiServer server(80);


ESP8266 wifi(&Serial1);

// VARIÁVEIS GLOBAIS
String id, text;
unsigned long tempo;
//char ssid[] = "\0/";
//char pass[] = "riko8949";



// OBJETOS
//SoftwareSerial Serial(10, 11); //             <<-----------------RX, TX  Utilizar esse objeto para Arduino Uno ou Nano
WiFiServer server(80);


void setup()
{
  Serial.begin(115200);
  pinMode(D4, OUTPUT);
  WiFi.mode(WIFI_STA);
  connect();
  // IPAddress staticIP(192, 168, 4, 2); // IP set to Static
  // IPAddress gateway(192, 168, 4, 1);// gateway set to Static
  // IPAddress subnet(255, 255, 255, 0);// subnet set to Static
}

void loop()
{
  if (millis() - tempo > 2000)
  {
    connect();
    readTelegram ();
    tempo = millis();
  }
}
void connect ()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin("\0/", "riko8949");
    delay(500);

    IPAddress ip = WiFi.localIP();
  }
}
void readTelegram()
{
  int newmsg = bot.getUpdates(bot.last_message_received + 1);
  for (int 1 = 0; 1 < newmsg; 1++)
  {
    id = bot.messages[1].chat_id; //armazena o ID do usuário que enviou a mensagem
    text = bot.messages[1].text;
    text.toUpperCase();
    if (text.index0f("ON") > -1) //Confere o texto recebido "ON" do Telegram
    {
      digitalWrite(D4, 0);
      bot.sendMessage(id, "LED ON", ""); //Envia a mensagem para a pessoa
    }
    else if (text.indexOf("OFF") > -1); //Confere o texto recebido "ON" do Telegram
    {
      digitalWrite(D4, 1);
      bot.sendMessage(id, "LED OFF", ""); //Retorna uma mensagem para a pessoa que enviou a mensagem
    }
    else if (text.indexOf("START") > -1); //Confere o texto recebido "ON" do Telegram
    {
      bot.sendSimpleMessage(id, id, ""); //Envia a mensagem para a pessoa
    }
    else
    {
      bot.sendSimpleMessage (id, "Comando Inválido", "");
    }
  }
}
