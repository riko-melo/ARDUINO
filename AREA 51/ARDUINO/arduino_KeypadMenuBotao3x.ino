int Botao_mais = 2;  //Variável para selecção do pino de entrada.
int Botao_menu = 3; //Variável para selecção do pino de entrada.
int Botao_menos = 4;  //Variável para selecção do pino de entrada.

int Num_Menu = 0; //Variável para selecção da página de acerto do relógio.

int h, m, s, D, M, A; //Variáveis para alteração da hora e data.

//Variável array de selecção do nome da página do menu de acerto do relógio.
char Lista_menu[] = {' ', 'h', 'm', 's', 'D', 'M', 'Y'};

//Correr a função SETUP apenas uma vez após ser pressionado Reset:
void setup() {
  //Iniciar a comunicação com LCD 16x2:
  lcd.begin(2, 16);
  //Definição do tipo de pino (Saída ou Entrada):
  pinMode(Botao_mais, INPUT);
  //Definição do tipo de pino (Saída ou Entrada):
  pinMode(Botao_menu, INPUT);
  //Definição do tipo de pino (Saída ou Entrada):
  pinMode(Botao_menos, INPUT);
  //Função para obter o tempo da RTC.
  setSyncProvider(RTC.get);
  //Verifica se o valor de tempo não está definido.
  if (timeStatus() != timeSet) {
    //Escreve no LCD:
    lcd.print("BAT. do RTC INOP");
    //Atraso para o próximo passo.
    delay(3000);
    //Apaga a informação do LCD.
    lcd.clear();
    //Alteração dos dados de hora e data.
    h = 1;
    m = 1;
    s = 1;
    D = 1;
    M = 1;
    A = 2001;
    //Grava na RTC uma nova hora e data.
    setTime(h, m , s , D , M , A );
    RTC.set(now());
  }
  else {
    //Escreve no LCD.
    lcd.print("RTC OK");
    //Atraso para o próximo passo.
    delay(3000);
    //Apaga a informação do LCD.
    lcd.clear();
  }
}

//Correr a função LOOP repetidamente:
void loop() {
  //Correr a função:
  Selector_Menu();

  //Correr a função:
  Relogio();
}

//>>> Selecção do menu:
void Selector_Menu() {
  //Condição para activar o acerto da hora e data do relógio:
  if (digitalRead(Botao_menu) == HIGH) {
    //Apaga a informação do LCD.
    lcd.clear();
    //Altera o valor de selecção do menu.
    Num_Menu = Num_Menu + 1;
    //Condição para seleccionar a página de acerto do relógio.
    if (Num_Menu < 7) {
      //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
      lcd.setCursor(15, 0);
      //Escreve no LCD.
      lcd.print(Lista_menu[Num_Menu]);
    }
    //Condição para voltar para a página do relógio.
    else if (Num_Menu > 6) {
      Num_Menu = 0;
    }
  }
}

