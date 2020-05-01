/* PROGRAMA: CONTROLE DE ACESSO
   AUTOR : HIGO RICARDO
   
  INSTAGRAM @ogih_
*/

#include <LiquidCrystal.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>

#define pinSDA 3
#define pinRST 2
MFRC522 mfrc522(pinSDA, pinRST);
/* O pino SCK deve ligar na porta 13 da Shield LCD
   O pino MOSI deve ligar na porta 11 da Shield LCD
   O pino MISO deve ligar na porta 12 da Shield LCD
   O pino NC – DESLIGADO
   O pino GND deve ligar no pino GND da Shield LCD
   O pino SDA deve ligar na porta 3 da Shield LCD
   O pino RST deve ligar na porta 2 da Shield LCD
   O pino 3.3V deve ligaro na saída 3.3v da Shiled LCD
*/

LiquidCrystal keypad(8, 9, 4, 5, 6, 7);

char st[20];
const int solenoide = 1;
// Apenas a porta 0 restou livre;

bool acesso = false;
int alarm = 0;
uint8_t alarmStat = 0;
uint8_t maxError = 5;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init(); // Inicia MFRC522
  keypad.begin(16, 2);

  pinMode(solenoide, OUTPUT);
  digitalWrite(solenoide, LOW);


}

void loop() {

  if (alarm >= maxError) {
    alarmStat = 1;
  }
  int teclado;
  teclado = analogRead(0);  //Leitura do valor da porta analogica A0

  if (teclado < 100) {
    keypad.clear();
    keypad.setCursor(0, 0);
    keypad.print("CLICOU NA TECLA:");
    keypad.setCursor(0, 1);
    delay (1000);
    keypad.print ("DIREITA");
    delay(1000);

  }
  else if (teclado < 200) {
    keypad.clear();
    keypad.setCursor(0, 0);
    keypad.print("CLICOU NA TECLA:");
    keypad.setCursor(0, 1);
    delay (1000);
    keypad.print ("P/ CIMA");
    delay(1000);

  }
  else if (teclado < 400) {
    keypad.clear();
    keypad.setCursor(0, 0);
    keypad.print("CLICOU NA TECLA:");
    delay (1000);
    keypad.setCursor(0, 1);
    keypad.print ("P/ BAIXO");
    delay(1000);

  }
  else if (teclado < 600) {
    keypad.clear();
    keypad.setCursor(0, 0);
    keypad.print("CLICOU NA TECLA:");
    keypad.setCursor(0, 1);
    delay (1000);
    keypad.setCursor(0, 1);
    keypad.print ("ESQUERDA");
    delay(1000);
    keypad.clear();
  }
  else if (teclado < 800) {
    keypad.clear();
    keypad.setCursor(0, 0);
    keypad.print("CLICOU NA TECLA:");
    keypad.setCursor(0, 1);
    delay (1000);
    keypad.setCursor(0, 1);
    keypad.print ("SELECT");
    delay(1000);

  }

  if (alarmStat == 0) {
    keypad.clear();
    keypad.setCursor (0, 0);
    keypad.print(F("ACESSO RESTRITO"));
    keypad.setCursor (0, 1);
    keypad.print("PASSE O CARTAO!");
    delay(500);

    if ( ! mfrc522.PICC_IsNewCardPresent())    {
      return;
    }
    // Procede a leitura do card
    if ( ! mfrc522.PICC_ReadCardSerial())    {
      return;
    }
    //Mostra ID no Monitor Serial
    String conteudo = "";
    Serial.println();
    Serial.println("Mensagem: ");
    Serial.print("O ID do cartão ou chaveiro RFID é: ");
    byte letra;
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    conteudo.toUpperCase();
    if (conteudo.substring(1) == "36 FA B3 C3") //UID 1 - Chaveiro
    {
      Serial.println("Oi, Aline!");
      Serial.println();
      keypad.clear();
      keypad.setCursor(0, 0);
      keypad.print(F("SEJA BEM-VINDO!"));
      keypad.setCursor(0, 1);
      keypad.print(F("ALINE,"));
      digitalWrite(solenoide, HIGH);
      keypad.setCursor(5, 1);
      keypad.print(F("PODE ENTRAR"));
      delay(4000);
      digitalWrite(solenoide, LOW);
      delay(1000);
    }
    if (conteudo.substring(1) == "76 F0 DF 1F") //UID 2 - Cartao
    {
      Serial.println("Oi, Higo!");
      Serial.println();
      keypad.clear();
      keypad.setCursor(0, 0);
      keypad.print(F("SEJA BEM-VINDO!"));
      keypad.setCursor(0, 1);
      keypad.print(F("HIGO,"));
      digitalWrite(solenoide, HIGH);
      keypad.setCursor(5, 1);
      keypad.print(F("PODE ENTRAR"));
      delay(4000);
      digitalWrite(solenoide, LOW);
      delay(1000);
    }


    alarmStat = 0;
    alarm = 0;
  }
}





