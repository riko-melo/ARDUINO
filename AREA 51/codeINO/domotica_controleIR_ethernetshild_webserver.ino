//CONTROLE REMOTO
#include <IRremote.h>

#include <SPI.h>
#include <Ethernet.h> //W5100
#include <MFRC522.h> // Leitor RFid

//Definindo os pinos do RFid
#define SS_PIN 48
#define RST_PIN 49

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void abrirporta();
void abrirportao();

IRsend irsend;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

EthernetServer server(80);

//RELES
int pino_rele1 = 2; // Luz do Escritório
int pino_rele2 = 3; // 
int pino_rele3 = 5; // Microfone1
int pino_rele4 = 6; // Microfone2
int pino_rele5 = 7; // Portão da Rua
int pino_rele6 = 8; // Porta da Sala
int pino_rele7 = 11; // 
int pino_rele8 = 12; // Luz da mesa

void setup() {

// disable w5100 SPI
pinMode(10,OUTPUT);
digitalWrite(10,HIGH);

// disable SD SPI
pinMode(4,OUTPUT);
digitalWrite(4,HIGH);

// rest of your setup

  
  Serial.begin(9600);


  //LEITOR RFid
  SPI.begin(); // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522

  pinMode(pino_rele1, OUTPUT); // Luz do Escritório
  pinMode(pino_rele2, OUTPUT); // 
  pinMode(pino_rele3, OUTPUT); // Microfone1
  pinMode(pino_rele4, OUTPUT); // Microfone2
  pinMode(pino_rele5, OUTPUT); // Portão da rua
  pinMode(pino_rele6, OUTPUT); // Porta da Sala
  pinMode(pino_rele7, OUTPUT); // 
  pinMode(pino_rele8, OUTPUT); // Luz da mesa

  //Desliga os reles
  digitalWrite(pino_rele1, HIGH); // Luz do Escritório
  digitalWrite(pino_rele2, HIGH); // 
  digitalWrite(pino_rele3, HIGH); // Microfone1
  digitalWrite(pino_rele4, HIGH); // Microfone2
  digitalWrite(pino_rele5, HIGH); // Portão da rua
  digitalWrite(pino_rele6, HIGH); // Porta da Sala
  digitalWrite(pino_rele7, HIGH); // 
  digitalWrite(pino_rele8, HIGH); // Luz da mesa
    
  while (!Serial) {
    ;
  }

  Ethernet.begin(mac, ip);
  server.begin();

  //Controle remoto
  pinMode(9,OUTPUT);
  digitalWrite(9, LOW);
   
}

