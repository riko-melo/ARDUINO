#define trigPin 3
#define echoPin 4

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
}
void mensurar()
{
  long duracao;
  float distancia, auxiliar;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duracao = pulseIn(echoPin, HIGH , 1000000);
  auxiliar = duracao;
  distancia = (auxiliar * 340 / 10000) / 2;
  Serial.print("Distancia: ");
  Serial.println(distancia);
  Serial.print("cm");
  Serial.print("Duracao: ");
  Serial.println(duracao);
  Serial.print("s");
}

void loop()
{
  mensurar();
  delay(1000);
}
