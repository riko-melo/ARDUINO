

#include <PCD8544.h>
#define BL 27
    // Um ​​glifo personalizado (um smiley) ...
    glifo de byte constante estático [] = {B00010000, B00110100, B00110000, B00110100, B00010000};

PCD8544 estático lcd;

void setup () {
  // Displays compatíveis com PCD8544 podem ter uma resolução diferente ...
  lcd.begin (84, 48);

  // Adicione o smiley à posição "0" da tabela ASCII ...
  lcd.createChar (0, glifo);
  pinMode (BL, OUTPUT);
  digitalWrite (BL, ALTO);
}


void loop () {
  // Só para mostrar que o programa está vivo ...
  contador int estático = 0;

  // Escreva um trecho de texto na primeira linha ...
  lcd.setCursor (0, 0);
  lcd.print ("Olá, mundo!");

  // Escreva o contador na segunda linha ...
  lcd.setCursor (0, 1);
  lcd.print (contador, DEC);
  lcd.write ('');
  lcd.write (0); // escreva o smiley

  // Use um potenciômetro para definir o contraste do LCD ...
  // nível curto = map (analogRead (A0), 0, 1023, 0, 127);
  // lcd.setContrast (level);

  atraso (200);
  contador ++;
}


/ * EOF - HelloWorld.ino * /
