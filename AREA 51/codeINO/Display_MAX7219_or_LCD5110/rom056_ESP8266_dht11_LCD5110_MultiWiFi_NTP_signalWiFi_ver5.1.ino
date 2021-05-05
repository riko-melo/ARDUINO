
/******************************************************************************************
   COPYRIGHT (c) 2019  HIGO MELO
 *                                                                                        *
        O SOFTWARE É FORNECIDO GRATUITAMENTE, SEM GARANTIA DE QUALQUER TIPO, EXPRESSA OU
   IMPLÍCITA, DE GARANTIA, RESSALVADAS ÀS GARANTIAS DE COMERCIALIZAÇÃO E NÃO VIOLAÇÃO DOS
   DIREIOTS AUTORAIS. EM NENHUMA CIRCUNSTÂNCIA, O TITULAR DOS DIREITOS AUTORAIS PODE SER
   RESPONSABILIZADO POR QUAISQUER DANOS OU UTILIZAÇÃO INDEVIDA DESTE.
 *                                                                                        *
        Este sketch é destinado ao nodeMCU v1.0 com display LCD monocromático (sem tons de
   cinza) conhecido como "Nokia 5110" com controlador    Adafruit_PCD8544, utilizando
   ainda    protocolo SPI para comunicação, através de 5 pinos, porém dá para eliminar
   um e usar apenas os 4 pinos da SPI.
        Não é reomendado a alteração do tipo de variaveis acima sob pena de erro compilando
   para a placa ESP8266U em função da biblioteca adafruit_PCD8544, então siga o esquema de*
   ligação abaixo para o LCD 5110 (cor vermelha):                                         *
   PINAGEM       LCD         ESP-12E                                                      *
   Pino 1 -----> RST ----->   D2 (GPIO4) - Saída do ESP para redefinir a exibição         *
   Pino 2 -----> CE  ----->   D5 (GPIO11) - Saída do ESP para selecionar chip / ativar exibição
   Pino 3 -----> DC  ----->   D6 (GPIO12) (MISO) - Saída dos dados de exibição / comando para ESP
   Pino 4 -----> DIN ----->   D7 (GPIO13) (MOSI) - DIN do ESP SPI MOSI para exibir entrada de dados
   Pino 5 -----> CLK ----->   D8(GPIO14) - saídas CLK do relógio ESP SPI
   Pino 6 -----> VCC ----->   3.3V - Alimentação do LCD para ligar o LCD
   Pino 7 -----> LIGHT ----->  GND
   Pino 8 -----> GND  ----->  GND*
 *                                                                                        *
   A resolução é de 84×48 pixels.
 *****************************************************************************************/

/*
   A biblioteca Adafruit_GFX deve antecede a biblioteca Adafruit_PCD8544 sob pena de erro na compilação do programa.
   Inspirado no modelo disponível em https://github.com/michielvanhoorn/thingspeak-nodemcu-with-nokia-5110-LCD/blob/master/thingspeak.ino
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


//Cria uma instancia da classe Adafruit_PCD8544 para enviar dados ao LCD
Adafruit_PCD8544 display = Adafruit_PCD8544(D8, D7, D6, D5, D2);
/******************************************(CLK, DIN, DC, CE, RST)*/
const unsigned char Arduino_Logo[] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 192,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 192,
  0, 0, 3, 128, 0, 0, 0, 56, 0, 9, 64,
  0, 0, 255, 255, 0, 0, 15, 255, 224, 0, 0,
  0, 7, 255, 255, 224, 0, 127, 255, 252, 0, 0,
  0, 31, 248, 31, 248, 3, 255, 129, 255, 0, 0,
  0, 63, 128, 1, 254, 7, 248, 0, 63, 192, 0,
  0, 126, 0, 0, 127, 31, 192, 0, 15, 224, 0,
  0, 252, 0, 0, 31, 255, 128, 0, 3, 240, 0,
  1, 248, 0, 0, 15, 254, 0, 48, 1, 240, 0,
  1, 240, 0, 0, 7, 252, 0, 48, 1, 248, 0,
  1, 240, 31, 248, 1, 248, 3, 255, 1, 248, 0,
  1, 240, 0, 0, 3, 248, 0, 48, 1, 248, 0,
  1, 248, 0, 0, 7, 254, 0, 48, 1, 240, 0,
  1, 248, 0, 0, 15, 255, 0, 0, 3, 240, 0, 0,
  252, 0, 0, 63, 191, 128, 0, 7, 224, 0, 0,
  127, 0, 0, 255, 15, 224, 0, 15, 192, 0, 0,
  63, 192, 7, 252, 7, 252, 0, 127, 128, 0, 0, 15,
  255, 255, 240, 1, 255, 255, 254, 0, 0, 0, 3, 255,
  255, 192, 0, 63, 255, 248, 0, 0, 0, 0, 63, 252,
  0, 0, 3, 255, 128, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 12, 31, 7, 131, 8,
  254, 49, 131, 0, 0, 0, 30, 24, 196, 115, 8,
  24, 57, 140, 192, 0, 0, 54, 24, 196, 51, 8,
  16, 45, 152, 96, 0, 0, 51, 31, 132, 51, 8, 16,
  39, 152, 96, 0, 0, 99, 25, 132, 99, 24, 24, 35,
  140, 192, 0, 0, 65, 24, 193, 0, 224, 198, 32, 131,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0
};

