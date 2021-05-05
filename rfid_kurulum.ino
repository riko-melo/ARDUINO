/****************************************************************
 * THIS PROJECT BELONG TO ENSAR LEVENT.
 * YOU CAN GET OTHER PROJECTS FROM INSTRUCTABLES
 * https://www.instructables.com/member/ensarlevent/instructables/

Tested with SparkFun Arduino Nano
****************************************************************/
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define RST_PIN 9
#define SS_PIN 10

byte readCard[4];
int successRead;

MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID CARD SETUP BY ensarlevent");
  Serial.println("--------------------------");
  Serial.println("PLEASE SHOW YOUR 1. ID CARD.");
  Serial.println();
  do {
    successRead = getID(); 
  }
  while (!successRead);
  for ( int i = 0; i < mfrc522.uid.size; i++ )
  {
    EEPROM.write(i, readCard[i] ); 
  }
  Serial.println("CARD SAVED TO EEPROM");
  Serial.println();
  Serial.println("PLEASE SHOW YOUR 2. ID CARD.");
  Serial.println();
  do {
    successRead = getID();
  }
  while (!successRead);
  for ( int i = 0; i < mfrc522.uid.size; i++ )
  {
    EEPROM.write(i + 4, readCard[i] );
  }
  Serial.println("CARD SAVED TO EEPROM");
  Serial.println();
  Serial.println("CARDS SAVED PLEASE UPLOAD THE rfid_work CODE");
  Serial.println("IF YOU HAVE A PROBLEM PLEASE CONTACT US.ensarlevent");
}

void loop()
{
}

int getID() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) { 
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  Serial.print("Card ID: ");

  for (int i = 0; i < mfrc522.uid.size; i++) { 
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
  }
  Serial.println("");
  mfrc522.PICC_HaltA();
  return 1;
}
