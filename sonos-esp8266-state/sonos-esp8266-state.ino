/*
prints information on current song every ten seconds
requires ArduinoJson library to parse JSON response
configure line 20 with IP of node server
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

const char* ssid = "********";
const char* password = "********";

char* state = "http://xxx.xxx.xxx.xxx:5005/state";

void setup() {
	USE_SERIAL.begin(115200);
	// USE_SERIAL.setDebugOutput(true);
	
	USE_SERIAL.println();
	
	for (uint8_t t = 4; t > 0; t--) {
		USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
		USE_SERIAL.flush();
		delay(1000);
	}
	
	WiFiMulti.addAP(ssid, password);
  
  // If there is no password:
  // WiFiMulti.addAP(ssid);
}

void loop() {
	// wait for WiFi connection
	if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    http.begin(state); //HTTP

    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
		// HTTP header has been send and Server response header has been handled
		USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
		
		// file found at server
		if (httpCode == HTTP_CODE_OK) {
			String payload = http.getString();
			USE_SERIAL.println(payload); // prints full details

			// print artist name
			int payload_len = payload.length() + 1;
			char char_array[payload_len];
			payload.toCharArray(char_array, payload_len);

			StaticJsonBuffer<1000> JSONBuffer;   //Memory pool
			JsonObject& parsed = JSONBuffer.parseObject(char_array);

			if (!parsed.success()) {   //Check for errors in parsing
				Serial.println("Parsing failed");
			} else {
				const char * artist = parsed["currentTrack"]["artist"];
				Serial.println(artist);
			}
		}
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  delay(10000);
}

