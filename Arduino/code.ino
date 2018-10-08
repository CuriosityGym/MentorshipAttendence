#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>
#include <MD5Builder.h>
#define PIN D1
constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D8;     // Configurable, see typical pin layout above 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 
const char* location_ID= "5678";
const char* heroku_URL = "mentorshipattendence.herokuapp.com";
MD5Builder _md5;
int salt=5,q;
unsigned long w;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN, NEO_GRB + NEO_KHZ800);

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  WiFiManager wifiManager;
  //wifiManager.resetSettings();   //reset saved settings
  //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));  //set custom ip for portal
  wifiManager.autoConnect("Mentee_Attendance");
  Serial.println("connected:)");
  pinMode(D2,OUTPUT);
  strip.begin();
  strip.setPixelColor(0, 255, 0, 0);
  strip.show();
}
 
void loop() {
  if(q==1 && (millis()-w)>2000) {
    strip.setPixelColor(0, 255, 0, 0);
    strip.show();
    noTone(D2);
    q=0;
    }
  if (  rfid.PICC_IsNewCardPresent() &&  rfid.PICC_ReadCardSerial())    // Look for new cards
  {
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(F("The NUID tag is:"));
  Serial.print(F("In hex: "));
  //printHex(rfid.uid.uidByte, rfid.uid.size);
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i]);
  }
  Serial.println();
  tone(D2,4000);
  strip.setPixelColor(0, 0, 255, 0);
  strip.show();
  q=1;
  w=millis();
  hit_url(rfid.uid.uidByte,rfid.uid.size);     
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  }
}

void hit_url(byte *UID, byte buffersize){
  WiFiClient client;
  // Set the http Port
  const int httpPort = 80;
  String s="";
  for(int i=0;i<buffersize;i++){
    s+= UID[i];
    }
    s+=salt;
    s=md5(s);
   if (!client.connect(heroku_URL, httpPort))  // Make sure we can connect
      {
         return;
         Serial.println("not connected");
      } 
    String url = "/" + s + "/" + String(location_ID); // URL for the request
   // Set some values for the JSON data depending on which event has been triggered
    //String value_1 = "MH 37T 5770";
    //String value_2 = String(h);
    //String value_3 = "";
    // Build JSON data string
     String data = "";
     //data = data + "\n" + "{\"value1\":\"" + value_1 + "\"}"; //,"value2\":\""+ value_2 +"\"}";
    if (client.connect(heroku_URL, httpPort))
           {

             // Sent HTTP POST Request with JSON data
             client.println("POST " + url + " HTTP/1.1");
             Serial.println("POST " + url + " HTTP/1.1");
             client.println("Host: " + String(heroku_URL));
             Serial.println("Host: " + String(heroku_URL));
             client.println("User-Agent: Arduino/1.0");
             Serial.println("User-Agent: Arduino/1.0");
             client.print("Accept: *");
             Serial.print("Accept: *");
             client.print("/");
             Serial.print("/");
             client.println("*");
             Serial.println("*");
             client.print("Content-Length: ");
             Serial.print("Content-Length: ");
             client.println(data.length());
             Serial.println(data.length());
             client.println("Content-Type: application/json");
             Serial.println("Content-Type: application/json");
             client.println("Connection: close");
             Serial.println("Connection: close");
             client.println();
             Serial.println();
             client.println(data);
             Serial.println(data); 
           }
  }

String md5(String str) {
  _md5.begin();
  _md5.add(String(str));
  _md5.calculate();
  return _md5.toString();
}
