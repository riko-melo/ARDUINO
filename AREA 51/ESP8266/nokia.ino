#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "images.h"

// Software SPI (slower updates, more flexible pin options):
// pin 13 - Serial clock out (SCLK)
// pin 11 - Serial data out (DIN)
// pin 8 - Data/Command select (D/C)
// pin 7 - LCD chip select (CS)
// pin 9 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 8, 7, 9);

void setup()   {
  display.begin();

  display.setContrast(50);  /* Initializes the display. Sets SPI freq, SPI data mode, Bit order */

  display.clearDisplay();  /* Clears the screen and buffer */  

  display.setCursor(13,5);  /* Set x,y coordinates */
  display.setTextSize(1); /* Select font size of text. Increases with size of argument. */
  display.setTextColor(BLACK); /* Color of text*/
  display.println("Electronic"); /* Text to be displayed */
  display.setCursor(28,15);
  display.println("Wings");
  display.display();
  delay(500);
  /* Draw rectangle with round edges. Parameters (x_co-ord,y-co-ord,width,height,color) */
  display.drawRoundRect(5, 25, 25, 13, 1, BLACK);
  /* Draw circle. Parameters (x_co-ord of origin,y-co-ord of origin,radius,color) */
  display.drawCircle(45, 34, 10, BLACK);
  /* Draw triangle. Parameters (x0,y0,x1,y1,x2,y2,width,color). (x0,y0), (x1,y1) and (x2,y2) are the co-ord of vertices of the triangle  */
  display.drawTriangle(60, 38, 70, 25, 80, 38, BLACK);
  display.display();
  delay(1000);
  display.clearDisplay();
  /* Draw image. Parameters (x_co-ord, y_co-ord, name of array containing image, width of image in pixels, height in pixels, color) */
  display.drawBitmap(0, 0, Arduino_Logo, 84, 48, 1);
  display.display();
  delay(1000); 
}

void loop() {  
  display.clearDisplay();
  display.drawBitmap(0, 0, Smiley_1, 84, 48, 1);
  display.display();
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0, 0, Smiley_2, 84, 48, 1);
  display.display();
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0, 0, Smiley_3, 84, 48, 1);
  display.display();
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0, 0, Smiley_4, 84, 48, 1);
  display.display();
  delay(300);
}



