#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define USE_SERIAL Serial


ESP8266WiFiMulti WiFiMulti;

void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }
  WiFiMulti.addAP("Airwave-A2_1-axz1xs1w7");
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    http.begin("http://172.16.51.214:5005/state"); //HTTP

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
        USE_SERIAL.println(payload);


        int payload_len = payload.length() + 1;

        char char_array[payload_len];

        payload.toCharArray(char_array, payload_len);

        StaticJsonBuffer<1000> JSONBuffer;   //Memory pool
        JsonObject& parsed = JSONBuffer.parseObject(char_array);

        if (!parsed.success()) {   //Check for errors in parsing
          Serial.println("Parsing failed");
        }

        const char * state = parsed["playbackState"];
        const char * artist = parsed["currentTrack"]["artist"];

        if (artist = "Kenny Chesney") {
          http.begin("http://172.16.51.214:5005/groupVolume/15");
          http.GET();
        }

        if (state == "PLAYING") {
          // TO DO: call pause()
          Serial.println(state);
        } else {
          // TO DO: call play()
          Serial.println(state);
        }
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  delay(10000);
}

