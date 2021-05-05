#include <TimeLib.h>

// Variaveis-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String menu[3] = {"Mostrar horas", "Definir alarme", "Mudar as horas"}; // Frases do menu
char selec = 0; // Variavel do menu principal
int alarme[2] = {06, 00}; // Variavel do alarme
bool telap = true, telaa = false, telah = false, telac = false, espera = false, ativado = true; // Variaveis de controle das telas

//Musica do despertador------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* musica[] = {"Re", "Mi", "Fa", "Sol", "Do", "La", "Fa", "Do", "Fim"}; //Game of Thrones
int duracao[] = {500, 500, 500, 500, 500, 500, 500, 500};
void tocar(char* mus[], int tempo[]);
int buz = 13; // Pino do Buzzer

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  
  // Relogio
  setTime(12, 00, 0, 1, 1, 2017); // Define hora/min/segundo - dia/mes/ano
  
  digitalWrite(A0, INPUT_PULLUP); // Seta o PULLUP no A0
  pinMode(buz, OUTPUT);
}

void loop() {
  while (telap) { // Tela principal
    lcd.setCursor(0, 0); // Escreve as opçoes do menu
    lcd.print(menu[selec]);
    lcd.setCursor(0, 1);
    if (selec < 2) lcd.print(menu[selec + 1]);
    
    if (!espera && ativado) { // Simbolo do relogio caso o despertador esteja ativo
      lcd.setCursor(15, 0);
      lcd.write(2);
    }
    
    //--------------------------------------------Testa os botoes
    if (analogRead(A0) < 195 && analogRead(A0) > 50 && selec > 0) { // Cima
      selec--;  
      lcd.clear();
    }//lcd.setCursor(3,0); lcd.print(" "); lcd.setCursor(0,1); lcd.print(" "); lcd.setCursor(9,1); lcd.print(" ");}
    if (analogRead(A0) < 380 && analogRead(A0) > 195 && selec < 2) { // Baixo
      selec++;  
      lcd.clear();
    }
    if (analogRead(A0) < 790 && analogRead(A0) > 555) { // Select
      telap = false;
      if (selec == 0) telah = true; // Tela de horas
      if (selec == 1) telaa = true; // Tela do alarme
      if (selec == 2) telac = true; // Tela de configuracao
      lcd.clear();
    }
    Analise(); // Analisa se esta na hora de despertar
    delay(200);
  }

  //-----------------------------------------------------------------Tela Configuração do Relógio
  int d1 = hour(), d2 = minute(), pos = 0; // Variaveis para a tela de configuração
  while (telac) { 
    lcd.setCursor(5, 0);
    if (d1 < 10) lcd.print("0"); lcd.print(d1); lcd.print(":"); if (d2 < 10) lcd.print("0"); lcd.print(d2);
    if (pos == 0) {
      lcd.setCursor(6, 1);  //Indica a posiçao da setinha
      lcd.write(1);
    }
    if (pos == 1) {
      lcd.setCursor(8, 1);
      lcd.write(1);
    }
    //--------------------------------------------------------Testa os botoes
    if (analogRead(A0) < 195 && analogRead(A0) > 50) { // Cima
      if (pos == 0) if (d1 < 23) d1 ++;
      if (pos == 1) if (d2 < 60) d2 ++;
    }
    if (analogRead(A0) < 380 && analogRead(A0) > 195) { // Baixo
      if (pos == 0) if (d1 > 0) d1 --;
      if (pos == 1) if (d2 > 0) d2 --;
    }
    if (analogRead(A0) < 50) if (pos < 1) { // Direita
        pos++;  
        lcd.setCursor(6, 1);
        lcd.print(" ");
      }
    if (analogRead(A0) < 555 && analogRead(A0) > 380) if (pos > 0) { // Esquerda
        pos--;  
        lcd.setCursor(6, 1);
        lcd.print(" ");
      }
    if (analogRead(A0) < 790 && analogRead(A0) > 555) { // Select
      setTime(d1, d2, 0, 11, 11, 2017);
      telac = false; telap = true; lcd.clear();
    }
    Analise(); //Analisa se esta na hora de despertar
    delay(100);
  }

  //-----------------------------------------------------------------Tela de Alarme
  d1 = alarme[0], d2 = alarme[1], pos = 0; //Variaveis para a tela de configuração
  int d3 = ativado;
  while (telaa) { 
    lcd.setCursor(0, 0);
    if (d1 < 10) lcd.print("0"); lcd.print(d1); lcd.print(":"); if (d2 < 10) lcd.print("0"); lcd.print(d2);
    lcd.setCursor(7, 0);
    if (d3) lcd.print("Ativado");
    if (!d3) {
      lcd.setCursor(6, 0);
      lcd.print("Desativado");
    }
    if (pos == 0) {
      lcd.setCursor(1, 1);  // Indica a posiçao da setinha
      lcd.write(1);
    }
    if (pos == 1) {
      lcd.setCursor(3, 1);
      lcd.write(1);
    }
    if (pos == 2) {
      lcd.setCursor(10, 1);
      lcd.write(1);
    }
    //--------------------------------------------------------Testa os botoes
    if (analogRead(A0) < 195 && analogRead(A0) > 50) { //Cima
      if (pos == 0) if (d1 < 23) d1 ++;
      if (pos == 1) if (d2 < 60) d2 ++;
      if (pos == 2) if (d3 < 1) {
          d3 ++;
          lcd.setCursor(6, 0);
          lcd.print(" ");
        }
    }
    if (analogRead(A0) < 380 && analogRead(A0) > 195) { //Baixo
      if (pos == 0) if (d1 > 0) d1 --;
      if (pos == 1) if (d2 > 0) d2 --;
      if (pos == 2) if (d3 > 0) d3 --;
    }
    if (analogRead(A0) < 50) if (pos < 2) {
        pos++;  //Direita
        lcd.setCursor(0, 1);
        lcd.print(" ");
      }
    if (analogRead(A0) < 555 && analogRead(A0) > 380) if (pos > 0) { //Esquerda
        pos--;  
        lcd.setCursor(0, 1);
        lcd.print(" ");
      }
    if (analogRead(A0) < 790 && analogRead(A0) > 555) { //Select
      alarme[0] = d1;
      alarme[1] = d2;
      ativado = d3;
      telaa = false; telap = true; lcd.clear();
    }
    Analise(); // Analisa se esta na hora de despertar
    delay(100);
  }
  while (telah) { //-----------------------------------------------------------------Tela de horas
    lcd.setCursor(3, 0);
    lcd.print(getHour());
    if (!espera && ativado) { // Simbolo do relogio caso o despertador esteja ativo
      lcd.setCursor(15, 0);
      lcd.write(2);
    }
    if (analogRead(A0) < 790 && analogRead(A0) > 555) { //Select
      telah = false; telap = true; lcd.clear();
    }
    Analise();
    delay(500);
  }
  delay(300);
}

