#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <SPI.h>
#include <bitBangedSPI.h>
#include <MAX7219_Dot_Matrix.h>

#include "Arduino.h"
#include <ArduinoJson.h>

const byte chips = 8; //number of matrix in the display

// for NodeMCU 1.0
#define DIN_PIN 15  // D8
#define CS_PIN  13  // D7
#define CLK_PIN 12  // D6

MAX7219_Dot_Matrix display (chips, CS_PIN, DIN_PIN, CLK_PIN);  // 2 chips, then specify the LOAD, DIN, CLK pins

WiFiClient client;

String weatherMain = "";
String weatherDescription = "";
String weatherLocation = "";
String country;
int humidity;
int pressure;
float temp;
float tempMin, tempMax;
int clouds;
float windSpeed;
String date;
int h,m,s;
int sunrise, sunset;

String currencyRates;
String weatherString;

const char* ssid = "taifur&mafi";
const char* password = "University";

String weatherKey = "deb88f35850ad1216f787bc647c4939f";
String weatherLang = "&lang=en";
String cityID = "1185241"; //Dhaka
// read OpenWeather api description for more info
// =======================================================================

char message1 [] = "This is a testing display. Designed by Md. Khairul Alam";
char weather_message[300];
char time_message[50];
char date_message[50];

int LED = 2;

void setup() {
  Serial.begin(115200);

  display.begin ();
  display.setIntensity (1);
  Serial.print("Connecting WiFi ");
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(LED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  getWeatherData();
  String str = weatherString;
  int str_len = str.length() + 1; 
  str.toCharArray(weather_message, str_len) ;   

  getTime();
}

unsigned long lastMoved = 0;
unsigned long MOVE_INTERVAL = 2;  // mS
int  messageOffset;

void updateDisplay (char *msg)
  {
  //String message = msg;
  display.sendSmooth (msg, messageOffset);
  
  // next time show one pixel onwards
  if (messageOffset++ >= (int) (strlen (msg) * 8))
    messageOffset = - chips * 8;
  }  // end of updateDisplay
  
long last = millis();
long lastTime = millis();

void loop() {
  ArduinoOTA.handle();

  if((millis()- last)>1000*60*15){  //update weather every 15 minutes 
    getWeatherData();
    getTime();
    String str = weatherString;
    int str_len = str.length() + 1;
    str.toCharArray(weather_message, str_len) ;
    last = millis();
   }
   
  // update display if time is up
  if(s>0 && s<40){
  if (millis () - lastMoved >= MOVE_INTERVAL)
    {
      updateDisplay(weather_message);
      lastMoved = millis ();
    }  
  }

  if(s>40 && s<55){
  if (millis () - lastMoved >= MOVE_INTERVAL)
    {
      updateDisplay(date_message);
      lastMoved = millis ();
    }  
  }
  /*
  // do other stuff here    
  if(millis()-lastTime>65000){ 
    long now = millis();
    do{
      updateTime();
      showTime();
      }while(millis()-now<40000);
    now = millis();
    lastTime = millis();
  }  
   */
   if(s>55){
      showTime(); 
    }
   
 updateTime();                  // Wait for two seconds (to demonstrate the active low LED)
}

// =======================================================================
// retrive weather data

const char *weatherHost = "api.openweathermap.org";

void getWeatherData()
{
  Serial.print("connecting to "); Serial.println(weatherHost);
  if (client.connect(weatherHost, 80)) {
    client.println(String("GET /data/2.5/weather?id=") + cityID + "&units=metric&appid=" + weatherKey + weatherLang + "\r\n" +
                "Host: " + weatherHost + "\r\nUser-Agent: ArduinoWiFi/1.1\r\n" +
                "Connection: close\r\n\r\n");
  } else {
    Serial.println("connection failed");
    return;
  }
  String line;
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    Serial.println("w.");
    repeatCounter++;
  }
  while (client.connected() && client.available()) {
    char c = client.read(); 
    if (c == '[' || c == ']') c = ' ';
    line += c;
  }

  client.stop();

  DynamicJsonBuffer jsonBuf;
  JsonObject &root = jsonBuf.parseObject(line);
  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }
  //weatherMain = root["weather"]["main"].as<String>();
  weatherDescription = root["weather"]["description"].as<String>();
  weatherDescription.toLowerCase();
  Serial.println(weatherDescription);
  //  weatherLocation = root["name"].as<String>();
  //  country = root["sys"]["country"].as<String>();
  sunrise = root["sys"]["sunrise"];
  sunset = root["sys"]["sunset"];
  temp = root["main"]["temp"];
  humidity = root["main"]["humidity"];
  pressure = root["main"]["pressure"];
  tempMin = root["main"]["temp_min"];
  tempMax = root["main"]["temp_max"];
  windSpeed = root["wind"]["speed"];
  clouds = root["clouds"]["all"];
  //String deg = String(char('~')+131);
  //weatherString = "  Temp: " + String(temp,1) /*+ deg*/ + "C (" + String(tempMin,1) /*+ deg*/ + "-" + String(tempMax,1) /*+ deg*/ + ")  ";
  weatherString = "  Temp: " + String(temp,1) /*+ deg*/ + "C";
  //weatherString += weatherDescription;
  weatherString += "  Humidity: " + String(humidity) + "% ";
  weatherString += "  Pressure: " + String(pressure) + "hPa ";
  weatherString += "  Clouds: " + String(clouds) + "%  ";
  weatherString += "  Wind: " + String(windSpeed,1) + "m/s   ";
  Serial.println(weatherString);
}

