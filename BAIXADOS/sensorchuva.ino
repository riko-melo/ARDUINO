#define pinSensorA A0
#define pinSensorD 8

void setup() {
  pinMode(pinSensorD, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.print("Digital:");
  
  if (digitalRead(pinSensorD)) {
     Serial.print("SEM CHUVA ");
  } else {
     Serial.print("ESTA CHUVENDO ");
  }

  Serial.print("  Analogico:");
  Serial.print(analogRead(pinSensorA)); 
  Serial.print("  ");

  Serial.print("  Atuador:");
  if (analogRead(pinSensorA) > 700) {
     Serial.println("TOLDO ABERTO");
     //digitalWrite(pinSolenoide, HIGH);
  } else {
    Serial.println("TOLDO FECHADO");
     //digitalWrite(pinSolenoide, LOW);
  }
}
