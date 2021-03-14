#include <LiquidCrystal.h> // biblioteca gráfica do display LCD
#include <SoftwareSerial.h> //biblioteca responsável por capturar a tecla que foi pressionada no teclado

// DEFINIÇÕES DE PINOS
#define pinTrig 6
#define pinEcho 7
#define pinBuzer 13

// DECLARAÇÃO DE VARIAVEIS
float onesize, vazao, volumeL;

unsigned long previousTime = 0; //necessita de ter o seu proprio zero
uint8_t letra_C[8] = {0x0, 0xE, 0x11, 0x11, 0x11, 0x11, 0x0, 0x0};
uint8_t letra_M[8] = {0x0, 0x1F, 0x2, 0x4, 0x2, 0x1F, 0x0, 0x0};
uint8_t letra_v[8] = {0x0, 0x7, 0x8, 0x10, 0x8, 0x7 , 0x0, 0x0};
uint8_t caracter_nivel[8] = {0x0, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0};

// DECLARAÇÃO DE VARIAVEIS
LiquidCrystal display(8, 9, 4, 5, 6, 7); //PORTAS DO ARDUINO QUE SÃO UTILIZADAS PELO SHIELD KEYPAD DISPLAY LCD




// DECLARAÇÃO DE FUNÇÕES

void sonar(byte trig, byte echo)
{
#define splitter 72
#define pausa 5
#define replay 10

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trig, LOW); // DESLIGA O TRIGGER E ESPERA 500 uS
  delayMicroseconds(500);

}

float unitvalue()
{
  long timer = 0;
  float result = 0;

  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  timer = pulseIn(pinEcho, HIGH);  // Leitura da onda de som
  result = ((float) timer / splitter);
  return result;
}

float gauge()
{
  float somatudo = 0;
  float mediana = 0;
  for (int x = 0; x < replay; x++) {
    delay(pausa);
    somatudo += unitvalue() ;
  }
  mediana = (float) somatudo / replay;
  mediana = mediana + 2.25;
  return mediana*2;
}
void alarme()
{
  pinMode(pinBuzer, OUTPUT);
  digitalWrite(pinBuzer, LOW); // DESLIGA O TRIGGER E ESPERA 500 uS
  delayMicroseconds(500);
}

void setup() {
  Serial.begin(9600);
  Serial.print("INICIALIZANDO...");

  sonar(pinTrig, pinEcho);
}

  void loop() {
    onesize = gauge();
    Serial.println();
    Serial.print("DISTANCIA: ");
    Serial.print(onesize);
    Serial.println(" cm");
    volumeL = (onesize / 7, 2) * 10;
    Serial.print("VOLUME: ");
    Serial.println(round(volumeL));
    Serial.print(" L");
    Serial.println();
    delay(2000);
  }
