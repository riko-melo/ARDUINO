//Programa : RFID - Controle de Acesso RFID para Fechadura elétrica
//Autor : Higo Ricardo

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

LiquidCrystal lcd(6, 7, 5, 4, 3, 2);

char st[20];

void setup()
{
  Serial.begin(9600); // Inicia a serial
  SPI.begin(); // Inicia SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  //Define o numero de colunas e linhas do LCD:
  lcd.begin(16, 2);
  mensagem();
}

void loop()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID do cartão ou chaveiro é ");
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem: ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "36 FA B3 C3") //UID 1 - Chaveiro
  {
    Serial.println("Olá, Aline!");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Olá, Aline!");
    lcd.setCursor(0, 1);
    lcd.print("Acesso liberado!");
    delay(3000);
    mensagem();
  }
  if (conteudo.substring(1) == "76 F0 DF 1F") //UID 2 - Cartao
  {
    Serial.println("Olá,Higo!");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Olá, Higo!");
    lcd.setCursor(0, 1);
    lcd.print("Acesso Liberado!");
    delay(3000);
    mensagem();
  }
}

void mensagem()
{
  lcd.clear();
  lcd.print(" Aproxime o seu");
  lcd.setCursor(0, 1);
  lcd.print("cartao do leitor");
}
