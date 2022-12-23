#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
#include <SPI.h>
#include <MFRC522.h>
constexpr uint8_t RST_PIN = 16;  
constexpr uint8_t SDA_PIN = 15;      
byte readCard[4];
String MasterTag = "73D492A7"; 
String tagID = "";
MFRC522 mfrc522(SDA_PIN, RST_PIN); 
const char* ssid     = "Subhamoy";
const char* password = "PASS020302";
WiFiClient wifiClient;
void setup() {
	Serial.begin(9600);	
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
	while (!Serial);
	SPI.begin();	
	mfrc522.PCD_Init();	
}
void loop() 
{ 
  while (getUID()) 
  {
    verify(); 
  }
}
void postid(){
  String serverName = "http://192.168.1.5:3000/";
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(wifiClient, serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "&uid="+String(tagID);
    int httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode > 0) {
      // Serial.print("HTTP Response code: ");
      //Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
  } 
  else {
    Serial.println("WiFi Disconnected");
  }
}
void getdata(){
  String serverName = "http://192.168.1.5:3000/";
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(wifiClient, serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    }
    http.end();
  } 
  else {
    Serial.println("WiFi Disconnected");
  }
}
void verify(){
  Serial.println(tagID);  
  String serverName = "http://192.168.1.5:3000/verify";
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(wifiClient, serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "&uid="+String(tagID);
    int httpCode = http.POST(httpRequestData);
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    }
    http.end();
  } 
  else {
    Serial.println("WiFi Disconnected");
  }  
}
boolean getUID() 
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {  
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {     
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {                  
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX));  
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}