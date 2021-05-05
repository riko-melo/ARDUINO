


﻿/*
 * AUTOR:   Higo Ricardo
 * SKETCH:  INTRUDER WARNING
 * VERSÃO:  1.0.0
 * DATA:    10/09/2018
 */

// BIBLIOTECAS 
#include <Ultrasonic.h>

// DEFINIÇÃO DOS PINOS DESTINADOS AO SENSOR 
#define pinTrigger 5 // Pino de disparo do sensor
#define pinEcho 4 // Pino de leitura do eco do sensor

Ultrasonic ultrasonic(pinTrigger, pinEcho);

void setup()
{
  Serial.begin(9600);
  Serial.print("Lendo dados do sensor...");
}

void loop()
{
  //Ler as informacoes do sensor, em cm e pol
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  
//Exibiç informacoes no serial monitor
  Serial.print("Distancia em cm: ");
  Serial.print(cmMsec);
  Serial.print(" - Distancia em polegadas: ");
  Serial.println(inMsec);
  delay(1000);
}