void loop() {
  
  EthernetClient client = server.available();
  if (client) {
    
    boolean currentLineIsBlank = true;

    String vars;
    int acao = 0;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        vars.concat(c);

        //AQUI ACONTECEM AS COMPARACOES
        if (vars.endsWith("/abrirsala")) acao=1;
        else if (vars.endsWith("/destravarportao")) acao=2; 
        else if (vars.endsWith("/mic1")) acao=3;
        else if (vars.endsWith("/mic2")) acao=4;
               
        else if (vars.endsWith("/assistirtv")) acao=7;
        else if (vars.endsWith("/desligartv")) acao=8;
        else if (vars.endsWith("/globo")) acao=9;
        else if (vars.endsWith("/sbt")) acao=10;
        else if (vars.endsWith("/aumentartv")) acao=11;
        else if (vars.endsWith("/abaixartv")) acao=12;
        else if (vars.endsWith("/tvmudo")) acao=13;
        else if (vars.endsWith("/proximocanal")) acao=14;
        else if (vars.endsWith("/canalanterior")) acao=15;
        else if (vars.endsWith("/aumentando5")) acao=16;
        else if (vars.endsWith("/abaixando5")) acao=17;
        else if (vars.endsWith("/vercamera")) acao=18;
        else if (vars.endsWith("/saircamera")) acao=19;       
        else if (vars.endsWith("/luzmesa")) acao=20;
        else if (vars.endsWith("/luzescritorio")) acao=21;
        else if (vars.endsWith("/netflix")) acao=22;
        else if (vars.endsWith("/direita")) acao=23;
        else if (vars.endsWith("/esquerda")) acao=24;
        else if (vars.endsWith("/podeiniciar")) acao=25;       


        
        Serial.write(c);
        
        if (c == '\n' && currentLineIsBlank) {
         
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          if (acao == 1) // ABRIR PORTA
          {
            abrirporta();
            delay(1000);
            break;
          }
          else if (acao == 2) //DESTRAVAR PORTAO
          {            
            abrirportao();
            delay(1000);           
            break;
          }
          else if (acao == 3) //MIC1
          {             
            delay(2000);
            digitalWrite(pino_rele3, !digitalRead(pino_rele3));
            digitalWrite(pino_rele4, !digitalRead(pino_rele4));
            break;
          }          
          else if (acao == 4) //MIC2
          {             
            delay(2000);
            digitalWrite(pino_rele3, !digitalRead(pino_rele3));
            digitalWrite(pino_rele4, !digitalRead(pino_rele4));
            break;
          }
          
          else if (acao == 7){ //LIGANDO A TV
            //Ligar receptor
            irsend.sendNEC(0x8F4C13E, 32);            
            break;
          }
          else if (acao == 8){ //DESLIGANDO A TV
            //desligar receptor
            irsend.sendNEC(0x8F4C13E, 32);            
            break;
          }
          else if (acao == 9){ //globo
            //0
            irsend.sendNEC(0x8F419E6, 32);
            delay(500);
            //1
            irsend.sendNEC(0x8F4B14E, 32);
            break;
          }
          else if (acao == 10){ //sbt
            
            irsend.sendNEC(0x8F419E6, 32);
            delay(500);            
            irsend.sendNEC(0x8F431CE, 32);
            break;
          }
          else if (acao == 11){ //Aumentando o volume
            
            irsend.sendNEC(0x8F459A6, 32);            
            break;
          }
          else if (acao == 12){ //Abaixando o volume
            
            irsend.sendNEC(0x8F47986, 32);            
            break;
          }
          else if (acao == 13){ //TV Mudo
            
            irsend.sendNEC(0x8F4B946, 32);            
            break;
          }
          else if (acao == 14){ //Proximo canal
            
            irsend.sendNEC(0x8F4D926, 32);            
            break;
          }
          else if (acao == 15){ //Canal anterior
            
            irsend.sendNEC(0x8F4F906, 32);            
            break;
          }
          else if (acao == 16){ //Aumentando 5
            
            irsend.sendNEC(0x8F459A6, 32);
            delay(500);
            irsend.sendNEC(0x8F459A6, 32);
            delay(500);
            irsend.sendNEC(0x8F459A6, 32);
            delay(500);
            irsend.sendNEC(0x8F459A6, 32);
            delay(500);
            irsend.sendNEC(0x8F459A6, 32);
            delay(500);
            irsend.sendNEC(0x8F459A6, 32);           
            break;
          }
          else if (acao == 17){ //Abaixando 5
            
            irsend.sendNEC(0x8F47986, 32);
            delay(500);
            irsend.sendNEC(0x8F47986, 32);
            delay(500);
            irsend.sendNEC(0x8F47986, 32);
            delay(500);
            irsend.sendNEC(0x8F47986, 32);
            delay(500);
            irsend.sendNEC(0x8F47986, 32);
            delay(500);
            irsend.sendNEC(0x8F47986, 32);           
            break;
          }
          else if (acao == 18){ //Ver câmera
            
            irsend.sendNEC(0x20DFD02F, 32);
            delay(500);
            irsend.sendNEC(0x20DFD02F, 32);
            delay(500);
            irsend.sendNEC(0x20DF22DD, 32);                       
            break;
          }
          else if (acao == 19){ //Sair câmera
            
            irsend.sendNEC(0x20DFF00F, 32);                       
            break;
          }
          else if (acao == 20) //LUZ MESA
          {
            digitalWrite(pino_rele8, !digitalRead(pino_rele8));                     
            break;
          }else if (acao == 21) //LUZ DO ESCRITÓRIO
          {
            digitalWrite(pino_rele1, !digitalRead(pino_rele1));                                       
            break;
          }
          else if (acao == 22){ //Netflix
            
            irsend.sendNEC(0x20DF3EC1, 32);            
            break;
          }
          else if (acao == 23){ //direita
            
            irsend.sendNEC(0x20DF609F, 32);            
            break;
          }
          else if (acao == 24){ //esquerda
            
            irsend.sendNEC(0x20DFE01F, 32);            
            break;
          }
          else if (acao == 25){ //pode iniciar
            
            irsend.sendNEC(0x20DF22DD, 32);            
            break;
          }
          else
          {
            //NÃO FAZ NADA
          }
          
          break;
        }
        if (c == '\n') {
          
          currentLineIsBlank = true;
        } else if (c != '\r') {
          
          currentLineIsBlank = false;
        }// Final do if
      } //Final do if
    } // Final do while
    
    delay(1);
    
    client.stop();
  } //Final do if client

      //Leitor de RFid
      if ( ! mfrc522.PICC_IsNewCardPresent()) 
      {
        return;
      }
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) 
      {
        return;
      }
      //UID na serial  
      String conteudo= "";
      byte letra;
      for (byte i = 0; i < mfrc522.uid.size; i++) 
      {         
         conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
         conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      conteudo.toUpperCase();
      if (conteudo.substring(1) == "00 00 00 00") //Marcelo
      {
        abrirporta();    
        delay(1000);
      }
     
      else if (conteudo.substring(1) == "00 00 00 00") //Mônica
      {
        abrirporta();    
        delay(1000);
      }
      
      else if (conteudo.substring(1) == "00 00 00 00") //Batista
      {    
        abrirporta();    
        delay(1000);
      }
        
      else if (conteudo.substring(1) == "00 00 00 00") //Bruno
      {    
        abrirporta();    
        delay(1000);
      }
      
      else if (conteudo.substring(1) == "00 00 00 00") //Lourdes
      {    
        abrirporta();    
        delay(1000);
      }
      
      else if (conteudo.substring(1) == "00 00 00 00") //Cláudia
      {    
        abrirporta();    
        delay(1000);
      }
      
      else if (conteudo.substring(1) == "00 00 00 00") //Chaveiro VII
      {    
        abrirporta();    
        delay(1000);
      }
      else
      {
        Serial.print("Não abrir portão");
        delay(1000);
        
      }
}


//FUNÇÃO PARA ABRIR O PORTA
void abrirporta()
{
    digitalWrite(pino_rele6, LOW);
    delay(500);
    digitalWrite(pino_rele6, HIGH);    
}

//FUNÇÃO PARA ABRIR O PORTÃO
void abrirportao()
{   
  digitalWrite(pino_rele5, LOW);
  delay(500);
  digitalWrite(pino_rele5, HIGH);       
}
