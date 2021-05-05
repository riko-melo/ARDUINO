/******************************************************************************************************
 ***********************************Copyright (c) 2018 by Higo Melo************************************
 ******************************************************************************************************
   ESTE CÓDIGO É CEDIDO, DE FORMA GRATUITA E NÃO EXCLUSIVA, PARA USO A QUEM POSSA INTERESSAR, RESSALVADOS
   OS DIREITOS AUTORAIS E DE COMERCIALIZAÇÃO. AO USAR O CÓDIGO, PARCIAL OU TOTALMENTE, VOCÊ CONCORDA
   OS TERMOS DESSA LICENÇA PARA NÃO ALUGAR, ARRENDAR, SUBLICENCIAR, DISTRIBUIR, DIVULGAR, FORNECER,
   TRANSFERIR, A TITULO ONEROSO, A QUALQUER TERCEIRO O PRESENTE CÓDIGO.
 ******************************************************************************************************
 ***************************************ISENÇÃO DE GARANTIA********************************************
 ******************************************************************************************************
   O TITULAR DOS DIREITOS AUTORAIS NÃO CONFERE QUALQUER TIPO DE GARANTIA SOBRE O PRODUTO, CONTRATUAL
   OU LEGAL, BEM COMO, EM NENHUMA CIRCUNSTANCIA, TEM OBRIGAÇÃO DE MODIFICAR, ATUALIZAR, CORRIGIR DE
   ERROS, MAS PODE FAZÊ-LO A SEU CRITÉRIO EXCLUSIVO.
 ******************************************************************************************************
 ***********************************EXCLUSÃO DE RESPONSABILIDADE***************************************
 ******************************************************************************************************
   O AUTOR NÃO SE RESPONSABILIZA POR QUASIQUER DANOS ESPECIAIS, DIRETOS OU INDIRETOS RELACIONADOS COM
   ESTE ACORDO E / OU COM O PRODUTO, INCLUINDO, SEM LIMITAÇÃO, PERDA DE LUCROS, PERDA DE EQUIPAMENTOS
   E DADOS.
   O USUÁRIO RECONHECE QUE LEU ESTE CONTRATO E CONCORDA COM TODOS OS SEUS TERMOS, COMPROMETENDO-SE
   AO FIEL E BOM CUMPRIMENTO DESTE INSTRUMENTO.
 ******************************************************************************************************
   Embutido na placa, há um display OLED 0,96", 128x64, monocromático. Este possui comunicação I2C e
   está ligado ao ESP8266 Wemos.
 ******************************************************************************************************
 ****************************DECLARAÇÃO DE BIBLIOTECAS*************************************************
 ******************************************************************************************************/
 /******************************************************************************************************
 ****************************DECLARAÇÃO DE BIBLIOTECAS*************************************************
 ******************************************************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>/*****************************************************************************************************
 *********************************CONFIGURAÇÃO DO TELEGRAM********************************************
 *****************************************************************************************************/
#define BOTtoken "628987865:AAErowZo2_gCthCyIJoCJgrjOpPmQKg48ac"
WiFiClientSecure client;
UniversalTelegramBot robot(BOTtoken, client);
String telegram, id, text;
#define pinLED 16
unsigned long tempo;
void setup() {
  //INICIALIZAÇÃO DO MONITOR SERIAL E DEFINIÇÃO DA VELOCIDADE
  Serial.begin(115200);
  delay(5);
 
  //DEFINIÇÃO DE PINAGEM
 pinMode(pinLED, OUTPUT);
  delay(5);
  digitalWrite(pinLED, HIGH);
  delay(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - tempo > 10000) {
      readTelegram ();
      tempo = millis();
    }
}

void chamarTelegram() {

  int sms = bot.getUpdates(bot.last_message_received + 1);
  Serial.println(sms);

  for (int y = 0; y < sms; y++) {
     id = bot.messages[y].chat_id; //armazena o ID do usuário que enviou a mensagem
     if (id == mybot) {
    text = bot.messages[y].text;
    text.toUpperCase();
       
  if (text.indexOf("ON") > -1) //Confere o texto recebido "ON" do Telegram
    {
      digitalWrite(pinLED, HIGH);
      bot.sendMessage(id, "LED ON", ""); //Envia a mensagem para a pessoa
    }

  else if (text.indexOf("OFF") > -1) {
    digitalWrite(pinLED, LOW);
    bot.sendMessage(id, "LED OFF", "");
  }

  else if (text.indexOf("START") > -1) {
    bot.sendSimpleMessage(id, id, "");
  }

  else {
    bot.sendSimpleMessage(id, "Comando Invalido", "");
  }
}
else{
  bot.sendSimpleMessage(id, "Usuário não autorizado", "");
  }
  }
}
