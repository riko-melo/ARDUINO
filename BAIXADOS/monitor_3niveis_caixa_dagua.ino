//Programa: Monitoracao de Caixa d´agua usando Arduino
//Autor: Arduino e Cia

//Display HD44780 com adaptador I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define pino_rele 7
#define sensor1 8
#define sensor2 9
#define sensor3 10

int valor_s1 = 1, valor_s2 = 1, valor_s3 = 1;

//Simbolos
uint8_t letra_C[8] = {0x0, 0xE, 0x11, 0x11, 0x11, 0x11, 0x0, 0x0};
uint8_t letra_M[8] = {0x0, 0x1F, 0x2, 0x4, 0x2, 0x1F, 0x0, 0x0};
uint8_t letra_v[8] = {0x0, 0x7, 0x8, 0x10, 0x8, 0x7 , 0x0, 0x0};
uint8_t caracter_nivel[8] = {0x0, 0x1F, 0x1F, 0x1F, 0x1F,
                             0x1F, 0x1F, 0x0};

LiquidCrystal_I2C lcd(0x3B, 16, 2);

int nivel_anterior = 0;

void setup()
{
  Serial.begin(9600);
  //Inicializa o display I2C
  lcd.init();
  lcd.backlight();
  //Caracteres customizados
  lcd.createChar(0, letra_C);
  lcd.createChar(1, letra_M);
  lcd.createChar(2, letra_v);
  lcd.createChar(3, caracter_nivel);

  //Define os pinos dos sensores como entrada
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);

  //Mostra a letra C no display
  lcd.setCursor(15, 0);
  lcd.write(uint8_t(0));
  //Mostra a letra M no display
  lcd.setCursor(8, 0);
  lcd.write(uint8_t(1));
  //Mostra a letra V no display
  lcd.setCursor(0, 0);
  lcd.write(uint8_t(2));

  Serial.println("Monitoracao de Caixa Dagua");
  Serial.println();
}

void loop()
{
  //Leitura dos sensores
  int valor_s1 = digitalRead(sensor1);
  int valor_s2 = digitalRead(sensor2);
  int valor_s3 = digitalRead(sensor3);

  //Mostra valor dos sensores no serial monitor
  Serial.print("S1: ");
  Serial.print(valor_s1);
  Serial.print(" S2: ");
  Serial.print(valor_s2);
  Serial.print(" S3: ");
  Serial.println(valor_s3);

  //Checa o nivel e atualiza o display
  if ((valor_s1 == 0) && valor_s2 == 0 && valor_s3 == 0)
  {
    //Atualiza o nivel no display, enviando o numero de
    //"quadrados" que devem ser mostrados para indicar
    //o nivel
    mostra_nivel(15);
    //Desliga o rele
    digitalWrite(pino_rele, HIGH);
  }

  if ((valor_s1 == 1) && valor_s2 == 0 && valor_s3 == 0)
  {
    //Atualiza o nivel no display
    mostra_nivel(11);
  }

  if ((valor_s1 == 1) && valor_s2 == 1 && valor_s3 == 0)
  {
    //Atualiza o nivel no display
    mostra_nivel(4);
  }

  if ((valor_s1 == 1) && valor_s2 == 1 && valor_s3 == 1)
  {
    mostra_nivel(0);
    //Liga o rele
    digitalWrite(pino_rele, LOW);
  }
  delay(100);
}

void mostra_nivel(int nivel)
{
  if (nivel != nivel_anterior)
  {
    //Apaga informacao anterior
    lcd.setCursor(0, 1);
    lcd.print("                ");
    //Atualiza o nivel no display
    for (int i = 0; i <= nivel; i++)
    {
      lcd.setCursor(i, 1);
      lcd.write(uint8_t(3));
    }
    nivel_anterior = nivel;
  }
}


