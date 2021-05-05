
#include "LedControl.h"

LedControl matriz = LedControl(11, 10, 9, 1);
/* Conectar o pino 4 ao DATAIN
   Conecte o pino 3 ao CLK
   Conecte o pino 2 ao CS */
// olho neutro
byte neutral_eye[8] = {B00000000,
                       B00011000,
                       B00111100,
                       B01111110,
                       B01111110,
                       B00111100,
                       B00011000,
                       B00000000
                      };

// olho esbugalhado
byte spooky_eye[8] = {B00111100,
                      B01111110,
                      B11111111,
                      B11111111,
                      B11111111,
                      B11111111,
                      B01111110,
                      B00111100
                     };

// olho fechado (para cima)
byte closed_eye_up[8] = {B00000000,
                         B00001100,
                         B00011000,
                         B00011000,
                         B00011000,
                         B00011000,
                         B00001100,
                         B00000000
                        };

// olho fechado (para baixo)
byte closed_eye_down[8] = {B00000000,
                           B00001100,
                           B00001100,
                           B00000110,
                           B00000110,
                           B00001100,
                           B00001100,
                           B00000000
                          };

// boca feliz            //parte 1
byte happy_mouth[24] = { B00000000,
                         B00000000,
                         B00000000,
                         B00000000,
                         B01100000,
                         B00110000,
                         B00011000,
                         B00001100,
                         //parte 2
                         B00001110,
                         B00000110,
                         B00000110,
                         B00000110,
                         B00000110,
                         B00000110,
                         B00000110,
                         B00001100,
                         //parte 3
                         B00001100,
                         B00011000,
                         B00110000,
                         B01100000,
                         B00000000,
                         B00000000,
                         B00000000,
                         B00000000
                       };


// boca triste           //parte 1
byte sad_mouth[24] = {   B00000000,
                         B00000000,
                         B00000000,
                         B00000000,
                         B00000110,
                         B00001100,
                         B00011000,
                         B00110000,
                         //parte 2
                         B00110000,
                         B01100000,
                         B01100000,
                         B01100000,
                         B01100000,
                         B01100000,
                         B01100000,
                         B00110000,
                         //parte 3
                         B00110000,
                         B00011000,
                         B00001100,
                         B00000110,
                         B00000000,
                         B00000000,
                         B00000000,
                         B00000000
                     };

// boca muito feliz           //parte 1
byte very_happy_mouth[24] = { B00000000,
                              B00000000,
                              B01110000,
                              B01111100,
                              B01100110,
                              B01100110,
                              B01100011,
                              B01100011,
                              //parte 2
                              B01100011,
                              B01100011,
                              B01100011,
                              B01100011,
                              B01100011,
                              B01100011,
                              B01100011,
                              B01100011,
                              //parte 3
                              B01100011,
                              B01100011,
                              B01100110,
                              B01100110,
                              B01111100,
                              B01110000,
                              B00000000,
                              B00000000
                            };

// boca neutra                //parte 1
byte neutral_mouth[24] = {    B00000000,
                              B00000000,
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              //parte 2
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              //parte 3
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              B00010000,
                              B00000000,
                              B00000000
                         };

// língua para fora      //parte 1
byte tongue_mouth[24] = {B00000000,
                         B00000000,
                         B00000100,
                         B11001110,
                         B01111110,
                         B00111110,
                         B00011100,
                         B00001100,
                         //parte 2
                         B00001110,
                         B00000110,
                         B00000110,
                         B00000110,
                         B00000110,
                         B00000110,
                         B00000110,
                         B00001100,
                         //parte 3
                         B00001100,
                         B00011000,
                         B00110000,
                         B01100000,
                         B00000000,
                         B00000000,
                         B00000000,
                         B00000000
                        };

// boca aberta             //parte 1
byte opened_mouth[24] = {  B00000000,
                           B00000000,
                           B00000000,
                           B00000000,
                           B00000000,
                           B00000000,
                           B00000000,
                           B00000000,
                           //parte 2
                           B00000000,
                           B00011100,
                           B00100010,
                           B01000001,
                           B01000001,
                           B00100010,
                           B00011100,
                           B00000000,
                           //parte 3
                           B00000000,
                           B00000000,
                           B00000000,
                           B00000000,
                           B00000000,
                           B00000000,
                           B00000000,
                           B00000000
                        };
void setup()
{
  //inicializar displays (configurar a intensidade de brilho e limpar tudo)
  matriz.shutdown(0, false);
  matriz.setIntensity(0, 1);
  matriz.clearDisplay(0);
  Serial.begin(19200);
  Serial.print("Monitor ligado");
}

void loop()
{
  if (Serial.available())  //verificar se foi recebido algum dado
  {
    char cmd = Serial.read();

    if (cmd == ':') {
      display_eyes(neutral_eye, neutral_eye);
    }
    if (cmd == ';') {
      display_eyes(neutral_eye, closed_eye_up);
    }
    if (cmd == '8') {
      display_eyes(spooky_eye, spooky_eye);
    }
    if (cmd == ')') {
      display_mouth(happy_mouth);
    }
    if (cmd == '(') {
      display_mouth(sad_mouth);
    }
    if ((cmd == 'D') || (cmd == 'd')) {
      display_mouth(very_happy_mouth);
    }
    if ((cmd == 'p') || (cmd == 'P')) {
      display_mouth(tongue_mouth);
    }
    if ((cmd == 'o') || (cmd == 'O')) {
      display_mouth(opened_mouth);
    }
    if (cmd == '|') {
      display_mouth(neutral_mouth);
    }

  }
}
//mudar olhos
void display_eyes(byte right_eye[], byte left_eye[]) {
  for (int i = 0; i < 8; i++) {
    matriz.setRow(0, i, left_eye[i]);
    matriz.setRow(1, i, right_eye[i]);
  }
}


//mudar boca
void display_mouth(byte mouth[]) {
  for (int i = 0; i < 8; i++) {
    matriz.setRow(2, i, mouth[i]);
    matriz.setRow(3, i, mouth[i + 8]);
    matriz.setRow(4, i, mouth[i + 16]);
  }
}
