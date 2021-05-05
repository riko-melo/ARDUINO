void displayAnimado()
{
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //DESENHA O QUADRADO VAZIO
  display.drawRect(12, 12, 30, 30);
  delay(2000);
  //DESENHA O QUADRADO CHEIO
  display.fillRect(20, 20, 35, 35);
  delay(2000);
  //DESENHA O CIRCULO VAZIO
  for (int a = 1; a < 8; a++)
  {
    display.drawCircle(92, 32, a * 3);
  }
  display.display();
  delay(3000);
  display.clear();
}

void setup() {
   display.init();
  delay(25);
  displayAnimado();
  delay(25);

}

void loop() {
 exibirSMS();

}

void exibirSMS() {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);  //Define fonte e tamanho da letra
  display->drawString (0 + x, 10 + y, "MENSAGEM");
  display->drawString (15 + x, 25 + y, "URGENTE!");
  display->display();
  delay(3000);
  display->clear();

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString (41, 0, "DE");
  display->setFont(ArialMT_Plain_16);
  display->drawString (29, 13, "HIGO");
  display->setFont(ArialMT_Plain_10);
  display->drawString (35, 32, "PARA");
  display->setFont(ArialMT_Plain_16);
  display->drawString (15, 45, "REBECA");
  display->display();
  delay(3000);
  display->clear();

  for (int16_t j = -127; j < 192; j++)
  {
    int i = -125;
    i++;
    if (j == 164)
    {
      display->setFont(ArialMT_Plain_16);
      display->drawString(j - 164, i, "SAUDADES!!!");
      display->display();
      delay(3000);
      display->clear();
    }
    if (j == 10)
    {
      display->setFont(ArialMT_Plain_10);
      display->drawString(j, 11, "DESDE QUE NASCEU,");
      delay(2000);
      display->drawString(j + 17, 22, "MEU CORAÇÃO");
      delay(2000);
      display->drawString(j + 35, 33, "É SEU!");
      display->display();
      delay(4000);
      display->clear();
    }
    else
    {
      display->setFont(ArialMT_Plain_16);
      display->drawString(j, 31, "EU TE AMO!");
      display->display();
      delay(17);
      display->clear();
    }
  }
}
