#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;

const char* ssid = "Airwave-A2_1-axz1xs1w7";
char* node_server_ip = "http://172.16.51.214";

char* volumeup = "http://172.16.51.214:5005/groupVolume/+3";
char* volumedown = "http://172.16.51.214:5005/groupVolume/-3";
char* playpause = "http://172.16.51.214:5005/alpha/playpause";

int volumeupState, volumedownState, playpauseState = 0;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  pinMode(D0, INPUT); // volume up
  pinMode(D3, INPUT); // volume down
  pinMode(D4, INPUT); // playpause

  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFiMulti.addAP(ssid);
}

void loop() {
  volumeupState = digitalRead(D0);
  volumedownState = digitalRead(D3);
  playpauseState = digitalRead(D4);

  if (volumeupState == LOW) {
    makeRequest(volumeup);
    delay(200);
  }

  if (volumedownState == LOW) {
    makeRequest(volumedown);
    delay(200);
  }
  
  if (playpauseState == LOW) {
    makeRequest(playpause);
    delay(200);
  }
  
}

void makeRequest(char* command) {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(command);
    int httpCode = http.GET();
    if (httpCode != 200) {
      Serial.printf("HTTP GET error: %s\n", http.errorToString(httpCode).c_str());
    } else {
      Serial.println(http.getString());
      Serial.println(command);
    }
    http.end();
  }
}

