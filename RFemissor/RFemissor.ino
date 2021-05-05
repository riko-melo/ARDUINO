#include <VirtualWire.h>

String mensagem;

void setup() {
  Serial.begin(9600);  //Define o pino 8 do Arduino como
  vw_set_tx_pin(8);
  vw_setup(2000);  //o pino de dados do transmissor
  Serial.println("Digite o texto e clique em ENVIAR...");

}
void loop() {
  char data[40];
  int numero;
  if (Serial.available() > 0)  {
    numero = Serial.readBytesUntil (13, data, 40);
    data[numero] = 0;
    Serial.print("Enviado : ");
    Serial.print(data);
    Serial.print(" - Caracteres : ");
    Serial.println(strlen(data));    //Envia a mensagem para a rotina que
    send(data);  //transmite os dados via RF
  }
}
void send (char *message) {
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Aguarda o envio de dados }
}