const unsigned char smiley_sonolento[] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 255, 255, 240, 0, 0,
  0, 0, 0, 0, 0, 63, 255, 255, 255,
  0, 0, 0, 0, 0, 0, 1, 255, 255, 255,
  255, 240, 0, 0, 0, 0, 0, 15, 254, 0,
  0, 15, 254, 0, 0, 0, 0, 0, 63, 224,
  0, 0, 0, 255, 128, 0, 0, 0, 0, 254,
  0, 0, 0, 0, 31, 224, 0, 0, 0, 3, 248,
  0, 0, 0, 0, 3, 248, 0, 0, 0, 15, 224,
  0, 0, 0, 0, 0, 252, 0, 0, 0, 31, 128,
  0, 0, 0, 0, 0, 63, 0, 0, 0, 63, 0, 0,
  0, 0, 0, 0, 31, 128, 0, 0, 124, 0, 0,
  0, 0, 0, 0, 7, 192, 0, 0, 248, 0, 0,
  0, 0, 0, 0, 3, 224, 0, 1, 240, 0, 0,
  0, 0, 0, 0, 1, 240, 0, 3, 224, 0, 0,
  0, 0, 0, 0, 0, 248, 0, 3, 224, 0, 63,
  0, 0, 31, 128, 0, 248, 0, 7, 192, 0,
  127, 128, 0, 63, 192, 0, 124, 0, 7, 128,
  0, 127, 192, 0, 63, 224, 0, 124, 0, 15, 128,
  0, 63, 128, 0, 63, 192, 0, 60, 0, 15, 128,
  0, 30, 0, 0, 15, 0, 0, 62, 0, 15, 128, 0, 0,
  0, 0, 0, 0, 0, 62, 0, 15, 0, 0, 0, 0, 0, 0,
  0, 0, 62, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 62,
  0, 15, 128, 0, 0, 0, 0, 0, 0, 0, 62, 0, 15,
  128, 0, 0, 0, 0, 0, 0, 0, 62, 0, 15, 128, 0,
  0, 0, 0, 0, 0, 0, 60, 0, 7, 128, 0, 192, 0,
  0, 0, 64, 0, 124, 0, 7, 192, 3, 224, 0, 0,
  0, 248, 0, 124, 0, 3, 192, 1, 248, 0, 0, 3,
  240, 0, 248, 0, 3, 224, 0, 254, 0, 0, 31, 224,
  0, 248, 0, 1, 240, 0, 63, 224, 0, 255, 128, 1,
  240, 0, 0, 248, 0, 15, 255, 255, 254, 0, 3, 224,
  0, 0, 124, 0, 1, 255, 255, 240, 0, 7, 192, 0, 0,
  63, 0, 0, 15, 254, 0, 0, 31, 128, 0, 0, 31, 128,
  0, 0, 0, 0, 0, 63, 0, 0, 0, 15, 224, 0, 0, 0, 0,
  0, 252, 0, 0, 0, 3, 248, 0, 0, 0, 0, 3, 248, 0,
  0, 0, 0, 254, 0, 0, 0, 0, 31, 224, 0, 0, 0, 0,
  63, 224, 0, 0, 0, 255, 128, 0, 0, 0, 0, 15, 254,
  0, 0, 15, 254, 0, 0, 0, 0, 0, 1, 255, 255, 255,
  255, 240, 0, 0, 0, 0, 0, 0, 63, 255, 255, 255,
  0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 240, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const unsigned char smiley_contente [] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 7, 254, 0, 0, 0, 0, 0,
  0, 0, 0, 3, 255, 255, 252, 0, 0, 0, 0, 0, 0,
  0, 63, 255, 255, 255, 192, 0, 0, 0, 0, 0, 1,
  255, 224, 0, 127, 248, 0, 0, 0, 0, 0, 7, 252,
  0, 0, 3, 255, 0, 0, 0, 0, 0, 63, 192, 0, 0, 0,
  63, 192, 0, 0, 0, 0, 127, 0, 0, 0, 0, 15, 224,
  0, 0, 0, 1, 248, 0, 0, 0, 0, 1, 248, 0, 0, 0,
  3, 240, 0, 0, 0, 0, 0, 124, 0, 0, 0, 15, 192,
  0, 0, 0, 0, 0, 63, 0, 0, 0, 31, 128, 0, 0, 0,
  0, 0, 15, 128, 0, 0, 62, 0, 0, 0, 0, 0, 0, 7,
  192, 0, 0, 124, 0, 0, 0, 0, 0, 0, 3, 224, 0, 0,
  124, 0, 0, 0, 0, 0, 0, 1, 224, 0, 0, 248, 0, 63,
  128, 0, 31, 192, 1, 240, 0, 0, 240, 0, 255, 240,
  0, 255, 240, 0, 248, 0, 1, 240, 1, 255, 248, 1,
  255, 248, 0, 248, 0, 1, 224, 3, 224, 120, 1, 224,
  124, 0, 120, 0, 1, 224, 3, 192, 124, 3, 224, 60,
  0, 124, 0, 3, 224, 0, 0, 0, 0, 0, 0, 0, 124, 0, 3,
  224, 0, 0, 0, 0, 0, 0, 0, 124, 0, 3, 224, 0, 0, 0,
  0, 0, 0, 0, 60, 0, 3, 224, 0, 0, 0, 0, 0, 0, 0, 124,
  0, 3, 224, 0, 0, 0, 0, 0, 0, 0, 124, 0, 1, 224, 0, 0,
  0, 0, 0, 0, 0, 120, 0, 1, 240, 0, 32, 0, 0, 0, 64, 0,
  120, 0, 1, 240, 0, 240, 0, 0, 0, 248, 0, 248, 0, 0,
  248, 0, 252, 0, 0, 3, 240, 0, 240, 0, 0, 120, 0, 63,
  0, 0, 15, 224, 1, 240, 0, 0, 124, 0, 31, 240, 0, 255,
  128, 3, 224, 0, 0, 62, 0, 7, 255, 255, 254, 0, 7, 192,
  0, 0, 31, 0, 0, 255, 255, 240, 0, 15, 128, 0, 0, 15,
  192, 0, 3, 254, 0, 0, 31, 0, 0, 0, 7, 224, 0, 0, 0, 0,
  0, 126, 0, 0, 0, 1, 248, 0, 0, 0, 0, 1, 252, 0, 0, 0,
  0, 254, 0, 0, 0, 0, 7, 240, 0, 0, 0, 0, 63, 128, 0, 0,
  0, 31, 192, 0, 0, 0, 0, 15, 240, 0, 0, 0, 255, 0, 0,
  0, 0, 0, 3, 255, 128, 0, 31, 252, 0, 0, 0, 0, 0, 0,
  127, 255, 255, 255, 224, 0, 0, 0, 0, 0, 0, 7, 255, 255,
  255, 0, 0, 0, 0, 0, 0, 0, 0, 63, 255, 192, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const unsigned char smiley_sorridente [] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 255, 128,
  0, 0, 0, 0, 0, 0, 0, 7, 255, 255, 254, 0, 0, 0,
  0, 0, 0, 0, 127, 255, 255, 255, 224, 0, 0, 0, 0,
  0, 3, 255, 192, 0, 63, 252, 0, 0, 0, 0, 0, 15, 248,
  0, 0, 1, 255, 0, 0, 0, 0, 0, 127, 128, 0, 0, 0, 31,
  224, 0, 0, 0, 0, 254, 0, 0, 0, 0, 7, 240, 0, 0, 0,
  3, 248, 0, 0, 0, 0, 1, 252, 0, 0, 0, 7, 224, 0, 0,
  0, 0, 0, 126, 0, 0, 0, 31, 128, 0, 0, 0, 0, 0, 31,
  128, 0, 0, 63, 0, 0, 0, 0, 0, 0, 15, 192, 0, 0, 126,
  0, 0, 0, 0, 0, 0, 7, 224, 0, 0, 248, 0, 0, 0, 0, 0,
  0, 3, 240, 0, 0, 248, 0, 0, 0, 0, 0, 0, 1, 240, 0, 1,
  240, 0, 6, 0, 0, 6, 0, 0, 248, 0, 3, 224, 0, 63, 128,
  0, 31, 192, 0, 124, 0, 3, 224, 0, 63, 192, 0, 63, 192,
  0, 124, 0, 3, 192, 0, 63, 192, 0, 63, 192, 0, 60, 0, 7,
  192, 0, 63, 128, 0, 31, 192, 0, 62, 0, 7, 192, 56, 0, 0,
  0, 0, 3, 192, 62, 0, 7, 128, 63, 0, 0, 0, 0, 15, 192, 62,
  0, 7, 128, 63, 128, 0, 0, 0, 31, 192, 30, 0, 7, 192, 7,
  192, 0, 0, 0, 62, 0, 30, 0, 7, 192, 3, 224, 0, 0, 0, 124,
  0, 62, 0, 7, 192, 7, 248, 0, 0, 1, 252, 0, 62, 0, 3, 192,
  15, 255, 0, 0, 15, 255, 0, 60, 0, 3, 224, 63, 191, 248, 1,
  255, 223, 192, 124, 0, 3, 224, 62, 15, 255, 255, 255, 7,
  192, 124, 0, 1, 240, 32, 0, 255, 255, 240, 0, 64, 248, 0,
  0, 248, 0, 0, 3, 252, 0, 0, 1, 240, 0, 0, 252, 0, 0, 0, 0,
  0, 0, 1, 240, 0, 0, 126, 0, 0, 0, 0, 0, 0, 7, 224, 0, 0,
  63, 0, 0, 0, 0, 0, 0, 15, 192, 0, 0, 31, 128, 0, 0, 0, 0,
  0, 31, 128, 0, 0, 7, 224, 0, 0, 0, 0, 0, 126, 0, 0, 0, 3,
  248, 0, 0, 0, 0, 1, 252, 0, 0, 0, 0, 254, 0, 0, 0, 0, 7,
  240, 0, 0, 0, 0, 127, 128, 0, 0, 0, 31, 224, 0, 0, 0, 0,
  15, 248, 0, 0, 1, 255, 0, 0, 0, 0, 0, 3, 255, 192, 0, 63,
  252, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 224, 0, 0, 0,
  0, 0, 0, 7, 255, 255, 254, 0, 0, 0, 0, 0, 0, 0, 0, 63,
  255, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const unsigned char smiley_lingua [] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 255, 255, 224, 0, 0, 0,
  0, 0, 0, 0, 31, 255, 255, 255, 0, 0, 0, 0, 0,
  0, 0, 255, 254, 15, 255, 224, 0, 0, 0, 0, 0,
  7, 254, 0, 0, 15, 252, 0, 0, 0, 0, 0, 31, 224,
  0, 0, 0, 255, 0, 0, 0, 0, 0, 127, 0, 0, 0, 0,
  31, 192, 0, 0, 0, 1, 252, 0, 0, 0, 0, 7, 240,
  0, 0, 0, 3, 240, 0, 0, 0, 0, 1, 248, 0, 0, 0,
  7, 192, 0, 0, 0, 0, 0, 124, 0, 0, 0, 31, 128,
  0, 0, 0, 0, 0, 63, 0, 0, 0, 62, 0, 0, 0, 0, 0,
  0, 31, 128, 0, 0, 60, 0, 0, 0, 0, 0, 0, 7, 128,
  0, 0, 124, 0, 0, 0, 0, 0, 0, 7, 192, 0, 0, 248,
  0, 0, 0, 0, 0, 0, 3, 224, 0, 1, 240, 0, 31, 128,
  0, 63, 0, 1, 224, 0, 1, 224, 0, 63, 192, 0, 127,
  128, 1, 240, 0, 3, 224, 0, 63, 192, 0, 127, 128,
  0, 240, 0, 3, 224, 0, 63, 128, 0, 63, 128, 0, 248,
  0, 3, 192, 0, 4, 0, 0, 4, 0, 0, 120, 0, 3, 192, 0,
  0, 0, 0, 0, 0, 0, 120, 0, 3, 192, 0, 0, 0, 0, 0, 0,
  0, 120, 0, 3, 192, 0, 96, 0, 0, 0, 192, 0, 120, 0,
  3, 192, 1, 240, 0, 0, 1, 240, 0, 120, 0, 3, 224, 0,
  252, 0, 0, 7, 224, 0, 248, 0, 3, 224, 0, 127, 0, 0,
  31, 192, 0, 240, 0, 1, 224, 0, 31, 240, 1, 255, 0, 0,
  240, 0, 1, 240, 0, 7, 255, 255, 252, 0, 1, 240, 0, 0,
  240, 0, 0, 255, 255, 224, 0, 3, 224, 0, 0, 248, 0, 0,
  127, 255, 128, 0, 3, 192, 0, 0, 124, 0, 0, 124, 7, 192,
  0, 7, 192, 0, 0, 62, 0, 0, 124, 7, 192, 0, 15, 128, 0,
  0, 31, 0, 0, 124, 7, 128, 0, 31, 0, 0, 0, 15, 192, 0,
  63, 31, 128, 0, 126, 0, 0, 0, 7, 224, 0, 31, 255, 0,
  0, 252, 0, 0, 0, 1, 248, 0, 7, 252, 0, 3, 240, 0, 0,
  0, 0, 254, 0, 0, 64, 0, 15, 224, 0, 0, 0, 0, 63, 192,
  0, 0, 0, 127, 128, 0, 0, 0, 0, 15, 248, 0, 0, 3, 254,
  0, 0, 0, 0, 0, 1, 255, 224, 0, 255, 240, 0, 0, 0, 0,
  0, 0, 63, 255, 255, 255, 128, 0, 0, 0, 0, 0, 0, 3, 255,
  255, 248, 0, 0, 0, 0, 0, 0, 0, 0, 7, 252, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


