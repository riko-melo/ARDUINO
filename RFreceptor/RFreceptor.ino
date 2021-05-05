#include <VirtualWire.h>
// #include <Adafruit_GFX.h>      //Carrega a biblioteca do display
//#include <Adafruit_PCD8544.h>  //Carrega a biblioteca grafica
byte message[VW_MAX_MESSAGE_LEN];    // Armazena as mensagens recebidas
byte msgLength = VW_MAX_MESSAGE_LEN; // Armazena o tamanho das mensagens
// Pinagem do LCD :
// pin 8 - Serial clock out (SCLK)
// pin 9 - Serial data out (DIN)
// pin 10 - Data/Command select (D/C)
// pin 11 - LCD chip select (CS/CE)
// pin 12 - LCD reset (RST) Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);
//Inicializa e seta os parametros do display

void setup()
{
  Serial.begin(9600);
  vw_set_rx_pin(5); // Define o pino 5 do Arduino como entrada de dados do receptor
  vw_setup(2000);             // Bits por segundo
  vw_rx_start();              // Inicializa o receptor
  // display.begin();
  //display.setContrast(50);
  //Ajusta o contraste do display  display.clearDisplay();
  //Apaga o buffer e o display  display.setTextSize(1);
  //Seta o tamanho do texto  display.setTextColor(BLACK);
  //Seta a cor do texto  display.setCursor(0,0);
  //Seta a posição do cursor  display.print("Aguardando...");
  // display.display();
  delay(2000);
}
void loop()
{
  uint8_t message[VW_MAX_MESSAGE_LEN];
  uint8_t msgLength = VW_MAX_MESSAGE_LEN;
  // display.setCursor(0, 0);  //Seta a posição do cursor
  if (vw_get_message(message, &msgLength))
  { 
    Serial.print("Recebido: ");
    // display.clearDisplay();   //Apaga o buffer e o display
    for (int i = 0; i < msgLength; i++)
    {
      Serial.write(message[i]);
      // display.write(message[i]);
      // display.display();
    }
    Serial.println();
  }
}

