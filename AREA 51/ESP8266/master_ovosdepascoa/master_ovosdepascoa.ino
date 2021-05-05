/**********************************************************************************************************
*                                    COPYRIGHT (c) 2018 by Higo Melo                                      *
*                                              25/05/2019                                                 *
===========================================================================================================
*                                           TERMOS DA LICENÇA
===========================================================================================================
*  ESTE CÓDIGO É CEDIDO, DE FORMA GRATUITA E NÃO EXCLUSIVA, PARA USO A QUEM POSSA INTERESSAR, RESSALVADOS *
*  OS DIREITOS AUTORAIS E DE COMERCIALIZAÇÃO. AO USAR O CÓDIGO, PARCIAL OU TOTALMENTE, VOCÊ CONCORDA      *
*  OS TERMOS DESSA LICENÇA PARA NÃO ALUGAR, ARRENDAR, SUBLICENCIAR, DISTRIBUIR, DIVULGAR, FORNECER,       *
*  TRANSFERIR, A TITULO ONEROSO, A QUALQUER TERCEIRO O PRESENTE OBJETO.                                   *
*                                          ISENÇÃO DE GARANTIA                                            *
*  O TITULAR DOS DIREITOS AUTORAIS NÃO CONFERE QUALQUER TIPO DE GARANTIA SOBRE O PRODUTO, CONTRATUAL      *
*  OU LEGAL, BEM COMO, EM NENHUMA CIRCUNSTANCIA, TEM OBRIGAÇÃO DE MODIFICAR, ATUALIZAR, CORRIGIR DE       *
*  ERROS, MAS PODE FAZÊ-LO A SEU CRITÉRIO EXCLUSIVO.                                                      *
*                                      EXCLUSÃO DE RESPONSABILIDADE                                       *
*  O AUTOR NÃO SE RESPONSABILIZA POR QUASIQUER DANOS ESPECIAIS, DIRETOS OU INDIRETOS RELACIONADOS COM ESTE*
*  ESTE ACORDO E/OU COM O PRODUTO, INCLUINDO, SEM LIMITAÇÃO, PERDA DE LUCROS, DE EQUIPAMENTOS E DE DADOS. *
*  O USUÁRIO RECONHECE QUE LEU ESTE CONTRATO E CONCORDA COM TODOS OS SEUS TERMOS, COMPROMETENDO-SE AO FIEL*
*  BOM CUMPRIMENTO DESTE INSTRUMENTO.                                                                     *
***********************************************************************************************************
***********************************************************************************************************
                                        DECLARAÇÃO DE BIBLIOTECAS
**********************************************************************************************************/
 /#include <ESP8266WiFi.h>  //ESP8266
 
#include <WiFi.h> //ESP32
#include <Wire.h>
#include <SSD1306Wire.h> //-----------> EMBUTIDO NA PLACA, HÁ UM DISPLAY OLED 0,96", 128x64, MONOCROMÁTICO. 
// Nome da rede e senha
const char* ssid = "Ovodepascoa";
const char* senha = "segredo123";
 
  
// Endereços para configuração da rede 
IPAddress ip(192, 168, 0, 2);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
 
//Inicializa o display
SSD1306Wire display(0x3C, 5, 4);
 
 
void setup() {
  delay(1000);
// Você pode remover o parâmetro senha se quiser que sua rede seja aberta
 
  WiFi.softAP(ssid, senha);
   
// configurações da rede
  WiFi.softAPConfig(ip, gateway, subnet);
  
  IPAddress myIP = WiFi.softAPIP();
 
// Mostra no display a palavra Ovo
  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(63, 12, "Ovo");
  display.display();
  
}
  
void loop() { // Nao tem nada no loop mesmo, so espera ser encontrado
}
