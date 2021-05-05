#include <SPI.h>		// Biblioteca de comunicacao SPI
#include <DHT.h>		 // Biblioteca Adafruit para sensor de temperatura DHT11

#define pinDHT 12
#define DHYTYPE DHT11
#define pinEmbalo 2
#define pinLED 13
#define pinChave 3

long unsigned timeOne = millis();
long unsigned timeOut = 0;
int valorChave = 0;
int antevalor = 0;
int contadorChave = 0;

DHT dht(pinDHT, DHYTYPE);	//Objeto responsavel pela comunicacao entre microntolador e sensor

void fazerInterrupcao();

void setup() {
  pinMode(pinEmbalo, INPUT_PULLUP);
  pinMode(pinChave, INPUT);
  pinMode(pinLED,OUTPUT);
  Serial.begin(9600);
  Serial.println("Inicialização do sensor de temperatura...");
  dht.begin();
  attachInterrupt(digitalPinToInterrupt(pinEmbalo), fazerInterupcao, RISING);

}

void loop() {
 valorChave = digitalRead(pinChave);
  if (valorChave != antevalorChave){
    if (valorChave = HIGH){
      contadorChave++
      Serial.println("LED ON");
      Serial.print("Acionamentos: ");
      Serial.print(contadorChave);
      else(){
        Serial.print("LED OFF");
      }
      delay (500);
    }
    antevalorChave = valorChave;      
    if (contadorChave %4 == 0 ) {
    digitalWrite(pinLED, LOW);
  }
  else {
    digitalWrite(pinLED, HIGH);
    
  }
  int timeIn = timeOne;
  timeOut = millis() - timeIn;
  if (timeOut > 10000) {
    Serial.print ("Umidade: ");
    Serial.print(umidade);
    Serial.print(" %\t");
    Serial.print ("Temperatura: ");
    Serial.print(celsius);
    Serial.print("*C");

    float celsius = 0;			// Variavel local que armazena a temperatura
    float umidade = 0;			// Variavel local que armazena a Umidade
    celsius = dht.readTemperature();	//Leitura da temperatura
    umidade = dht.readHumidity();		// Leitura da Umidade
  }
  if (isnan(celsius) || isnan(umidade)) {
    Serial.println("Falha na leitura do sensor DHT");
    delay(2000);
    return;
  }


}
void fazerInterrupcao() {
  static bool estado = LOW;
  static unsigned long delayEstado;


  if ( (millis() - delayEstado) > 100 ) {
    estado = !estado;
    delayEstado = millis();
  }
// acrescentar codificação da balança
 Serial.print("A Balança não foi instalada.");
}


