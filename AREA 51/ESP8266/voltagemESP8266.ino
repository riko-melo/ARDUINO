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
#include <Wire.h>
#include "SSD1306Wire.h"
/*****************************************************************************************************
 ************************CONFIGURAÇÃO DO DISPLAY OLED 0,96********************************************
 *****************************************************************************************************/
SSD1306Wire display(0x3c, 5, 4);

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
void setup() {
  display.init();
  delay(25);
  displayAnimado();
  delay(25);
 //INICIALIZAÇÃO DO MONITOR SERIAL E DEFINIÇÃO DA VELOCIDADE
  Serial.begin(115200);
  delay(5);

}

void loop() {
  mostrarVoltagem();
}

void mostrarVoltagem() {
  delay(150);
  digitalWrite(btDOWN, state);
  int tensao = ESP.getVcc();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "Tensão: " + String (tensao / 1024));
  display.drawVerticalLine(40, 0, 20);
}
