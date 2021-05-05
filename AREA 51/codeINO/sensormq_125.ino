

//Carrega a biblioteca do sensor ultrassonico
#include <Ultrasonic.h>
 
//Define os pinos para o trigger e echo
#define pino_trigger 4
#define pino_echo 5
#define MQ_analog A2
#define MQ_dig 7

int valor_analog;

int valor_dig;

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);
 
void setup()
{
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
  pinMode(MQ_analog, INPUT);

  pinMode(MQ_dig, INPUT);
}
 
void loop()
{
  //Le as informacoes do sensor, em cm e pol
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  //Exibe informacoes no serial monitor
  Serial.print("Distancia em cm: ");
  Serial.print(cmMsec);
  Serial.print(" - Distancia em polegadas: ");
  Serial.println(inMsec);
  delay(1000);
  
  valor_analog = analogRead(MQ_analog);
  valor_dig = digitalRead(MQ_dig);
  Serial.print(valor_analog);
  Serial.print(" || ");
  if(valor_dig == 0)
    Serial.println("A qualidade do ar esta atingiu niveis novicos a saude!!!");

   else
     Serial.println("A qualidade do ar esta boa, dentro do aceitavel!");
   delay(500);

}
