#include <Servo.h>

#define PARADO               86
#define SENTIDO_HORARIO      0
#define SENTIDO_ANTIHORARIO  160

#define pinPIR 12
#define pinServo0  2

Servo servo0;

void setup() {
  pinMode(pinPIR, INPUT);
  servo0.attach(pinServo0);
}
void servo () {
  if (digitalRead(pinPIR) != 0) {
    servo0.write(SENTIDO_HORARIO);

  }
  delay(2000);
  servo0.write(PARADO);

}
void PIR (void)
{
  bool valorPIR = digitalRead(pinPIR);
  if (valorPIR == HIGH) {
    Serial.println("INVASOR DETECTADO");
    delay(1000);
    servo();
    delay(1000);
    Serial.println("");
    delay(1000);
  }
  else {
    Serial.println("ÁREA SEGURA");
    delay(1000);
  }
}
void loop() {

  PIR();

}
