#include <dht.h>
//#include <U8g2lib.h>
#include <Wire.h>
#include "Adafruit_VEML6070.h"
#define dht_apin A0 // Analog Pin sensor is connected to
Adafruit_VEML6070 uv = Adafruit_VEML6070();
dht DHT;
 
void setup(){
  
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  Serial.println("VEML6070 Test");
  uv.begin(VEML6070_1_T);  // pass in the integration time constant
  delay(1000);//Wait before accessing Sensor
 
}//end "setup()"
 
void loop(){
  //Start of Program 
 
    DHT.read11(dht_apin);
    Serial.print("h= ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("t= ");
    Serial.print(DHT.temperature); 
    Serial.print("C  ");
    Serial.print("UV= ");
    Serial.println(uv.readUV());
    delay(1500);//Wait 5 seconds before accessing sensor again.
 
  //Fastest should be once every two seconds.
 
}// end loop(
