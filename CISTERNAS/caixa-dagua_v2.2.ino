#include <LiquidCrystal.h> // biblioteca gráfica do display LCD
#include <SoftwareSerial.h> //biblioteca responsável por capturar a tecla que foi pressionada no teclado

// DEFINIÇÕES DE PINOS
const int pinTrig  = 6;
const int pinEcho = 7;
const int pinBuzer = 13;

// DECLARAÇÃO DE VARIAVEIS
int counter;
float onesize, flowrate, vol_m3;
unsigned long starttime, busytime;
unsigned long previousTime = 0; //necessita de ter o seu proprio zero
uint8_t letra_C[8] = {0x0, 0xE, 0x11, 0x11, 0x11, 0x11, 0x0, 0x0};
uint8_t letra_M[8] = {0x0, 0x1F, 0x2, 0x4, 0x2, 0x1F, 0x0, 0x0};
uint8_t letra_v[8] = {0x0, 0x7, 0x8, 0x10, 0x8, 0x7 , 0x0, 0x0};
uint8_t caracter_nivel[8] = {0x0, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0};

// DECLARAÇÃO DE VARIAVEIS
LiquidCrystal display(8, 9, 4, 5, 6, 7); //PORTAS DO ARDUINO QUE SÃO UTILIZADAS PELO SHIELD KEYPAD DISPLAY LCD




// DECLARAÇÃO DE FUNÇÕES

float medirVolume(){
  double R = 0.75; // raio maior da caixa
  double r = 0.60; // raio menor da caixa
  float pi = 3.1;
   float h = 0.90 - onesize; // altura da caixa com tampa subtraindo a altura da água na caixa
  vol_m3 = ((pi*h)/3)*(pow(R,2)+(R*r)+pow(r,2));// calcula o volume do cone cortado que é forma geométrica de uma caixa d'água 1000L
  return vol_m3;
 }

float unitvalue(int pinTrig, int pinEcho)
{
#define splitter 72
  long timer = 0;
  float result = 0;
  digitalWrite(pinTrig, LOW); // DESLIGA O TRIGGER E ESPERA 500 uS
  delayMicroseconds(500);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  timer = pulseIn(pinEcho, HIGH);  // Leitura da onda de som
  result = ((float) timer / splitter);
  return result;
}

float gauge()
{
#define pausa 5
#define replay 10
  float somatudo = 0;
  float mediana = 0;
  for (int x = 0; x < replay; x++)
  {
    delay(pausa);
    somatudo += (int)unitvalue(pinTrig, pinEcho) ; // PODE FALHAR AQUI
  }
  mediana = (float) somatudo / replay;
  mediana = mediana - 2;
  return mediana * 1.65;
}

void alarme()
{
  pinMode(pinBuzer, OUTPUT);
  digitalWrite(pinBuzer, LOW); // DESLIGA O TRIGGER E ESPERA 500 uS
  delayMicroseconds(500);
}

void setup()
{
  Serial.begin(9600);
  Serial.print("INICIANDO O SISTEMA, AGUARDE POR FAVOR...");

  pinMode(pinTrig, OUTPUT); // Define o pino Trig do sensor JSN como uma saída
  pinMode(pinEcho, INPUT); // Define o pino Trig do sensor JSN como uma entrada

}

void loop()
{
  busytime = millis();
  if (busytime - starttime > 3000)
  {
    onesize = gauge();
   float volumeL = medirVolume();
    flowrate -= volumeL; 
    Serial.println();
    Serial.print("DISTANCIA: ");
    Serial.print(onesize);
    Serial.println("cm");
    
    Serial.print("VOLUME: ");
    Serial.print(volumeL);
    Serial.println("L");
    Serial.println();
    
    Serial.print(counter);         
    Serial.println("s");       
    
    starttime = busytime;
    counter++;
  }

  if (counter == 20)
  {
    if (flowrate < 0){
      #define full 900
      flowrate = flowrate / 60;
    Serial.print("CONSUMO: ");
    Serial.print(flowrate);
    Serial.println("L/min");
    } 
    else if (flowrate > 0)
    {
       Serial.print("VAZÃO: ");
        Serial.print(flowrate);
    Serial.println("L/min");
    }
    flowrate = 0;
    counter = 0;
  }
}
