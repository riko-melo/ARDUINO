#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <LCD5110_Basic.h>

LCD5110 myGLCD(2,3,4,5,6);

const int buttonPin = 7;     // the number of the pushbutton pin
int buttonState = 0;

extern uint8_t SmallFont[];

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

String lastRfid = "";
String kart1 = "";
String kart2 = "";

MFRC522::MIFARE_Key key;

void setup()
{  
  pinMode(buttonPin, INPUT); 
  buttonState = digitalRead(buttonPin);
  
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  myGLCD.print("WELCOME", CENTER, 1);
  myGLCD.print("PLEASE SHOW", CENTER, 10);
  myGLCD.print("YOUR IDCARD", CENTER, 20);
  myGLCD.print("V ((o)) V", CENTER, 35);
  
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID KART OKUMA UYGULAMASI");
  Serial.println("--------------------------");
  Serial.println();
  //EEPROM'dan kart bilgisini oku
  readEEPROM();
}

void loop()
{
  //yeni kart okununmadıkça devam etme
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //kartın UID'sini oku, rfid isimli string'e kaydet
  String rfid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    rfid += mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
    rfid += String(mfrc522.uid.uidByte[i], HEX);
  }
  //string'in boyutunu ayarla ve tamamını büyük harfe çevir
  rfid.trim();
  rfid.toUpperCase();
  
  if (rfid == lastRfid)
    return;
  lastRfid = rfid;

  Serial.print("Kart 1: ");
  Serial.println(kart1);
  Serial.print("Kart 2: ");
  Serial.println(kart2);
  Serial.print("Okunan: ");
  Serial.println(rfid);
  Serial.println();
  //1 nolu kart okunduysa LED'i yak, 2 nolu kart okunduysa LED'i söndür
    if (rfid == kart1)
  {
    Serial.println("LED yandi.");
    myGLCD.clrScr();
    myGLCD.print("LAST ENTERING:", CENTER, 10);
    myGLCD.print("ENSAR LEVENT", CENTER, 32);
    
    delay (2000);
    
    myGLCD.clrScr();
     myGLCD.print("WELCOME", CENTER, 1);
  myGLCD.print("PLEASE SHOW", CENTER, 10);
  myGLCD.print("YOUR IDCARD", CENTER, 20);
  myGLCD.print("V ((o)) V", CENTER, 35);
  }
  if (rfid == kart2)
  {
    Serial.println("LED sondu.");
    myGLCD.clrScr();
    myGLCD.print("LAST ENTERING:", CENTER, 10);
    myGLCD.print("BLUE CHIP", CENTER, 32);

    delay (2000);
    
    myGLCD.clrScr();
     myGLCD.print("WELCOME", CENTER, 1);
  myGLCD.print("PLEASE SHOW", CENTER, 10);
  myGLCD.print("YOUR IDCARD", CENTER, 20);
  myGLCD.print("V ((o)) V", CENTER, 35);
  }
  Serial.println();
  delay(200);
}

void readEEPROM()
{
  //EEPROM'dan ilk kartın UID'sini oku (ilk 4 byte)
  for (int i = 0 ; i < 4 ; i++)
  {
    kart1 += EEPROM.read(i) < 0x10 ? " 0" : " ";
    kart1 += String(EEPROM.read(i), HEX);
  }
  //EEPROM'dan ikinci kartın UID'sini oku
  for (int i = 4 ; i < 8 ; i++)
  {
    kart2 += EEPROM.read(i) < 0x10 ? " 0" : " ";
    kart2 += String(EEPROM.read(i), HEX);
  }
  //Okunan stringleri düzenle
  kart1.trim();
  kart1.toUpperCase();
  kart2.trim();
  kart2.toUpperCase();
}