// biblioteca com funções para o WiFi do ESP6288
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <NTPClient.h>
#include <WiFiUdp.h>



//Cria uma instancia da classe ESP8266WiFiMulti para conectar as redes WiFi
ESP8266WiFiMulti wifiMulti;

WiFiUDP udp;

NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);

// biblioteca com funções para o sensor DHT11
#include <DHT.h>
// Pinagem do sensor DHT11 no ESP8266
#define DHTPIN D3

// Indicação do modelo de sensor, no caso DHT 11
#define DHTTYPE DHT11

//Declaração de objeto dht para receber dados do sensor
DHT dht(D3, DHT11);

float umidade, celsius, fahrenheit;
String tempoAtual;
int force;


void setup()
{
  // Inicialização do LCD
  display.begin();
  display.setContrast(60);  // Adjust for your display

  display.clearDisplay();
  display.drawBitmap(0, 0, Arduino_Logo, 84, 48, 1);
  display.display();
  delay(5000);
  display.clearDisplay();
  display.setCursor(30, 0);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.println("HIGO"); /* Text to be displayed */
  display.setCursor(30, 10);
  display.println("RICARDO");
  display.setCursor(40, 20);
  display.println("DE");
  display.setCursor(20, 30);
  display.println("OLIVEIRA");
  display.setCursor(30, 40);
  display.println("MELO");
  display.display();
  delay(5000);
  display.clearDisplay();
  /* Draw rectangle with round edges. Parameters (x_co-ord,y-co-ord,width,height,color) */
  display.drawRoundRect(05, 15, 15, 25, 15, BLACK);
  /* Draw circle. Parameters (x_co-ord of origin,y-co-ord of origin,radius,color) */
  display.drawCircle(25, 25, 25, BLACK);
  /* Draw triangle. Parameters (x0,y0,x1,y1,x2,y2,width,color). (x0,y0), (x1,y1) and (x2,y2) are the co-ord of vertices of the triangle  */
  display.drawTriangle(50, 28, 60, 15, 70, 28, BLACK);
  display.display();
  delay(5000);
  display.clearDisplay();



  //Inicialização da conexão WiFi
  display.setCursor(0, 5);
  display.print(" CONECTANDO...");
  display.display();
  delay(2000);
  display.clearDisplay();

  //Coloque aqui as redes wifi necessarias
  wifiMulti.addAP(")o,0(", "riko1234");
  wifiMulti.addAP("iPhone de HIGO", "riko2019");

  while (wifiMulti.run() != WL_CONNECTED)
  {
    //Aguarda a conexao da rede wifi
    delay(250);
    Serial.print('.');
    display.clearDisplay();
    display.setCursor(0, 5);
    display.println(".");
    display.display();
    delay(500);

  }

  display.setCursor(0, 15);
  display.println("CONECTADO");
  display.display();
  delay(3000);
  display.clearDisplay();

  display.setCursor(0, 25);
  display.print("REDE: ");
  display.setCursor(0, 30);
  display.print(WiFi.SSID());
  display.setCursor(0, 45);
  display.print(WiFi.localIP());
  display.display();
  delay(5000);
  display.clearDisplay();

  ntp.begin();
  ntp.update();

  // Inicialização do sensor DHT11
  dht.begin();


}

