#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <Preferences.h>
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
#define RXp2 16
#define TXp2 17
#define WIFI_SSID "Infinix NOTE 7"
#define WIFI_PASSWORD "1346792580anas"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDh97MwB8KWgPNi28444v48DQvjE9pZLm8"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "waledkasa@gmail.com"
#define USER_PASSWORD "N0tAG00dPlace"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://cap312-ef948-default-rtdb.europe-west1.firebasedatabase.app/"

// Define Firebase objects
String apiKeyValue = "hellow world";
String sensorName = "DHT11";
String sensorLocation = "Outdoors";
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 3000;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String dataPath = "/data";

// Parent Node (to be updated in every loop)
String parentPath;

FirebaseJson json;
Preferences preferences;


void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  
  initWiFi();
   // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000); 
  }
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";
}

void loop()
{
   preferences.begin("my-app", false);
  unsigned int counter = preferences.getUInt("counter", 0);
  
  if(Firebase.ready())
  {
    String data =  "api_key=" + apiKeyValue + "&sensor=" + sensorName
                            + "&location=" + sensorLocation + "&value1=" + String((String(Serial2.readString()[14]) + String(Serial2.readString()[15])).toInt())
                            + "&value2=" + String((String(Serial2.readString()[3]) + String(Serial2.readString()[4])).toInt()) + "&value3=" + String(UV()) + "";
    Serial.println(data);
    counter++;
    Serial.printf("Current counter value: %u\n", counter);
    preferences.putUInt("counter", counter);                        
    parentPath= databasePath + "/" + String(counter);
    sendDataPrevMillis = millis();
    json.set(dataPath.c_str(), data);
    Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json);
    preferences.end();
    delay(500); 
  }
  
}
int UV(){
  String UVI;
  for (int i=28; i<Serial2.readString().length(); i++){
    UVI += (Serial2.readString()[i]);
  }
  int UVF = (UVI.toInt()/186) ; //186
  return UVF;
}
