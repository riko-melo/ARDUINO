/*************************************************************************************
   O SOFTWARE É FORNECIDO GRATUITAMENTE, SEM GARANTIA DE QUALQUER TIPO, EXPRESSA
   OU IMPLÍCITA, DE GARANTIA, RESSALVADAS ÀS GARANTIAS DE COMERCIALIZAÇÃO E NÃO
   VIOLAÇÃO DOS DIREIOTS AUTORAIS. EM NENHUMA CIRCUNSTÂNCIA, O TITULAR DOS DIREITOS
   AUTORAIS PODE SER RESPONSABILIZADO POR QUAISQUER DANOS OU UTILIZAÇÃO INDEVIDA
   DESTE.
  ************************************************************************************
   Embutido na placa, há um display OLED 0,96", 128x64, monocromático. Este possui
   comunicação I2C e está ligado ao ESP8266 Wemos por meio de 3 fios:
   SDA no GPIO4   (para dados)
   SCL no GPIO15  (para o clock)
   RST no GPIO16 (para o reset e inicio do display)

  **************************************BIBLIOTECAS************************************
  *************************************************************************************/
#include <Wire.h>
#include <ESP8266WiFi.h>
#include "SSD1306Wire.h"
#include <DHT.h>

// Configuração do Display OLED
SSD1306Wire  display(0x3c, 5, 4);

#define pinLED 16

#define pinDHT D10
#define DHTTYPE DHT11

DHT dht(D10, DHT11);

float umidade, celsius, fahrenheit;

void setup() {
  // Inicializaçaõ do monitor serial e definição da velocidade de transmissão
  Serial.begin(115200);
  delay(100);

  // Inicialização do display OLED e definição do modo de exibição
  displayAnimado();


  //Iniciar o sensor DHT11
  dht.begin();
  //Iniciar o cliente NTP e ajustar a hora
 }

void displayAnimado()
{
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  //display.drawXbm(posX, posY, frame1_width, frame1_height, frame1_bits);
  display.display();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //Desenha um quadrado
  display.drawRect(12, 12, 30, 30);
  //Desenha um quadrado cheio
  display.fillRect(20, 20, 35, 35);
  //Desenha circulos
  for (int i = 1; i < 8; i++)
  {
    display.drawCircle(92, 32, i * 3);
    delay(3000);
    display.clear();
  }

  
  void grafico()
  {
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    for (int contador = 0; contador <= 100; contador++)
    {
      //Desenha a barra de progresso
      display.drawProgressBar(0, 32, 120, 10, contador);
      display.drawString(64, 15, String(contador) + "%");
      display.display();
      delay(17);
      display.clear();
    }
  }

  void dht11()
  {
    /*

      Teste de funcionalidade do DHT11
      if (isnan(umidade) || isnan(celsius) || isnan(fahrenheit) )
      {
       Serial.println(F("Falhou a leitura do sensor DHT!"));
       display.println("Falhou a leitura do sensor DHT!");
       display.display();
       delay(3000);
       display.clearDisplay();
       return
      }
    */
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    celsius = dht.readTemperature();
    display.drawString(0, 5, "Temperatura: ");
    display.drawString(0, 15, celsius);
    display.drawString(10, 15, char(247));
    display.drawString (20, 15, "C");
    display.display();
    delay(5000);
    display.clear();

    fahrenheit = dht.readTemperature(true);
    display.drawString(0, 5, "Temperatura: ");
    display.drawString(0, 15, fahrenheit);
    display.drawString(10, 15, char(247));
    display.drawString (20, 15, "F");
    display.display();
    delay(5000);
    display.clear();

    umidade = dht.readHumidity();
    display.drawString(0, 5, "Umidade: ");
    display.drawString(0, 15, umidade);
    display.drawString(10, 15, "%");
    display.display();
    delay(5000);
    display.clear();

    //Exibição de dados do sensor DHT11 no monitor serial
    Serial.print(F("UMIDADE: "));
    Serial.print(umidade);
    Serial.println(F("%"));
    Serial.println ("");
    Serial.print ("TEMPERATURA: ");
    Serial.print(celsius);
    Serial.println(F("°C "));
    Serial.println ("");
    Serial.print(fahrenheit);
    Serial.println(F("°F "));
    delay(5000);
    Serial.println ("");

    /* Date getDate()
      {
      //Recupera os dados de data e horário usando o client NTP
      char* strDate = (char*)ntpClient.getFormattedDate().c_str();

      //Passa os dados da string para a struct
      Date date;
      sscanf(strDate, "%d-%d-%dT%d:%d:%dZ",
             &date.year,
             &date.month,
             &date.day,
             &date.hours,
             &date.minutes,
             &date.seconds);

      //Dia da semana de 0 a 6, sendo 0 o domingo
      date.dayOfWeek = ntpClient.getDay();
      return date;
      }
    */

    void loop()
    {
      /*
          Date date = get.Date();
          display.printf(" %s             \n\n %s  \n\n %02d/%02d/%d\n\n %02d:%02d:%02d",
          WiFi.SSID().c_str(),
          dayOfWeekNames[date.dayOfWeek],
          date.day,
          date.month,
          date.year,
          date.hours,
          date.minutes,
          date.seconds);
        }
      */


     dh11();

    }