void ajustarHora()
{
  //Lógica de verificação do tempo
  ntp.forceUpdate();
  tempoAtual = ntp.getFormattedTime();
  display.setCursor(15, 5);
  display.setTextSize(1);
  display.println ( "HORA: ");
  display.setCursor(15, 15);
  display.setTextSize(2);
  display.println(tempoAtual);
  display.display();
  delay(200);
  display.clearDisplay();

}
void analyzerWLAN()
{
  force = (WiFi.RSSI());
  display.setCursor(5, 5);
  display.setTextSize(1);
  display.print("REDE: ");
  display.println (WiFi.SSID());
  display.setCursor(0, 15);
  display.setTextSize(2);
  display.print(force);
  display.print("dBm");
  display.display();
  delay(250);
  display.clearDisplay();
}


/****************************************************************************************
      Signal strength (RSSI) is the usable strength of the radio waves, expressed in
  decibels (db) between 0db (strongest) and -120db (weakest). The closer it is to zero,
  the stronger/cleaner the signal is. For WiFi, the normal range is -45db to -87db.
  Anything below -85db is generally unusable, and over -50db can be considered
  perfect.
  RSSI
  Quality > -50db
  Fantastic ~= -50db
  Great ~= -75db
  Average ~= -85db
  Poor ~= -95db

*******************************************************************************************/


