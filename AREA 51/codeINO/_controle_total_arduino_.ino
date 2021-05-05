/***************************************************
****************************************************
******** Controle Total Arduino 2.0 - Basic ********
******** Criado por Marlon Nardi Walendorff ********
Blog: http://goo.gl/iD20Xx
Facebook: http://goo.gl/ZRABnn
Youtube: http://goo.gl/2b3Zdi
Email: controletotalarduino@outlook.com.br
****************************************************
****************************************************/

// Saidas PWM
int PWM1 = 0;
int PWM2 = 0;
int PWM3 = 0;
int PWM4 = 0;
int PWM5 = 0;
int PWM6 = 0;

// Saidas Digitais
int Saida1=2;
int Saida2=4;
int Saida3=7;
int Saida4=8;
int Saida5=12;
int Saida6=13;

void setup()  { 
  
  Serial.begin(9600); // Taxa de transmissão de 9600
 
 // Define as portas digitais como saidas "OUTPUT"
  pinMode(Saida1,OUTPUT);
  pinMode(Saida2,OUTPUT);
  pinMode(Saida3,OUTPUT);
  pinMode(Saida4,OUTPUT);
  pinMode(Saida5,OUTPUT);
  pinMode(Saida6,OUTPUT);
} 

void loop()  { 
  char caracter; // Define "caracter" como variável
  caracter = Serial.read(); // Define "Serial.read()" igual a "caracter"
  
// Codições para o funcionamento das saidas digitais
  
// Para a "Saida1"
  if(caracter == 'A'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "A", liga a Saida1
 digitalWrite(Saida1,HIGH);
}
  else
  if(caracter == 'a'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "a", desliga a Saida1
 digitalWrite(Saida1,LOW);
}
  
  
// Para a "Saida2"
  if(caracter == 'B'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "B", liga a Saida2
 digitalWrite(Saida2,HIGH);
}
  else
  if(caracter == 'b'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "b", desliga a Saida2
 digitalWrite(Saida2,LOW);
}



// Para a "Saida3"
  if(caracter == 'C'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "C", liga a Saida3
 digitalWrite(Saida3,HIGH);
}
  else
  if(caracter == 'c'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "c", desliga a Saida3
 digitalWrite(Saida3,LOW);
}
 
 
 
 
 // Para a "Saida4"
  if(caracter == 'D'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "D", liga a Saida4
 digitalWrite(Saida4,HIGH);
}
  else
  if(caracter == 'd'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "d", desliga a Saida4
 digitalWrite(Saida4,LOW);
} 



 // Para a "Saida5"
  if(caracter == 'E'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "E", liga a Saida5
 digitalWrite(Saida5,HIGH);
}
  else
  if(caracter == 'e'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "e", desliga a Saida5
 digitalWrite(Saida5,LOW);
} 



 // Para a "Saida6"
  if(caracter == 'F'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "F", liga a Saida6
 digitalWrite(Saida6,HIGH);
}
  else
  if(caracter == 'f'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "f", desliga a Saida6
 digitalWrite(Saida6,LOW);
} 
  
  
  
// Codições para o funcionamento das saidas PWM 
  
// Para o "PWM1"
 if(caracter == 'G'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "G", acrescenta "25" PWM
  if (PWM1 < 255){
     PWM1 = PWM1 + 25;
}
  }
   else
    if(caracter == 'g'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "g", subtrai "25" PWM
     if (PWM1 > 0 ){
      PWM1 = PWM1 - 25;
   }
     }
  analogWrite(3,PWM1); // Define pino "3" como PWM e o liga a variável "PWM1"  
  
  
  
  
  
// Para o "PWM2"
 if(caracter == 'H'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "H", acrescenta "25" PWM
  if (PWM2 < 255){
     PWM2 = PWM2 + 25;
}
  }
   else
    if(caracter == 'h'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "h", subtrai "25" PWM
     if (PWM2 > 0 ){
      PWM2 = PWM2 - 25;
   }
     }
  analogWrite(5,PWM2); // Define pino "5" como PWM e o liga a variável "PWM2"  
   
  

  

// Para o "PWM3"
 if(caracter == 'I'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "I", acrescenta "25" PWM
  if (PWM3 < 255){
     PWM3 = PWM3 + 25;
}
  }
   else
    if(caracter == 'i'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "i", subtrai "25" PWM
     if (PWM3 > 0 ){
      PWM3 = PWM3 - 25;
   }
     }
  analogWrite(6,PWM3); // Define pino "6" como PWM e o liga a variável "PWM3"  
   
  
  


// Para o "PWM4"
 if(caracter == 'J'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "J", acrescenta "25" PWM
  if (PWM4 < 255){
     PWM4 = PWM4 + 25;
}
  }
   else
    if(caracter == 'j'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "j", subtrai "25" PWM
     if (PWM4 > 0 ){
      PWM4 = PWM4 - 25;
   }
     }
  analogWrite(9,PWM4); // Define pino "9" como PWM e o liga a variável "PWM4"  
   
  
  
  
  
  
  // Para o "PWM5"
 if(caracter == 'K'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "K", acrescenta "25" PWM
  if (PWM5 < 255){
     PWM5 = PWM5 + 25;
}
  }
   else
    if(caracter == 'k'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "k", subtrai "25" PWM
     if (PWM5 > 0 ){
      PWM5 = PWM5 - 25;
   }
     }
  analogWrite(10,PWM5); // Define pino "10" como PWM e o liga a variável "PWM5" 
  




// Para o "PWM6"
 if(caracter == 'L'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "L", acrescenta "25" PWM
  if (PWM6 < 255){
     PWM6 = PWM6 + 25;
}
  }
   else
    if(caracter == 'l'){ // Se o Caracter enviado pelo Software Controle Total Arduino 2.0 - BASIC for "l", subtrai "25" PWM
     if (PWM6 > 0 ){
      PWM6 = PWM6 - 25;
   }
     }
  analogWrite(11,PWM6); // Define pino "11" como PWM e o liga a variável "PWM6" 
  
  
  

  delay(50); // Tempo de 100ms
} 

