#define LED_Amarelo 5 //Define LED_Amarelo como 5
#define LED_Vermelho 6 //Define LED_Vermelho como 6
#define Lamp 7 //Define Lamp como 7

#include <Thermistor.h> //Inclui a biblioteca Thermistor.h

#include <SPI.h> //Inclui a biblioteca SPI.h
#include <Ethernet.h> //Inclui a biblioteca Ethernet.h


Thermistor temp(5); //Instância a biblioteca Thermistor.h para ser utilizado o pino A5


// Configurações para o Ethernet Shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x83, 0xEA }; // Entre com o valor do MAC

IPAddress ip(192,168,1,2); // Configure um IP válido 
byte gateway[] = { 192 , 168, 1, 1 }; //Entre com o IP do Computador onde a Câmera esta instalada
byte subnet[] = { 255, 255, 255, 0 }; //Entre com a Máskara de Subrede
EthernetServer server(80); //Inicializa a biblioteca EthernetServer com os valores de IP acima citados e configura a porta de acesso(80)
//======================================================================================

byte COD = B1000;

String A1_carga = "LED Amarelo Blink"; //Função do primeiro botão
String A2_carga = "LED Vermelho Fade"; //Função do segundo botão
String A3_carga = "Lampada"; //Função do terceiro botão

boolean A1_estado=false; //Variável para armazenar o estado do primeiro botão
boolean A2_estado=false; //Variável para armazenar o estado do segundo botão
boolean A3_estado=false; //Variável para armazenar o estado do terceiro botão
       
long anteriorMillis = 0; //Variável para auxiliar no timer do BLINK do LED Amarelo      
long intervalo = 1000; //Variável para armazenar a frequência do BLINK do LED Amarelo   

int brilhoFade = 0;    // Variável para armazenar o valor do brilho do FADE do LED Vermelho
int degrauFade = 5;    // Variável para armazenar o valor do degrau do FADE do LED Vermelho


void setup()
{

  pinMode(LED_Amarelo,OUTPUT); //Define o pino 5 como saída
  pinMode(LED_Vermelho,OUTPUT); //Define o pino 6 como saída
  pinMode(Lamp,OUTPUT); //Define o pino 7 como saída
  Ethernet.begin(mac, ip);// Inicializa o Server com o IP e Mac atribuido acima

}



