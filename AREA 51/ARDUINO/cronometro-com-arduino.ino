//********************* Projeto TimeFut *********************//
// Desenvolvido por: Josemar Alves                           //
// E-mail: josemar.bool@gmail.com                            //
// Versão: 1.0 - 14/04/2014                                  //
//                                                           //
//************** Funcionalidades ****************************//
// - Faz a contagem do tempo (10 Min) de uma partida         //
// - Aguarda um Play pelo controle;                          //
// - Toca a sirene e inicia a contagem regressiva;           //
// - Faltando 1 Min. dá outro toque na sirene;               //
// - Ao final do tempo mantem a sirene ligada por 5 seg;     //
// - Em qualquer momento pode receber:                       //
//     # Play: Inicia ou reinicia contagem                   //
//     # Pause: Pausa contagem                               //
//     # Reset: Reinicia tempo quando pausado                //
//                                                           //
//***********************************************************//

//***************** Display de 7 segmentos ******************//
//                    ______a_______
//                   /             /
//                 f/             /b
//                 /             /
//                /------g------/
//               /             /
//             e/             /c
//             /______d______/  0 pt
//                
// Ligação:  Display - a  b  c  d  e  f  g  pt      
//           74HC595 - Qa Qb Qc Qd Qe Qf Qg Qh
//

#include <TimerOne.h>                                        // Biblioteca para controle do Timer do Arduino


//*************** Variaveis Globais e Defines ***************//
                                                             
#define  RESET  0x00                                         // Possíveis estados do jogo (statusJogo)  
#define  PLAY   0x01
#define  PAUSE  0x02
#define  STOP   0x03

#define  DISP1  0x01                                         // Possíveis estados do display (statusDisp)
#define  DISP2  0x02
#define  DISP3  0x03
#define  DISP4  0x04

#define  CONTROLE_PLAY   0x01                                // Possíveis estados do controle (statusControle)
#define  CONTROLE_PAUSE  0x02
#define  CONTROLE_RESET  0x03

#define  SEM_ALERTA     0x00                                 // Possíveis estados do alesrta (statusAlerta)
#define  ALERTA_PLAY    0x01
#define  ALERTA_PAUSE   0x02
#define  ALERTA_STOP    0x03
#define  ALERTA_RESET   0x04
#define  ALERTA_INICIO  0x05
#define  ALERTA_1MIN    0x06
#define  ALERTA_FIM     0x07

#define  FREQ          50                                    // Frequência dos displays
#define  TEMPO_TIMER   5000                                  // Timer configurados para 5ms, o timer é incrementado a cada 1us -> ((1/(4*FREQ))*1000000) = 5000 ciclos
#define  TEMPO_SEG     200                                   // Quantidade de estouros do timer para gerar 1 segundo -> (1000000/TEMPO_TIMER) = 200 estouros do timer
#define  TEMPO_5S      1000                                  // 5 x TEMPO_SEG -> usado para o alerta
#define  TEMPO_MINUTO  60                                    // 1 minuto = 60 segundos
#define  TEMPO_JOGO    600                                   // 10 minutos = 10 x 60 segundos = 600
#define  BASE10        10                                    // Base para calcular o numero a ser mostrado no display

                                                             // Valores de 0 a 9 para o display de 7 segmentos (o = Aceso / 1 = Apagado)
byte  decTO7seg[10] = {B00000010,                            // ZERO_7SEG
                       B10011110,                            // UM_7SEG
                       B00100100,                            // DOIS_7SEG
                       B00001100,                            // TRES_7SEG
                       B10011000,                            // QUATRO_7SEG
                       B01001000,                            // CINCO_7SEG
                       B11000000,                            // SEIS_7SEG
                       B00011110,                            // SETE_7SEG
                       B00000000,                            // OITO_7SEG
                       B00011000};                           // NOVE_7SEG
                                 
