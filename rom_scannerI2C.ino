/*
  ┌▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀┐
  +                   PROGRAMA CASA INTELIGENTE                                                             +
  └▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀┘
  AUTOR: HIGO RICARDO
     
                
  ( )     ( )            _
  | |_ _ _| |  ( )     /'_'\
  | |_ _ _| |  ( )    ( (_) ) 
  | |     | |  (_)     \___/  
  (_)     (_)      
         
 (_   _)( )                  ( )       ( )   ( )           ( )
  | |  | |__     _ _   ___  | |/')    `\`\_/'/'_    _   _ | |
  | |  |  _ `\ /'_` )/' _ `\| , <       `\ /'/'_`\ ( ) ( )| |
  | |  | | | |( (_| || ( ) || |\`\       | |( (_) )| (_) || |
  (_)  (_) (_)`\__,_)(_) (_)(_) (_)      (_)`\___/'`\___/'(_)
              ~_^ For downloading the file! ^_~           (_)
  =======================================================================================================
               Acesse a página web: https://hgrom.edu.br

*/
#include <Wire.h>
#include <DHT12.h>

DHT12 dht12;
/* Caso queira utilizar o referido módulo na interface I2C (portas digitais):
   Pino 1 do DHT12 deve ser conectado ao pino 5v da Interface I2C
   Pino 2 do DHT12 deve ser conectado pino SDA da Interface I2C
   Pino 3 do DHT12 deve ser conectao ao GND da Interface I2C
   Pino 4 do DHT12 deve ser conectado SCL da Interface I2C
*/

/* Caso queira utilizar portas analógicas somente:
   Pino 1 do DHT12 deve ser conectado ao pino 5v do Arduino
   Pino 2 do DHT12 deve ser conectado a porta A0 do Arduino
   Pino 3 do DHT12 deve ser conectao ao GND do Arduino
   Pino 4 do DHT12 deve ser conectado ao pino A1 do Arduino
*/

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  dht12.begin();
  Serial.println("Verificando status do sistema!");
  Serial.print("Aguarde alguns segundos...");
  if (!dht12.begin()) {
    Serial.println("O DHT12 não foi encontrado!");
    Serial.println("Ckeck to make sure SD card");
    Serial.println (":(");
  }

  delay(2000);

}

void scanner (void)
{
   /* O algoritmo abaixo a seguir procura dispositivo i2c e retorna endereço x0 do mesmo, caso haja algum. */
  bool i2cRead = true;
  if (i2cRead) {
    Serial.println("\n Carregando o Scanner i2C...");
    delay(1000);
    byte error, address;
    int nDevices;
    nDevices = 0;
    Serial.println("  Varredura concluída...");
    delay(1000);
    for (address = 1; address < 127; address++ )
    {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0)
      {

        Serial.print("Dispositivo I2C encontrado no endereço 0x");
        if (address < 16)
          Serial.print("0");
        Serial.print(address, HEX);
        Serial.println("  !");
        nDevices++;
        delay(1000);
      }
      else if (error == 4)
      {
        Serial.print("Nenhum dispositivo foi localizado!");
        if (address < 16)
          Serial.print("0");
        Serial.println(address, HEX);
        delay(1000);
      }
    }
    i2cRead = false;
    delay(3000);

void loop()
{
scanner ()
  float umidade = dht12.readHumidity();
  float celsius = dht12.readTemperature();
  float fahrenheit = dht12.readTemperature(true);
  bool dht12Read = true;

  if (isnan(umidade) || isnan(celsius) || isnan (fahrenheit)) {
    Serial.println("O sensor DHT 12 não foi encontrado!");
    dht12Read = false;
  }
  if (dht12Read) {
    Serial.println("Umidade: ");
    Serial.print(umidade);
    delay(1000);
    Serial.print(" %\n");
    Serial.println("Temperatura: ");
    Serial.print(celsius);
    Serial.println(" *C ");
    delay(1000);
    Serial.print(fahrenheit);
    Serial.print(" *F ");
    delay(1000);
  }
  delay (5000);
}