// =======================================================================
// retrive curency rate

const char* currencyHost = "cinkciarz.pl";

void getCurrencyRates()
{
  WiFiClientSecure client;
  Serial.print("connecting to "); Serial.println(currencyHost);
  if (!client.connect(currencyHost, 443)) {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET / HTTP/1.1\r\n") +
               "Host: " + currencyHost + "\r\nConnection: close\r\n\r\n");

  //Serial.print("request sent");
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    Serial.println("c.");
    repeatCounter++;
  }
  Serial.println("connected");
  while (client.connected() && client.available()) {
    String line = client.readStringUntil('\n');
    //      Serial.println(line);
    int currIdx = line.indexOf("/kantor/kursy-walut-cinkciarz-pl/usd");
    if (currIdx > 0) {
      String curr = line.substring(currIdx + 33, currIdx + 33 + 3);
      curr.toUpperCase();
      line = client.readStringUntil('\n');
      int rateIdx = line.indexOf("\">");
      if (rateIdx <= 0) {
        Serial.println("Found rate but wrong structure!");
        return;
      }
      currencyRates = "        PLN/" + curr + ": ";
      if (line[rateIdx - 1] == 'n') currencyRates += char('~'+24); else currencyRates += char('~'+23); // down/up
      currencyRates += line.substring(rateIdx + 2, rateIdx + 8) + " ";

      line = client.readStringUntil('\n');
      rateIdx = line.indexOf("\">");
      if (rateIdx <= 0) {
        Serial.println("Found rate but wrong structure!");
        return;
      }
      if (line[rateIdx - 1] == 'n') currencyRates += char('~'+24); else currencyRates += char('~'+23); // down/up
      currencyRates += line.substring(rateIdx + 2, rateIdx + 8);
      currencyRates.replace(',', '.');
      break;
    }
  }
  client.stop();
}

// =======================================================================

float utcOffset = 2;
long localEpoc = 0;
long localMillisAtUpdate = 0;

void getTime()
{
  WiFiClient client;
  if (!client.connect("www.google.com", 80)) {
    Serial.println("connection to google failed");
    return;
  }

  client.print(String("GET / HTTP/1.1\r\n") +
               String("Host: www.google.com\r\n") +
               String("Connection: close\r\n\r\n"));
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    //Serial.println(".");
    repeatCounter++;
  }

  String line;
  client.setNoDelay(false);
  while(client.connected() && client.available()) {
    line = client.readStringUntil('\n');
    line.toUpperCase();
    if (line.startsWith("DATE: ")) {
      date = ""+line.substring(6, 22);
      h = line.substring(23, 25).toInt();
      m = line.substring(26, 28).toInt();
      s = line.substring(29, 31).toInt();
      localMillisAtUpdate = millis();
      localEpoc = (h * 60 * 60 + m * 60 + s);

      String clock_date = date;
      int date_len = clock_date.length() + 1; 
      clock_date.toCharArray(date_message, date_len) ;
    
    }
  }
  client.stop();
}

// =======================================================================

void updateTime()
{
  long curEpoch = localEpoc + ((millis() - localMillisAtUpdate) / 1000);
  long epoch = round(curEpoch + 3600 * utcOffset + 86400L) % 86400L;
  h = ((epoch  % 86400L) / 3600) % 24;
  m = (epoch % 3600) / 60;
  s = epoch % 60;
}

// =======================================================================
long lastmillis=millis();
void showTime()
{
    String timeString = "";
    timeString+= h/10 ? h/10 : 0;
    timeString+= h%10;
    timeString+= ":";
    timeString+= m/10;
    timeString+= m%10;
    timeString+= ":";
    timeString+= s/10;
    timeString+= s%10;   
    String clock_time = timeString;
    int clock_len = clock_time.length() + 1; 
    clock_time.toCharArray(time_message, clock_len) ;
    display.sendString(time_message);
}
