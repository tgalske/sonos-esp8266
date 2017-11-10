// prints the MAC address of ESP-12E
// useful if ISP requires MAC address to allow Internet access

#include <ESP8266WiFi.h>
 
void setup(){
	Serial.begin(115200);
	delay(500);

	Serial.println();
	Serial.print("MAC: ");
	Serial.println(WiFi.macAddress());
}
 
void loop(){}