void Analise() { // Analisa se deu hora de despertar
  if (hour() == alarme[0] && minute() == alarme[1] && !espera && ativado) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Segure select");
    lcd.setCursor(3, 1);
    lcd.print("para parar");
    while (!espera) {
      tocar(musica, duracao);
      if (analogRead(A0) < 790 && analogRead(A0) > 555) espera = true; //Select
      delay(200);
    }
    lcd.clear();
  }
  if (espera && second() == 0 && ativado) {
    espera = false;
  }
}

String getHour() { // Retorna uma string com a hora h:m:s
  String saida = "";
  if (hour() < 10)
    saida += "0";
  saida += hour();
  saida += ":";
  if (minute() < 10)
    saida += "0";
  saida += minute();
  saida += ":";
  if (second() < 10)
    saida += "0";
  saida += second();
  return saida;
}

void tocar(char* mus[], int tempo[]) {
  int tom = 0;
  for (int i = 0; mus[i] != "Fim"; i++) {
    if (mus[i] == "Do") tom = 262;
    if (mus[i] == "Re") tom = 294;
    if (mus[i] == "Mi") tom = 330;
    if (mus[i] == "Fa") tom = 349;
    if (mus[i] == "Sol") tom = 392;
    if (mus[i] == "La") tom = 440;
    if (mus[i] == "Si") tom = 494;
    if (mus[i] == "Do#") tom = 528;
    if (mus[i] == "Re#") tom = 622;
    if (mus[i] == "Fa#") tom = 370;
    if (mus[i] == "Sol#") tom = 415;
    if (mus[i] == "La#") tom = 466;
    if (mus[i] == "Pausa") tom = 0;
    tone(buz, tom, tempo[i]);
    delay(tempo[i]);
    if (analogRead(A0) < 790 && analogRead(A0) > 555) { //Select
      espera = true;  // CONDIÇAO DO DESPERTADOR - Não tem relação com essa função
      break;
    }
  }
}

