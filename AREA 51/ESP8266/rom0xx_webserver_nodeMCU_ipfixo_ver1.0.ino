#include <DHT12.h>
#include <ESP8266WiFi.h>

#define DHTTYPE DHT12

DHT12 dht12;
//Cria um server na porta 80 (porta padrÃ£o para onde os navegadores enviam as requisiÃ§Ãµes http)
WiFiServer server(80);

const int pinLED = 5;
static char temperatura[7];
static char humidity[7];

String HTTP_req;
String URLValue;

const byte qtdePinosDigitais = 6;
byte pinosDigitais[qtdePinosDigitais] = { 4     , 5     , 12     , 13     , 14     , 15};
byte modoPinos[qtdePinosDigitais]     = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT};

const byte qtdePinosAnalogicos = 2;
byte pinosAnalogicos[qtdePinosAnalogicos] = {A0, A1};

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("Conectando a rede WiFi...");
  WiFi.begin(")o,0(", "riko1234");
  delay(1000);

  //Enquanto o ESP nÃ£o se conectar Ã  rede
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  //ConfiguraÃ§Ãµes do IP fixo. VocÃª pode alterar conforme a sua rede
  IPAddress ip(192, 168, 0, 100);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  Serial.print("Configurando IP do ESP8166 para : ");
  Serial.println(ip);

  //Envia a configuraÃ§Ã£o
  WiFi.config(ip, gateway, subnet);

  //Inicializa o server que criamos na porta 80
  server.begin();

  Serial.println("");
  Serial.println("NodeMCU conectado com sucesso!");
  delay (1000);
  // Inicia o servidor WEB
  server.begin();
  Serial.println("Servidor iniciado");
  delay(1000);

  // Mostra o endereco IP
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  dht12.begin();
   pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  //Deixa o GPIO0 e o GPIO2 com saÃ­da LOW
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);

    }
}

void loop()
{
  //Verifica se algum cliente estÃ¡ tentando se conectar
  WiFiClient client = server.available();
  if (!client)
  {
    //Se nÃ£o houver nenhum cliente podemos retornar pois nÃ£o hÃ¡ nada a fazer
    return;
  }

  Serial.println ("Novo Cliente");
  //Fazemos a leitura da requisiÃ§Ã£o
  String req = client.readStringUntil('\r');
  Serial.print("RequisiÃ§Ã£o: ");
  Serial.println(req);

  /*Este Ã© o html que iremos retornar para o cliente
     Ã‰ composto basicamente de dois botÃµes (ON e OFF) para o GPIO0 e dois botÃµes (ON e OFF) para o GPIO2
     A parte que mais interessa é <a href=' com a aÃ§Ã£o vinculada a cada botÃ£o
     Quando clicamos em um destes botÃµes essa informaÃ§Ã£o chegarÃ¡ atÃ© o ESP para que ele verifique qual aÃ§Ã£o deve executar
     A parte dentro de '<style>' Ã© apenas para modificarmos o visual da pÃ¡gina que serÃ¡ exibida, vocÃª pode alterÃ¡-la como queira

  */

  String html =
    "<html>"
    "<head>"
    "<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>"
    "<title>ESP8266</title>"
    "<style>"
    "body{"
    "text-align: center;"
    "font-family: sans-serif;"
    "font-size:14px;"
    "padding: 25px;"
    "}"

    "p{"
    "color:#444;"
    "}"

    "button{"
    "outline: none;"
    "border: 2px solid #1fa3ec;"
    "border-radius:18px;"
    "background-color:#FFF;"
    "color: #1fa3ec;"
    "padding: 10px 50px;"
    "}"

    "button:active{"
    "color: #fff;"
    "background-color:#1fa3ec;"
    "}"
    "</style>"
    "</head>"
    "<body>"
    "<p>GPIO0</p>"
    "<p><a href='?acao=gpio0On'><button>ON</button></a></p>"
    "<p><a href='?acao=gpio0Off'><button>OFF</button></a></p>"
    "<p>GPIO2</p>"
    "<p><a href='?acao=gpio2On'><button>ON</button></a></p>"
    "<p><a href='?acao=gpio2Off'><button>OFF</button></a></p>"
    "</body>"
    "</html>";

  //Escreve o html no buffer que serÃ¡ enviado para o cliente
  client.print(html);
  //Envia os dados do buffer para o cliente
  client.flush();

  //Verifica se a requisiÃ§Ã£o possui a aÃ§Ã£o gpio0On
  if (req.indexOf("acao=gpio0On") != -1)
  {
    //Se possui a aÃ§Ã£o gpio0On colocamos a saÃ­da do GPIO0 como alta
    digitalWrite(0, HIGH);
  }
  //SenÃ£o, verifica se a requisiÃ§Ã£o possui a aÃ§Ã£o gpio0Off
  else if (req.indexOf("acao=gpio0Off") != -1)
  {
    //Se possui a aÃ§Ã£o gpio0Off colocamos a saÃ­da do GPIO0 como baixa
    digitalWrite(0, LOW);
  }
  //SenÃ£o, verifica se a requisiÃ§Ã£o possui a aÃ§Ã£o gpio2On
  else if (req.indexOf("acao=gpio2On") != -1)
  {
    //Se possui a aÃ§Ã£o gpio2On colocamos a saÃ­da do GPIO2 como alta
    digitalWrite(2, HIGH);
  }
  //SenÃ£o, verifica se a requisiÃ§Ã£o possui a aÃ§Ã£o gpio2Off
  else if (req.indexOf("acao=gpio2Off") != -1)
  {
    //Se possui a aÃ§Ã£o gpio0Off colocamos a saÃ­da do GPIO2 como baixa
    digitalWrite(2, LOW);
  }

  //Fecha a conexÃ£o com o cliente
  client.stop();
  Serial.println("Encerrada a conexão!");
}
