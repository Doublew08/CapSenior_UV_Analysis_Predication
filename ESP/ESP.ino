#include <WiFiClient.h>
#include <HTTPClient.h>
#include "WiFi.h"
#define RXp2 16
#define TXp2 17
//-------------------------------------------------------------------
//enter WIFI credentials
const char* ssid     = "LifeIsGreat";
const char* password = "N0tReally";
//-------------------------------------------------------------------
//enter domain name and path
//http://www.example.com/sensordata.php
const char* SERVER_NAME = "http://cap312.byethost12.com//sensordata.php";

//PROJECT_API_KEY is the exact duplicate of, PROJECT_API_KEY in config.php file
//Both values must be same
String PROJECT_API_KEY = "hello world";
//-------------------------------------------------------------------
//Send an HTTP POST request every 30 seconds
unsigned long lastMillis = 0;
long interval = 5000;
//-------------------------------------------------------------------

/*
 * *******************************************************************
 * setup() function
 * *******************************************************************
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Serial.println("esp32 serial initialize");
  WiFi.begin(ssid, password);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 5 seconds (timerDelay variable),");
  Serial.println("it will take 5 seconds before publishing the first reading.");
  //-----------------------------------------------------------------

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Message Received: ");
  String data = Serial2.readString();
  Serial.println(data);
  //-----------------------------------------------------------------
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    if(millis() - lastMillis > interval) {
       //Send an HTTP POST request every interval seconds
       upload_temperature();  
    }
  }
  //-----------------------------------------------------------------
  else {
    Serial.println("WiFi Disconnected");
  }
  //-----------------------------------------------------------------

  delay(1000);  
}
int UV(){
  String UVI;
  for (int i=28; i<Serial2.readString().length(); i++){
    UVI += (Serial2.readString()[i]);
  }
  int UVF = (UVI.toInt()/186) ; //186
  return UVF;
}
void upload_temperature()
{ 
  //--------------------------------------------------------------------------------
  //Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  //Read temperature as Celsius (the default)
  float t = (String(Serial2.readString()[14]) + String(Serial2.readString()[15])).toInt();
  float h = (String(Serial2.readString()[3]) + String(Serial2.readString()[4])).toInt();
  float UVI = UV();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  //Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);
  //float uv = 
  //--------------------------------------------------------------------------------
  //°C
  
  String humidity = String(h);
  String temperature = String(t);
  String UV = String(UVI);

  Serial.println("Temperature: "+temperature);
  Serial.println("Humidity: "+humidity);
  Serial.println("UV: "+UV);
  Serial.println("--------------------------");
  //--------------------------------------------------------------------------------
  //HTTP POST request data
  String temperature_data;
  temperature_data = "api_key="+PROJECT_API_KEY;
  temperature_data += "&temperature="+temperature;
  temperature_data += "&humidity="+humidity;
  temperature_data += "&UV="+UV;

  Serial.print("temperature_data: ");
  Serial.println(temperature_data);
  //--------------------------------------------------------------------------------
  
  WiFiClient client;
  HTTPClient http;

  http.begin(client, SERVER_NAME);
  // Specify content-type header  
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Send HTTP POST request
  int httpResponseCode = http.POST(temperature_data);
  //--------------------------------------------------------------------------------
  // If you need an HTTP request with a content type: 
  //application/json, use the following:
  //http.addHeader("Content-Type", "application/json");
  //temperature_data = "{\"api_key\":\""+PROJECT_API_KEY+"\",";
  //temperature_data += "\"temperature\":\""+temperature+"\",";
  //temperature_data += "\"humidity\":\""+humidity+"\"";
  //temperature_data += "}";
  //int httpResponseCode = http.POST(temperature_data);
  //--------------------------------------------------------------------------------
  // If you need an HTTP request with a content type: text/plain
  //http.addHeader("Content-Type", "text/plain");
  //int httpResponseCode = http.POST("Hello, World!");
  //--------------------------------------------------------------------------------
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
    
  // Free resources
  http.end();
  }
/*
PassDB : h_Tjhbz-af535+r
NameDB : 4224859_capdb  
UserDB : 4224859_capdb 
Host : fdb28.awardspace.net 
*/