void loop()
{

  acionamentos(); //Vai para a função que executa o acionamento dos botões

  int temperature = temp.getTemp(); //Pega a temperatura ambiente já convertida em graus celcius e armazena na variável temperature

  EthernetClient client = server.available();// Verifica se tem alguém conectado

  if (client)
  {

    boolean currentLineIsBlank = true; // A requisição HTTP termina com uma linha em branco Indica o fim da linha
    String valPag;

    while (client.connected())
    {

      if (client.available())
      {

        char c = client.read(); //Variável para armazenar os caracteres que forem recebidos
        valPag.concat(c); // Pega os valor após o IP do navegador ex: 192.168.1.2/0001        

        //Compara o que foi recebido
        if(valPag.endsWith("0001")) //Se o que for pego após o IP for igual a 0001
        {
          COD = COD ^ B0001; //Executa a lógica XOR entre a variável atual de COD e o valor B0001
          A1_estado = !A1_estado; //Inverte o estado do primeiro acionamento
        }
        
        else if(valPag.endsWith("0010")) //Senão se o que for pego após o IP for igual a 0010
        {
          COD = COD ^ B0010; //Executa a lógica XOR entre a variável atual de COD e o valor B0010
          A2_estado = !A2_estado; //Inverte o estado do segundo acionamento

        }
        
        else if(valPag.endsWith("0100")) //Senão se o que for pego após o IP for igual a 0100
        {
          COD = COD ^ B0100; //Executa a lógica XOR entre a variável atual de COD e o valor B0010
          A3_estado = !A3_estado;  //Inverte o estado do terceiro acionamento      
        }
        //=========================================================================================================================


        if (c == '\n' && currentLineIsBlank)
        {
          //Inicia página HTML
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.print("<HTML> ");
          client.println("<BR><center></B></I></U><a href=\"http://www.labdegaragem.com\"> <img src=http://www.labdegaragem.com.br/logo.png></a></B></I></U></center>");
          //=========================================================================================================================
          
          //Display da Temperatura
          if(COD>=B1000)
          {
            client.print("<BR>");

            client.print("<center>Temperatura do Sistema:  <font size=7>  <font color=\"#ff6600\">  ");
            client.print(temperature);
            client.print("*C </font></font></center>");
          }
          //=========================================================================================================================

          client.print("<BR><BR>");

          //Primeiro BOTAO
          client.print("<center><button onclick=\"window.location.href='http://192.168.1.2/0001'\">\0</button> > Codigo: 0001 > ");
          if(A1_estado)
          {            
            client.print("<B><span style=\"color: #00ff00;\">");   
            client.print(A1_carga);
            client.print(" - ON");     
            client.print("</span></B></center>");
          }         
          else
          {
            client.print("<B><span style=\"color: #ff0000;\">");
            client.print(A1_carga);
            client.print(" - OFF");
            client.print("</span></B></center>");
          }
          //=========================================================================================================================


          //Segundo BOTAO
          client.print("<center><button onclick=\"window.location.href='http://192.168.1.2/0010'\">\0</button> > Codigo: 0010 > ");
          if(A2_estado)
          {
            client.print("<B><span style=\"color: #00ff00;\">");
            client.print(A2_carga);
            client.print(" - ON");
            client.print("</span></B></center>");
          }
          else
          {
            client.print("<B><span style=\"color: #ff0000;\">");
            client.print(A2_carga);
            client.print(" - OFF");
            client.print("</span></B></center>");
          }
          //=========================================================================================================================

          
          //Terceiro BOTAO
          client.print("<center><button onclick=\"window.location.href='http://192.168.1.2/0100'\">\0</button> > Codigo: 0100 > ");
          if(A3_estado)
          {
            client.print("<B><span style=\"color: #00ff00;\">");
            client.print(A3_carga);
            client.print(" - ON");
            client.print("</span></B></center> ");
          }
          else
          {
            client.print("<B><span style=\"color: #ff0000;\">");
            client.print(A3_carga);
            client.print(" - OFF");
            client.print("</span></B></center> ");
          } 
          //=========================================================================================================================      

          client.print("<BR><BR>");

          //COD ATUAL
          client.print("<center>COD ATUAL: <B><font size=7> ");
          if(COD>=B1000)
          {
            client.print(COD,BIN);
            client.print("b</center></font></B>");
          }
          else
          {
            client.print(COD,HEX);
            client.print("h</center></font></B>");
          }
          //=========================================================================================================================

          client.print(" <meta http-equiv=\"refresh\" content=\"5; url=http://192.168.1.2/\"> ");

          client.println("</HTML>");

          break;

        } //Fecha if (c == '\n' && currentLineIsBlank)
        
      } //Fecha if (client.available())
      
    } //Fecha While (client.connected())
    
    delay(3);// Espera um tempo para o navegador receber os dados
    client.stop(); // Fecha a conexão
    
  } //Fecha if(client)
  
} //Fecha loop



void acionamentos()
{ //Abre função acionamento()

  //LED Amarelo - BLINK
  //Baseado no sketch BlinkWithoutDelay, disponível em File > Examples > Digital > BlinkWithouDelay
  if(A1_estado) //Se o botão da página estiver em estado ON
  {
    
    unsigned long atualMillis = millis(); //Armazena o tempo decorrido desde que o programa comecou a ser rodado

    if(atualMillis - anteriorMillis > intervalo) //Se o tempo decorrido for maior que 1000 milissegundos
    {
      anteriorMillis = atualMillis; //Armazena o valor atual de Millis em anteriorMillis
      digitalWrite(LED_Amarelo, !digitalRead(5)); //Inverte o estado do LED
    }

  }

  else digitalWrite(LED_Amarelo,LOW); //Senão, quan apaga o LED Amarelo
  //=========================================================================================================================


  //LED Vermelho - FADE
  if(A2_estado)
  {
    
    analogWrite(LED_Vermelho, brilhoFade); //Altera o valor de PWM no pino 6
    brilhoFade = brilhoFade + degrauFade; //Soma na variável brilhoFade o valor do degrau

    if (brilhoFade == 0 || brilhoFade == 255) degrauFade = -degrauFade ; //Se o brilho Fade chega no máximo ou no mínimo...
                                                                         //Inverte o sentido de intensidade do Fade    

    delay(15); //Aguarda 15 milissegundos   
    
  }

  else digitalWrite(LED_Vermelho,LOW);
  //=========================================================================================================================

  //Lampada (ON/OFF)
  if(A3_estado) digitalWrite(Lamp,HIGH);
  else digitalWrite(Lamp,LOW);
  //=========================================================================================================================

}



