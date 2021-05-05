/* 
TITLE:       CODE GRABBER FOR HT6P20B ENCODER
CREATED BY:  AFONSO CELSO TURCATO
DATE:        14/JAN/2014
E-MAIL:      acturcato (at) gmail.com
LICENSE:     GPL
REV.:        00
DESCRIÇÃO:
http://acturcato.wordpress.com/2014/01/14/clonagem-de-controle-remoto-rf-learning-code-ht6p20b-com-arduino/
*/

struct rfControl        //Struct for RF Remote Controls
{
   unsigned long addr;  //ADDRESS CODE
   boolean btn1;        //BUTTON 1
   boolean btn2;        //BUTTON 2
};

struct rfControl rfControl_RX;    //Set variable rfControl_RX as rfControl type.
struct rfControl rfControl_TX;    //Set variable rfControl_TX as rfControl type.

const byte pinRF_RX = 52;      //Pin where RF Receiver Module is connected. If necessary, change this for your project
const byte pinRF_TX = 53;      //Pin where RF Transmitter Module is connected. If necessary, change this for your project
const byte pinButton_TX = 2;   //Pin where "Enable transmission button" is connected. Change this field as pin used in your project
const byte pinLearnButton = 3; //Pin where "Learn button" is connected. Change this field as pin used in your project
const byte pinLearnLED = 4;    //Pin where "Learning Led" is connected 
const byte pinReceiveLED = 13; //On board Led

volatile unsigned int lambda;  // desired value for Pulse Clock Width on Transmissions cases

void myDelay(unsigned int t)
{
  delayMicroseconds(t);
}

void sendData(char data)
{
  int pulse = (int)lambda;
  
  if(data == '0')
  {
    digitalWrite(pinRF_TX, LOW);
    myDelay(pulse);
  
    digitalWrite(pinRF_TX, HIGH);
    myDelay(2 * pulse);
  }
  
  if(data == '1')
  {
    digitalWrite(pinRF_TX, LOW);
    myDelay(2 * pulse);
  
    digitalWrite(pinRF_TX, HIGH);
    myDelay(pulse);
  }
}

void sendPilotCode()
{
  //Keep pinRF on HIGH for little time
  digitalWrite(pinRF_TX, HIGH);
  delayMicroseconds(500);
  
  //Set pinRF on LOW for 23 Lambdas
  digitalWrite(pinRF_TX, LOW);
  myDelay(23 * lambda);
  
  //Set pinRF on HIGH for one Lambda
  digitalWrite(pinRF_TX, HIGH);
  myDelay(lambda);
}

void sendAddressCode(unsigned long addressCodeHEX)
{
 String addressCodeBIN = "0000000000000000000000" + String(addressCodeHEX, BIN);
 
 addressCodeBIN = addressCodeBIN.substring(addressCodeBIN.length()-22, addressCodeBIN.length());
 
 //Send all bits for Address Code
 for(int i=0; i<22; i++)
 {
   char data = addressCodeBIN[i];
   sendData(data);
 }
}

void sendDataCode(boolean btn1, boolean btn2)
{
  //Send BTN2 values
  if(btn2)
    sendData('1');
  else
    sendData('0');
    
  //Send BTN1 values
  if(btn1)
    sendData('1');
  else
    sendData('0');
}

void sendAntiCode()
{
  sendData('0');
  sendData('1');
  sendData('0');
  sendData('1');
}

void sendAllCodes()
{
 //Check if there is already a valid value stored
 if(!rfControl_TX.addr == NULL)
 {
   //Send PILOTE CODE (details: http://acturcato.wordpress.com/2014/01/04/decoder-for-ht6p20b-encoder-on-arduino-board-english/)
   sendPilotCode();
  
   //Send stored ADDRESS CODE
   sendAddressCode(rfControl_TX.addr);

   //Send stored DATA CODE of buttons
   sendDataCode(rfControl_TX.btn1, rfControl_TX.btn2);
 
   //Send a valid ANTI-CODE
   sendAntiCode();
 }
}

