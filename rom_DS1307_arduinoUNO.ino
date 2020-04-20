
 /* 
  
  ┌▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀┐
                       PROGRAMA DE MEDIÇÃO DE TEMPERATURA BASEADO NO SENSOR DHT12                                                             +
  └▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀┘
 */
#include <DS1307.h>
 
//Modulo RTC DS1307 ligado as portas A4 e A5 do Arduino 
DS1307 rtc(A5, A4);
 
void setup()
{
  //Aciona o relogio
  Serial.begin(9600);
  rtc.halt(false);
   
  //As linhas abaixo setam a data e hora do modulo e devem serem comentadas apos a primeira utilizacao
  //rtc.setDOW(MONDAY);      //Define o dia da semana
  //rtc.setTime(02, 21, 0);     //Definir do horario
  //rtc.setDate(07, 04, 2019);   //Definir dia, mes e ano

  
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
   
 }
void relogio (void)
{
  //Mostra as informações no Serial Monitor
  Serial.print("Hora : ");
  Serial.print(rtc.getTimeStr());
  Serial.println(" ");
  Serial.print("Data : ");
  Serial.print(rtc.getDateStr(FORMAT_SHORT));
  Serial.println (" ");
  Serial.print("Dia: ");
  Serial.println(rtc.getDOWStr(FORMAT_SHORT));
  Serial.println ("");
   
  //Aguarda 1 segundo e repete o processo
  delay (3000);
}
 
void loop()
{
  relogio();
}