int latchPin = 8;                                            // Latch, conectado ao pino 12 do 74HC595
int dataPin = 11;                                            // Data, conectado ao pino 14 do 74HC595
int clockPin = 12;                                           // Clock, conectado ao pino 11 do 74HC595
int alertaPin = 13;                                          // Pino que controla a sirene
int botao_PLAY = 5;                                          // Botão play do controle
int botao_PAUSE = 6;                                         // Botão pause do controle
int botao_RESET = 7;                                         // Botão reset do controle
int ledPin = 9;                                              // Pino para oscilar o Led a cada 1 segundo
int disp1Pin = 1;                                            // Pino que controle o display 1
int disp2Pin = 2;                                            // Pino que controle o display 2
int disp3Pin = 3;                                            // Pino que controle o display 3
int disp4Pin = 4;                                            // Pino que controle o display 4

unsigned long contador;                                      // Variável para contar o tempo corrente
unsigned long tempoAux;                                      // Variável auxiliar para armazenar o tempo atual, usada para o alerta
int contTime;                                                // Variável para contar o tempo de jogo
int contSeg;                                                 // Variável para contar 1 segundo
int minTemp, segTemp;                                        // Variáveis para armazenar o valor dos segundos e minutos na forma decimal

byte min1, min2, seg1, seg2;                                 // Variáveis para armazenar os valores a serem enviados ao display

byte statusJogo;                                             // Variável para controle do jogo
byte statusDisp;                                             // Variável para controle dos displays
byte statusControle;                                         // Variável para controlar o controle remoto
byte statusAlerta;                                           // Variável para controle do alerta
byte alertaAnterior;                                         // Variável para controle do alerta
byte alertaPendente;                                         // Variável para controle do alerta

//************************** Setup **************************//
void setup() {
                                    
  pinMode(latchPin, OUTPUT);                                 // define os pinos como entrada ou saída
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(disp1Pin, OUTPUT);
  pinMode(disp2Pin, OUTPUT);
  pinMode(disp3Pin, OUTPUT);
  pinMode(disp4Pin, OUTPUT);

  pinMode(alertaPin, OUTPUT);
  pinMode (ledPin, OUTPUT);
  
  pinMode (botao_PLAY, INPUT);
  pinMode (botao_PAUSE, INPUT);
  pinMode (botao_RESET, INPUT);
    
  digitalWrite(disp1Pin, HIGH);                              // Apaga todos os displays              
  digitalWrite(disp2Pin, HIGH);
  digitalWrite(disp3Pin, HIGH);
  digitalWrite(disp4Pin, HIGH);

  contSeg = 0;                                               // Variável de contagem dos segundos = 0
  contTime = TEMPO_JOGO;                                     // Inicializa tempo do jogo
  statusDisp = DISP4;                                        // Inicializa display
  statusJogo = RESET;                                        // Inicializa estado do jogo
  statusControle = CONTROLE_RESET;                           // Inicializa estado do controle
  statusAlerta = SEM_ALERTA;                                 // Inicializa Alerta
  alertaAnterior = SEM_ALERTA;
  alertaPendente = 0;
  Timer1.initialize(TEMPO_TIMER);                            // Inicializa timer para contar 5ms
  Timer1.attachInterrupt(trata_timer);                       // Determina função a ser executada a cada estouro do timer

}

//************************** Loop ***************************//
void loop() {                                                // Loop principal  
  while(true){    
    atualiza_controle();                                     // Chama função que lê os botões 
    trata_jogo();                                            // Chama função que verificado os estados do jogo de acordo com o tempo e o controle
    trata_alerta();                                          // Chama a função que trata os novos alertas e os alertas pendentes
  }  
}

//********************** trata_controle *********************//
void atualiza_controle(void){                                // Função para ler qual botão foi precisonado e atualizar a variável
  if (digitalRead(botao_PAUSE) == HIGH) statusControle = CONTROLE_PAUSE;
  if (digitalRead(botao_RESET) == HIGH) statusControle = CONTROLE_RESET;
  if (digitalRead(botao_PLAY) == HIGH) statusControle = CONTROLE_PLAY;
  }
  