void dht11()
{
  /*

    Teste de funcionalidade do DHT11
    if (isnan(umidade) || isnan(celsius) || isnan(fahrenheit) )
    {
     Serial.println(F("Falhou a leitura do sensor DHT!"));
     display.println("Falhou a leitura do sensor DHT!");
     display.display();
     delay(3000);
     display.clearDisplay();
     return
    }
  */
  celsius = dht.readTemperature();
  display.setTextSize(1);
  display.setCursor(10, 5);
  display.println ("TEMPERATURA: ");
  display.setCursor(0, 15);
  display.setTextSize(2);
  display.print(celsius);
  display.print(char(247));
  display.print("C");
  display.display();
  delay(10000);
  display.clearDisplay();


  umidade = dht.readHumidity();
  display.setTextSize(1);
  display.setCursor(15, 5);
  display.println ("UMIDADE: ");
  display.setCursor(0, 15);
  display.setTextSize(2);
  display.print(umidade);
  display.print("%");
  display.display();
  delay(10000);
  display.clearDisplay();

  /******************************************************
    Exibição de dados do sensor DHT11 no monitor serial
    Serial.print(F("UMIDADE: "));
    Serial.print(umidade);
    Serial.println(F("%"));
    Serial.println ("");
    Serial.print ("TEMPERATURA: ");
    Serial.print(celsius);
    Serial.println(F("°C "));
    Serial.println ("");
    Serial.print(fahrenheit);
    Serial.println(F("°F "));
    delay(5000);
    Serial.println ("");
   ********************************************************/
}

void loop()

{

  for (int a = 0; a < 60; a++)
  {
    ajustarHora();
  }

  delay(500);
  for (int e = 0; e < 40; e++)
  {
    analyzerWLAN();

  }
  for (int i = 0; i < 5; i++)
  {
    displayAnimado();
    delay(1000);
  }
  dht11();
}



void displayAnimado()
{
  display.clearDisplay();
  display.drawBitmap(0, 0, smiley_contente, 84, 48, 1);
  display.display();
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0, 0, smiley_sorridente, 84, 48, 1);
  display.display();
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0, 0, smiley_sonolento, 84, 48, 1);
  display.display();
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0, 0, smiley_lingua, 84, 48, 1);
  display.display();
  delay(650);
  display.clearDisplay();
}

/*
   https://www.arduinoecia.com.br/2018/04/relogio-nodemcu-esp8266-display-oled-ntp.html
   https://www.youtube.com/watch?v=9Ew3R8Qgv5w
   http://www.squids.com.br/arduino/index.php/projetos-arduino/projetos-squids/basico/191-projeto-51-temperatura-e-relogio-digital-com-modulo-rtc-ds3231-para-arduino
   Acessado em 21/04/2019
*/