boolean receiveAllCodes(struct rfControl &_rfControl)
{ 
  static boolean startbit;      //checks if start bit was identified
  static int counter;           //received bits counter: 22 of Address + 2 of Data + 4 of EndCode (Anti-Code)
  static unsigned long buffer;  //buffer for received data storage
  
  int lambda;      // on pulse clock width (if fosc = 2KHz than lambda = 500 us)
  int dur0, dur1;  // pulses durations (auxiliary)
  
  if (!startbit)
  {// Check the PILOT CODE until START BIT;
    dur0 = pulseIn(pinRF_RX, LOW);  //Check how long DOUT was "0" (ZERO) (refers to PILOT CODE)
        
    //If time at "0" is between 9200 us (23 cycles of 400us) and 13800 us (23 cycles of 600 us).
    if((dur0 > 9200) && (dur0 < 13800) && !startbit)
    {    
      //calculate wave length - lambda
      lambda = dur0 / 23;
      
      //Reset variables
      dur0 = 0;
      buffer = 0;
      counter = 0;
      
      startbit = true;
    }
  }

  //If Start Bit is OK, then starts measure os how long the signal is level "1" and check is value is into acceptable range.
  if (startbit && counter < 28)
  {
    ++counter;
    
    dur1 = pulseIn(pinRF_RX, HIGH);
    
    if((dur1 > 0.5 * lambda) && (dur1 < (1.5 * lambda)))  //If pulse width at "1" is between "0.5 and 1.5 lambda", means that pulse is only one lambda, so the data é "1".
    {
      buffer = (buffer << 1) + 1;   // add "1" on data buffer
    }
    else if((dur1 > 1.5 * lambda) && (dur1 < (2.5 * lambda)))  //If pulse width at "1" is between "1.5 and 2.5 lambda", means that pulse is two lambdas, so the data é "0".
    {
      buffer = (buffer << 1);       // add "0" on data buffer
    }
    else
    {
      //Reset the loop
      startbit = false;
    }
  }
  
  //Check if all 28 bits were received (22 of Address + 2 of Data + 4 of Anti-Code)
  if (counter==28) 
  { 
    // Check if Anti-Code is OK (last 4 bits of buffer equal "0101")
    if ((bitRead(buffer, 0) == 1) && (bitRead(buffer, 1) == 0) && (bitRead(buffer, 2) == 1) && (bitRead(buffer, 3) == 0))
    {     
      counter = 0;
      startbit = false;
      
      //Get ADDRESS CODE from Buffer
      _rfControl.addr = buffer >> 6;
      
      //Get Buttons from Buffer
       _rfControl.btn1 = bitRead(buffer,4);
       _rfControl.btn2 = bitRead(buffer,5);
       
      //If a valid data is received, return OK
      return true;
    }
    else
    {
      //Reset the loop
      startbit = false;
    }
  }
  
  //If none valid data is received, return NULL and FALSE values 
  _rfControl.addr = NULL;
  _rfControl.btn1 = NULL;
  _rfControl.btn2 = NULL; 
  
  return false;
}

void setup()
{
  //Change this value according to the values ​​accepted by your RH receiver. In my tests, the values ​​between 320 and 420 were OK!
  lambda = 350;
  
  pinMode(pinButton_TX, INPUT);
  pinMode(pinRF_RX, INPUT);
  pinMode(pinRF_TX, OUTPUT);
  
  pinMode(pinLearnButton, INPUT);
  pinMode(pinLearnLED, OUTPUT);
   
  pinMode(pinReceiveLED, OUTPUT);
  
  //Set de interrupt mode (See: http://arduino.cc/en/Reference/attachInterrupt for more details)
  attachInterrupt(pinButton_TX - 2, sendAllCodes, LOW);
  
  //Initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  //Start pinRF_TX on LOW (disable transmissions)
  digitalWrite(pinRF_TX, LOW);
  
  //Wait a second for RF modules startup
  delay(1000);
}

void loop()
{
  //Disables transmissions
  digitalWrite(pinRF_TX, LOW);
  
  //Blink Received LED when receive something
  digitalWrite(pinReceiveLED, digitalRead(pinRF_RX));
  
  //Check if Learn Button was pressed
  if(!digitalRead(pinLearnButton) && !digitalRead(pinLearnLED))
  {
    digitalWrite(pinLearnLED, HIGH);
    Serial.println("#Learning Mode ENABLED! Reset the board to cancel...\n");
  }
   
  if(receiveAllCodes(rfControl_RX))
  {   
    //If a valid data is received, print ADDRESS CODE and Buttons values    
    Serial.println("#Received data:");
    Serial.print("\tAddress: "); Serial.println(rfControl_RX.addr, HEX);
    Serial.print("\tButton1: "); Serial.println(rfControl_RX.btn1, BIN);
    Serial.print("\tButton2: "); Serial.println(rfControl_RX.btn2, BIN);
    Serial.println();
    
    //If Learning Mode is enabled, store data.
    if(digitalRead(pinLearnLED))
    {
      //Set values to "clone"
      rfControl_TX.addr = rfControl_RX.addr;
      rfControl_TX.btn1 = rfControl_RX.btn1;
      rfControl_TX.btn2 = rfControl_RX.btn2;
      
      Serial.print("#The address "); Serial.print(rfControl_RX.addr, HEX); Serial.println(" has been grabbed!\n");
    
      digitalWrite(pinLearnLED, LOW);
      
      Serial.println("#Learning Mode DISABLED!\n");
    }
  }
}
