#include "DHT.h"
#include <Wire.h>
#include "Adafruit_VEML6070.h"
Adafruit_VEML6070 uv = Adafruit_VEML6070();
#define DHTPIN A0          // what digital pin we're connected to
#define DHTTYPE DHT11     // DHT11
DHT dht(DHTPIN, DHTTYPE);
void setup(){
  
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  Serial.println("VEML6070 Test");
  uv.begin(VEML6070_1_T);  // pass in the integration time constant
  dht.begin();
  delay(1000);//Wait before accessing Sensor
 
}//end "setup()"
 
void loop(){
  //Start of Program 
 
    Serial.print("h= ");
    Serial.print(dht.readHumidity());
    Serial.print("%, ");
    Serial.print("t= ");
    Serial.print(dht.readTemperature()); 
    Serial.print("°C, ");
    Serial.print("UV= ");
    Serial.println(uv.readUV());
    /*Serial.print(", ");
    Serial.println(convert_to_risk_level(uv.readUV()));*/
    delay(1500);//Wait 5 seconds before accessing sensor again.
 
  //Fastest should be once every two seconds.
 
}// end loop(
