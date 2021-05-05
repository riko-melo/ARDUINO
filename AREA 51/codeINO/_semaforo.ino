int GREEN = 13;
int YELLOW = 12;
int RED = 11;

void setup () {
  pinMode (GREEN, OUTPUT);
  pinMode (YELLOW, OUTPUT);
  pinMode (RED, OUTPUT);
}
void loop () {
  digitalWrite(GREEN, HIGH);
  delay (10000);
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, HIGH);
  delay (4000);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, HIGH);
  delay (10000);
  digitalWrite(RED, LOW);
}
