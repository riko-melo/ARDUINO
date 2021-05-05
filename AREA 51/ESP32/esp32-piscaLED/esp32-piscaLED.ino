/******************************************************************************************
   COPYRIGHT (c) 2019  Higo Melo
 *                                                                                        *
        O SOFTWARE É FORNECIDO GRATUITAMENTE, SEM GARANTIA DE QUALQUER TIPO, EXPRESSA OU
   IMPLÍCITA, DE GARANTIA, RESSALVADAS ÀS GARANTIAS DE COMERCIALIZAÇÃO E NÃO VIOLAÇÃO DOS
   DIREIOTS AUTORAIS. EM NENHUMA CIRCUNSTÂNCIA, O TITULAR DOS DIREITOS AUTORAIS PODE SER
   RESPONSABILIZADO POR QUAISQUER DANOS OU UTILIZAÇÃO INDEVIDA DESTE.
*****************************************************************************************/
int pinLED = 2;

void setup()
{
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, 1);
  Serial.println("LED ON");

  }

 


void loop()
{
                              
    digitalWrite(pinLED, !digitalRead(pinLED));
   delay(1000);    
  
}
