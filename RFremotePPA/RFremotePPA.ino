#include <RFremote.h>

RFrecv rfrecv (3);

void setup()
{
  Serial.begin(9600);
  delay(500);
  Serial.println("INICIADO!");
  rfrecv.begin();
}

void loop()
{
  
  if (rfrecv.available())
  {
    Serial.println("RECEBIDO!");
   
     Serial.println((char*)rfrecv.cmd);
  }
}