//************************ trata_jogo ***********************//
void trata_jogo(void){                                       // Função que modifica o estado do jogo e dos alertas de acordo com os botões precionados
  switch(statusJogo){    
    case PLAY:                                               // Para jogo em modo PLAY
        if(statusControle == CONTROLE_PAUSE){                // Se botão PAUSE foi pressionado...
            statusJogo = PAUSE;                              // Muda estado do jogo para PAUSE
            statusAlerta = ALERTA_PAUSE;                     // Muda estado do alerta para PAUSE
        }      
        break;     
     case PAUSE:                                             // Para jogo em modo PAUSE
         if(statusControle == CONTROLE_PLAY){                // Se botão PLAY foi pressionado...
            statusJogo = PLAY;                               // Muda estado do jogo para PLAY
            statusAlerta = ALERTA_PLAY;                      // Muda estado do alerta para PLAY
        }
         if(statusControle == CONTROLE_RESET){               // Se botão RESET foi pressionado...
            statusJogo = RESET;                              // Muda estado do jogo para RESET
            statusAlerta = ALERTA_RESET;                     // Muda estado do alerta para RESET
        }    
        break;     
     case RESET:                                             // Para jogo em modo RESET
         if(statusControle == CONTROLE_PLAY){                // Se botão PLAY foi pressionado...
            statusJogo = PLAY;                               // Muda estado do jogo para PLAY
            statusAlerta = ALERTA_PLAY;                      // Muda estado do alerta para PLAY
        }  else{                                             // Se botão PLAY NÃO foi pressionado...
       contSeg = 0;                                          // Zera variavel que conta os segundos
       contTime = TEMPO_JOGO;                                // Inicializa tempo do jogo
       atualiza_contador();                                  // Atualiza os digitos do display       
        }
        break;
      case STOP:                                             // Para jogo em modo STOP
        if(statusControle == CONTROLE_RESET){                // Se botão RESET foi pressionado...
            statusJogo = RESET;                              // Muda estado do jogo para RESET
        }      
        break;   
     default:  break;     
    }
}

//********************** trata_alerta ***********************//
void trata_alerta(){                                         // Função que executa os alertas sonoros de acordo com os estados do jogo
  if((statusAlerta!=alertaAnterior) || (alertaPendente!=0)){ // Se houve mudança no estado do alerta ou se ha alerta pendente...
    if(alertaPendente != 0){                                 // Se houve ha alerta pendente...
      if(contador >= tempoAux){                              // Se tempo do alerta já passou...
        digitalWrite(alertaPin, LOW);                        // Desliga a sirene
        alertaPendente = 0;                                  // Limpa o alerta pendente
        }
      } else {                                               // Se não tem alerta pendente (entao houve mudança no estado do alerta)...
        digitalWrite(alertaPin, HIGH);                       // Liga a sirene
        tempoAux = contador + TEMPO_SEG;                     // Define que o alerta vai ficar ligado por 1 segundo
        if(statusAlerta == ALERTA_FIM) tempoAux = contador + TEMPO_5S;  // Se houver alerta de fim de jogo...  Define que o alerta vai ficar ligado por 5 segundo
        alertaPendente++;                                    // Seta variável de alerta pendente        
        alertaAnterior = statusAlerta;                       // Atualiza o estado do alerta
      }
    }
  }
//********************* trata_timer *************************//
void trata_timer(void){                                      // Função chamada a cada estouro do timer que executa a contagem do tempo
  contador++;                                                // Incrementa contador de tempo
  troca_display();                                           // Troca o display que está aceso 
  if(statusJogo == PLAY){                                    // Se o jogo estiver em modo PLAY...
    contSeg++;                                               // Incrementa variável de contagem de segundo
      if(contSeg == TEMPO_SEG){                              // Se a variável atingiu o tempo de 1 segundo...
        contTime--;                                          // Decrementa tempo de jogo
        digitalWrite(ledPin, !digitalRead(ledPin));          // Muda o estado do Led
        if(contTime == TEMPO_MINUTO) statusAlerta = ALERTA_1MIN;  // Se falta apenas 1 minuto para final de jogo...  Modifica estado do alerta para alerta de 1 minuto
        if(contTime == 0){                                   // Se acabou o tempo de jogo...
          statusAlerta = ALERTA_FIM;                         // Modifica estado do alerta para alerta de fim do jogo
          statusJogo = STOP;                                 // Muda o estado do jogo para STOP
        }  
        contSeg = 0;                                         // Zera a variável de contagem de segundo
        atualiza_contador();                                 // Atualiza os digitos do display      
      }       
    }
  }

