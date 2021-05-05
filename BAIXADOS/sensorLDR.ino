/*
 Sensor LDR
 Conectar um LDR a uma entrada analogica para controlar cinco saidas em 
funcao da luz ambiente.
 Este codigo e de dominio publico. 
 Criado em 27/11/2011 por Arduteka.
 Modificado em 13/01/2014 por Multilogica-Shop.
*/
//Armazenar os dados recolhidos pelo sensor LDR:
int
 valorLDR = 0; 
//Definir os pinos de entrada dos LEDs:
int
 pinLed1 = 12;
int
 pinLed2 = 11;
int
 pinLed3 = 10;
int
 pinLed4 = 9;
int
 pinLed5 = 8;
//Definir pino de entrada do sensor LDR
int
 pinLDR = 0;
void setup
()
{
Serial.begin
(9600);
//Definir os pinos de saida dos LEDs:
pinMode
(pinLed1, 
OUTPUT
);
pinMode
(pinLed2, 
OUTPUT
);
pinMode
(pinLed3, 
OUTPUT
);
pinMode
(pinLed4, 
OUTPUT
);
pinMode
(pinLed5, 
OUTPUT
);
  //Definimos o uso de uma referencia externa:  
pinMode
(
EXTERNAL
);
}
void loop
()
{
  //Guardar o valor da leitura de uma variavel:
  valorLDR = 
analogRead
(pinLDR);
Serial.println
(valorLDR);
  //Definicao do padrao de controle dos LEDs:
if
(valorLDR >= 1023)
  {
digitalWrite
(pinLed1, 
LOW
);
digitalWrite
(pinLed2, 
LOW
);
digitalWrite
(pinLed3, 
LOW
);
digitalWrite
(pinLed4, 
LOW
);
digitalWrite
(pinLed5, 
LOW
);
  }
else if
((valorLDR >= 823) & (valorLDR < 1023))
  {
digitalWrite
(pinLed1, 
HIGH
);
digitalWrite
(pinLed2, 
LOW
);
digitalWrite
(pinLed3, 
LOW
);
digitalWrite
(pinLed4, 
LOW
);
digitalWrite
(pinLed5, 
LOW
);
  }
else if
((valorLDR >= 623) & (valorLDR < 823))
  {
digitalWrite
(pinLed1, 
HIGH
);
digitalWrite
(pinLed2, 
HIGH
);
digitalWrite
(pinLed3, 
LOW
);
digitalWrite
(pinLed4, 
LOW
);
digitalWrite
(pinLed5, 
LOW
);
  }
else if
((valorLDR >= 423) & (valorLDR < 623))
  {
digitalWrite
(pinLed1, 
HIGH
);
digitalWrite
(pinLed2, 
HIGH
);
digitalWrite
(pinLed3, 
HIGH
);
digitalWrite
(pinLed4, 
LOW
);
digitalWrite
(pinLed5, 
LOW
);
  }
else if
((valorLDR >= 223) & (valorLDR < 423))
  {
digitalWrite
(pinLed1, 
HIGH
);
digitalWrite
(pinLed2, 
HIGH
);
digitalWrite
(pinLed3, 
HIGH
);
digitalWrite
(pinLed4, 
HIGH
);
digitalWrite
(pinLed5, 
LOW
);
  }
else
  {
digitalWrite
(pinLed1, 
HIGH
);
digitalWrite
(pinLed2, 
HIGH
);
digitalWrite
(pinLed3, 
HIGH
);
digitalWrite
(pinLed4, 
HIGH
);
digitalWrite
(pinLed5, 
HIGH
);
  }
}