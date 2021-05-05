#include <Ultrasonic.h>
#include <Servo.h>

// PARAMETROS DO SENSOR ULTRASONICO
const int pinTrigger = 4;
const int pinEcho = 5;
Ultrasonic ultrassom(pinTrigger, pinEcho);

#define alarme 4
#define pinRefletancia 6

// DEFINICAO DAS VARIÁVEIS GLOBAIS
float distancia;
unsigned long stoptimer;
bool terraFirme;             // VÁRIÁVEL PARA CHECAR SE HÁ CHÃO A FRENTE DO ROBO
bool perigo = false;         // VARIÁVEL PARA CONTROLE DE SEGURANÇA

void setup()
{
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
  pinMode(pinTrigger, OUTPUT); // Configura o pino trigger como saída
  pinMode(pinEcho, INPUT); // Configura o pino eco como entrada.
  pinMode (alarme, OUTPUT);
  pinMode(pinRefletancia, INPUT);

}

void loop()
{
  checarPerigo();
  delay(50);
}

void naoCabeca() {

  //servoEsquerda.write(180-negativoMax);  // MOVIMENTA PARA TRÁS
  //servoDireita.write(negativoMax);       // MOVIMENTA PARA TRÁS

  //delay(500);

  //servoEsquerda.write(parado); // PARA O SERVO
  //servoDireita.write(parado);  // PARA O SERVO

  //servoCabeca.write(40);
  delay(250);
  //  servoCabeca.write(140);
  delay(250);
  // servoCabeca.write(40);
  delay(250);
  //servoCabeca.write(140);
  //delay(250);
  //servoCabeca.write(90);
  delay(250);
}

void checarPerigo () {
  if ( (millis() - stoptimer) > 500) { // LER DISTANCIA A CADA 500 MILISEGUNDOS
    distancia = ultrassom.distanceRead();
    Serial.print("Distancia: ");
    Serial.print(distancia);
  }
  stoptimer = millis(); // ATRIBUI O VALOR ATUAL DE MILLIS À VARIÁVEL DE CONTROLE
  terraFirme = !digitalRead(pinRefletancia);   // VERIFICA SE TEM CHÃO À FRENTE

  if ( (distancia < alarme) || !terraFirme) { // PARA O ROBO CASO NÃO TENHA CHÃO OU ENCONTRE UM OBSTÁCULO PERTO
    if (!perigo) {
      perigo = true; // GUARDA VALOR PARADO PARA LEMBRAR QUE TEM OBSTÁCULO À FRENTE DURANTE A PRÓXIMA EXECUÇÃO DO LOOP
      naoCabeca();
    }
  } else {
    perigo = false;
  }
}


