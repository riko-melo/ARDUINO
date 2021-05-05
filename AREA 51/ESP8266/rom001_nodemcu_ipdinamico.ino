/******************************************************************************************************
   ESTE CÓDIGO É CEDIDO, DE FORMA GRATUITA E NÃO EXCLUSIVA, PARA USO A QUEM INTERESSAR, RESSALVADOS
   OS DIREITOS AUTORAIS E DE COMERCIALIZAÇÃO. AO USAR O CÓDIGO, PARCIAL OU TOTALMENTE, VOCÊ CONCORDA
   OS TERMOS DESSA LICENÇA PARA NÃO ALUGAR, ARRENDAR, SUBLICENCIAR, DISTRIBUIR, DIVULGAR, FORNECER,
   TRANSFERIR, A TITULO ONEROSO, A QUALQUER TERCEIRO O PRESENTE CÓDIGO.
 * ****************************************************************************************************
 * *************************************ISENÇÃO DE GARANTIA********************************************
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
   OA FIEL E BOM CUMPRIMENTE DESTE INSTRUMENTO.
 ******************************************************************************************************
   Embutido na placa, há um display OLED 0,96", 128x64, monocromático. Este possui comunicação I2C e
   está ligado ao ESP8266 Wemos.
 ******************************************************************************************************
 ****************************DECLARAÇÃO DE BIBLIOTECAS*************************************************
 ******************************************************************************************************/
#include <ESP8266WiFi.h>
/*****************************************************************************************************
 *******************************CONFIGURAÇÃO DO SENSOR DHT11******************************************
 *****************************************************************************************************/
#define pinLED 16
/*****************************************************************************************************
 *******************************CONFIGURAÇÃO DA CONEXÃO WI-FI******************************************
 *****************************************************************************************************/
const char* ssid     = ")o,0(";
const char* password = "riko1234";

void setup() {

  Serial.begin(115200);//------------>INICIALIZAÇÃO DO MONITOR SERIAL E DEFINIÇÃO DA VELOCIDADE
  delay(5);
  pinMode(pinLED, OUTPUT);//-----------> DEFINE O LED COMO O PINO AO QUAL ESTÁ CONECTADO O LED COMO SAÍDA
  delay(5);
  digitalWrite(pinLED, HIGH);//------> DESLIGA O LED
  delay(5);
  Serial.println();
  conectarWLAN();
}

void conectarWLAN() {
  WiFi.mode(WIFI_STA);//--------------------> DEFINIÇÃO DO MODO DE OPERAÇÃO DO RÁDIO COMO ESTAÇÃO
  WiFi.begin(ssid, password); //------------->INICIALIZAÇÃO DA CONEXÃO WLAN
  Serial.println();
  int a = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    
    Serial.print("CONECTANDO...");
    Serial.print(".");
    a++;
    delay(500);
    if (a == 30)
    {
      ESP.reset();
      
    }
  Serial.println();
  Serial.println("Done!");
  delay(1000);
  Serial.println("Wi-Fi connected!");
  delay(1000);
  
}
}

void loop(){
Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(2000);
  Serial.print("Máscara de rede: ");
  Serial.println(WiFi.subnetMask());
  delay(2000);
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  delay(2000);
 }
