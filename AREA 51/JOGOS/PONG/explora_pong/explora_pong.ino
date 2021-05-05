 Criado em 22 Dec 2012 
 por Tom Igoe 

 Este exemplo está no domínio público. 
 * / 

#include <Esplora.h> 

void setup ( ) { 
  Serial . começar ( 9600 ) ;     // inicializa a comunicação serial 
} 

void loop ( ) { 
  // lê o slider e três dos botões 
  int slider = Esplora. readSlider ( ) ; 
  int resetButton = Esplora. readButton ( SWITCH_1 ) ; 
  int serveButton = Esplora. readButton ( SWITCH_3 ) ; 
  int switchPlayerButton =Esplora readButton ( SWITCH_4 ) ; 

  Serial . imprimir ( slider ) ;                // imprime o valor do slider 
  Serial . imprimir ( "," ) ;                   // adiciona uma vírgula 
  Serial . print ( resetButton ) ;           // imprime o valor do botão de reset 
  Serial . imprimir ( "," ) ;                   // adiciona outra vírgula 
  Serial . print ( serveButton ) ;           // imprime o valor do botão de saque
  Serial . imprimir ( "," ) ;                   // adiciona outra vírgula 
  Serial . println ( switchPlayerButton ) ;  // imprime o último botão com um 
  atraso de nova linha ( 10 ) ;                           // demora antes de enviar o próximo conjunto 
}
