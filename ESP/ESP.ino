#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include "soc/rtc_wdt.h"
#define RXp2 16
#define TXp2 17
const char* ssid = "LifeIsGreat";
const char* password = "N0tReally";
AsyncWebServer server(80);
/* four data types being converted to char array */
char charArr[]="char array";
String str = "this is a string";
int integer = 7683;
float decimalNumber = 768.30000;

String readDHTTemperature() {
  TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed=1;
  TIMERG0.wdt_wprotect=0;
  int t = (String(Serial2.readString()[14]) + String(Serial2.readString()[15])).toInt();
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}
String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read Humidity value from the string recieved from arduino
    int h = (String(Serial2.readString()[3]) + String(Serial2.readString()[4])).toInt(); 
    if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}
/*
String readVEMLUV() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read Humidity value from the string recieved from arduino


    int h = (String(Serial2.readString()[3]) + String(Serial2.readString()[4])).toInt(); 
    if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}*/
String UV(){
  String UVI;
  for (int i=28; i<Serial2.readString().length(); i++){
    UVI += (Serial2.readString()[i]);
  }
  return UVI;
}
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
   <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">UV</span>
    <span id="uv">%UV%</span>
    <sup class="units">&percnt;</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("UV").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/uv", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";
// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  else if(var == "uv"){
    return UV();
   }
  return String();
}
void setup() {
  Serial.begin(115200);
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });   
  server.on("/UV", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", UV().c_str());
  });  
  // Start server
  server.begin();
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Serial.println("Waiting for a client to connect....");
}
void loop() {
  /*Serial.print("Message Received: ");
  Serial.println(Serial2.readString());
  //for (int i=28; i<Serial2.readString().length(); i++) Serial.print(Serial2.readString()[i]);
  Serial.print(UV());*/
  delay(100);
}
