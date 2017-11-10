#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;

const char* ssid = "********";
const char* password = "********";

char* button1 = "http://xxx.xxx.xxx.xxx:5005/";
char* button2 = "http://xxx.xxx.xxx.xxx:5005/";
char* button3 = "http://xxx.xxx.xxx.xxx:5005/";

int button1State, button2state, button3State = 0;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  pinMode(D0, INPUT); // button 1
  pinMode(D3, INPUT); // button 2
  pinMode(D4, INPUT); // button 3

  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFiMulti.addAP(ssid, password);
  
  // If there is no password:
  // WiFiMulti.addAP(ssid);
}

void loop() {
  button1State = digitalRead(D0);
  button2state = digitalRead(D3);
  button3State = digitalRead(D4);

  if (button1State == LOW) {
    makeRequest(button1);
    delay(200);
  }

  if (button2state == LOW) {
    makeRequest(button2);
    delay(200);
  }
  
  if (button3State == LOW) {
    makeRequest(button3);
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