//>>> Função de escrita dos dados do relógio no LCD:
void Relogio() {
  //Verifica se o valor de tempo está definido.
  if (timeStatus() == timeSet) {
    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(0, 0);
    //Escreve no LCD:
    if (hour() < 10)
      lcd.print('0');
    lcd.print(hour());
    lcd.print(":");
    if (minute() < 10)
      lcd.print('0');
    lcd.print(minute());
    lcd.print(":");
    if (second() < 10)
      lcd.print('0');
    lcd.print(second());

    //Define a coluna (em 16) e a linha (em 2) do LCD onde escrever:
    lcd.setCursor(0, 1);
    //Escreve no LCD:
    if (day() < 10)
      lcd.print('0');
    lcd.print(day());
    lcd.print("/");
    if (month() < 10)
      lcd.print('0');
    lcd.print(month());
    lcd.print("/");
    lcd.print(year());

    //Atraso para o próximo passo.
    delay(500);
  }
  else {
    //Escreve no LCD.
    lcd.print("Bateria do RTC INOP");
    //Atraso para o próximo passo:
    delay(3000);
    //Alteração dos dados de hora e data.
    h = 1;
    m = 1;
    s = 1;
    D = 1;
    M = 1;
    A = 2001;
    //Grava na RTC uma nova hora e data.
    setTime(h, m, s, D, M, A);
    RTC.set(now());
  }

  //Condição para acerto das "horas":
  if (Num_Menu == 1) {
    //Condição para incrementar +1 hora:
    if (digitalRead(Botao_mais) == HIGH) {
      h = hour() + 1;
      //Grava na RTC uma nova hora e data.
      setTime(h, minute(), second(), day(), month(), year());
      RTC.set(now());
    }
    //Condição para retirar -1 hora:
    if (digitalRead(Botao_menos) == HIGH) {
      h = hour() - 1;
      //Grava na RTC uma nova hora e data.
      setTime(h, minute(), second(), day(), month(), year());
      RTC.set(now());
    }
  }
  //Condição para acerto dos "minutos":
  else if (Num_Menu == 2) {
    //Condição para incrementar +1 minuto:
    if (digitalRead(Botao_mais) == HIGH) {
      m = minute() + 1;
      //Grava na RTC uma nova hora e data.
      setTime(hour(), m, second(), day(), month(), year());
      RTC.set(now());
    }
    //Condição para retirar -1 minuto:
    if (digitalRead(Botao_menos) == HIGH) {
      m = minute() - 1;
      //Grava na RTC uma nova hora e data.
      setTime(hour(), m, second(), day(), month(), year());
      RTC.set(now());
    }
  }
  //Condição para acerto dos "segundos":
  else if (Num_Menu == 3) {
    //Condição para incrementar +1 segundo:
    if (digitalRead(Botao_mais) == HIGH) {
      s = second() + 1;
      //Grava na RTC uma nova hora e data.
      setTime(hour(), minute(), s, day(), month(), year());
      RTC.set(now());
    }
    //Condição para retirar -1 segundo:
    if (digitalRead(Botao_menos) == HIGH) {
      s = second() - 1;
      //Grava na RTC uma nova hora e data.
      setTime(hour(), minute(), s, day(), month(), year());
      RTC.set(now());
    }
  }
  //Condição para acerto dos "dias":
  else if (Num_Menu == 4) {
    //Condição para incrementar +1 dia:
    if (digitalRead(Botao_mais) == HIGH) {
      D = day() + 1;
      //Grava na RTC uma nova hora e data.
      setTime(hour(), minute(), second(), D, month(), year());
      RTC.set(now());
    }
    //Condição para retirar -1 dia:
    if (digitalRead(Botao_menos) == HIGH) {
      D = day() - 1;
      //Grava na RTC uma nova hora e data.
      setTime(hour(), minute(), second(), D, month(), year());
      RTC.set(now());
    }
  }
  //Condição para acerto dos "meses":
  else if (Num_Menu == 5) {
    //Condição para incrementar +1 mês:
    if (digitalRead(Botao_mais) == HIGH) {
      M = month() + 1;
      //Grava na RTC uma nova hora e data.
      setTime(hour(), minute(), second(), day(), M, year());
      RTC.set(now());
    }
    //Condição para retirar -1 mês:
    if (digitalRead(Botao_menos) == HIGH) {
      M = month() - 1;
      //Grava na RTC uma nova hora e data.
      setTime(hour(), minute(), second(), day(), M, year());
      RTC.set(now());
    }
  }
  //Condição para acerto dos "anos":
  else if (Num_Menu == 6) {
    //Condição para incrementar +1 ano:
    if (digitalRead(Botao_mais) == HIGH) {
      A = year() + 1;
      //Grava na RTC uma nova hora e data.
      setTime(hour(), minute(), second(), day(), month(), A);
      RTC.set(now());
    }
    //Condição para retirar -1 ano:
    if (digitalRead(Botao_menos) == HIGH) {
      A = year() - 1;
      //Grava na RTC uma nova hora e data.
      setTime(hour(), minute(), second(), day(), month(), A);
      RTC.set(now());
    }
  }
}
