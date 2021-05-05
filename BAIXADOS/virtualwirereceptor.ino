#include <MicroLCD.h>
#include <VirtualWire.h>

#define pinRF  12

LCD_SH1106 lcd;
//LCD_SSD1306 lcd;

struct tipoPacote {
  int valor1;
  byte valor2;
  int valor3;
};

tipoPacote pacote; 
uint8_t buf[sizeof(pacote)];
uint8_t buflen = sizeof(pacote);

void setup() {
  lcd.begin();

  lcd.clear();
  lcd.setFontSize(FONT_SIZE_SMALL);
  lcd.println("...");

  lcd.setCursor(0,4);
  lcd.println("|____|____|____|____|");
  lcd.println("0   50  100  150  200");  
  

  vw_set_rx_pin(pinRF);
  vw_setup(2000);   
  vw_rx_start();
}

void loop() {

  if ( vw_have_message() ) {
    vw_get_message(buf, &buflen);
    memcpy(&pacote,&buf,buflen);

    
    lcd.setFontSize(FONT_SIZE_SMALL);
    lcd.setCursor(0,0);
    lcd.print("      ");
    lcd.setCursor(0,0);
    lcd.print(pacote.valor1);

    lcd.setCursor(0,1);
    lcd.print("    ");
    lcd.setCursor(0,1);    
    lcd.print(pacote.valor2);

    lcd.setCursor(0,3);
    lcd.print("                     ");
    lcd.setCursor(pacote.valor3,3);
    lcd.print("V");

  }  
}
