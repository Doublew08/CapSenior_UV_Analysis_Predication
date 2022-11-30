#include <dht.h>
//#include <U8g2lib.h>
#include <Wire.h>
#include "Adafruit_VEML6070.h"
#define dht_apin A0 // Analog Pin sensor is connected to
Adafruit_VEML6070 uv = Adafruit_VEML6070();
dht DHT;
String convert_to_risk_level(int reading) 
{
  int integration_time = 4;   //available for Integration-Time 1, 2, 4
                              // MUST be adjusted according to the set integration time
  reading = reading / integration_time;

  String risk_level;

  if(reading <= 560)
      risk_level = "LOW (UV 0-2)";
  else if(reading > 560 && reading <= 1120)
      risk_level = "Moderate (UV 3-5)";
  else if(reading > 1120 && reading <= 1494)
      risk_level = "High (UV 6-7)";
  else if(reading > 1494 && reading <= 2054)
      risk_level = "Very High (UV 8-10)";
  else if(reading > 2054 && reading <= 9999)
      risk_level = "Extreme (UV >10)";
  else
      risk_level = "ERROR";

  return risk_level;
}
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
    Serial.println(convert_to_risk_level(uv.readUV()));
    delay(1500);//Wait 5 seconds before accessing sensor again.
 
  //Fastest should be once every two seconds.
 
}// end loop(