//****************** atualiza_contador **********************//
void atualiza_contador(void){                                // Função para atualizar os dígitos a serem mostrados no display.
  minTemp = contTime/TEMPO_MINUTO;                           // Separa do tempo de jogo a quantidade de minutos
  min1 = decTO7seg[minTemp/BASE10];                          // Separa a dezena dos minutos do tempo de jogo e guarda na variável
  min2 = decTO7seg[minTemp%BASE10];                          // Separa a unidade dos minutos do tempo de jogo e guarda na variável
  
  segTemp = contTime%TEMPO_MINUTO;                           // Separa do tempo de jogo a quantidade de segundos
  seg1 = decTO7seg[segTemp/BASE10];                          // Separa a dezena dos segundos do tempo de jogo e guarda na variável
  seg2 = decTO7seg[segTemp%BASE10];                          // Separa a unidade dos segundos do tempo de jogo e guarda na variável
}

//******************** troca_display ************************//
void troca_display(void){                                    // Função que faz a varredura dos displays  
  switch(statusDisp){                                        
    case DISP1:                                              // Para o display 1 aceso...
      digitalWrite(disp1Pin, HIGH);                          // Apaga display 1
      shiftOut(min2);                                        // Envia a unidade de minutos para os displays
      digitalWrite(disp2Pin, LOW);                           // Acende display 2
      statusDisp = DISP2;                                    // Muda o estado para indicar que o display 2 está aceso
      break;
    case DISP2:                                              // Para o display 2 aceso...
      digitalWrite(disp2Pin, HIGH);                          // Apaga display 2
      shiftOut(seg1);                                        // Envia a dezena de segundos para os displays
      digitalWrite(disp3Pin, LOW);                           // Acende display 3
      statusDisp = DISP3;                                    // Muda o estado para indicar que o display 3 está aceso
      break;
    case DISP3:                                              // Para o display 3 aceso...
      digitalWrite(disp3Pin, HIGH);                          // Apaga display 3
      shiftOut(seg2);                                        // Envia a unidade de segundos para os displays
      digitalWrite(disp4Pin, LOW);                           // Acende display 4
      statusDisp = DISP4;                                    // Muda o estado para indicar que o display 4 está aceso
      break;
    case DISP4:                                              // Para o display 4 aceso...
      digitalWrite(disp4Pin, HIGH);                          // Apaga display 4
      shiftOut(min1);                                        // Envia a dezena de minutos para os displays
      digitalWrite(disp1Pin, LOW);                           // Acende display 1
      statusDisp = DISP1;                                    // Muda o estado para indicar que o display 1 está aceso
      break;
    default:  break;    
   }  
}

//************************ shiftOut *************************//
void shiftOut(byte dataOut) {                                // Função que desloca 8 bits, com o bit menos significativo (LSB) sendo deslocado primeiro; na borda de subida do clock
  boolean pinState;
  digitalWrite(latchPin, LOW);                               // Desabilita a saida para o display
  digitalWrite(clockPin, LOW);
  for (int i=0; i<=7; i++){                                  // Para cada bit em dataOut, envie um bit
    digitalWrite(clockPin, LOW);                             // Define clockPin como LOW antes de enviar o bit
    if ( dataOut & (1<<i) ){                                 // Se o valor de DataOut e (E lógico) de uma máscara de bits forem verdadeiros,
      pinState = HIGH;                                       // Defina pinState como 1 (HIGH)
      } else {
      pinState = LOW;                                        // Senão, defina pinState como 0 (LOW)
      }
    digitalWrite(dataPin, pinState);                         // Envia o bit no extremo ascendente do clock
    digitalWrite(clockPin, HIGH);                            // Gera uma borda de subida no pino de clock
    }
  digitalWrite(latchPin, HIGH);                              // Envia os dados para o display
  digitalWrite(clockPin, LOW);                               // Interrompa o deslocamento de dados
}
