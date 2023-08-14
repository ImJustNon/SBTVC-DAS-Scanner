#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WiFi config
const char* ssid = "Galaxy A24";
const char* password = "gulnwzamakmak";

// api config
const char* serverUrl = "http://45.141.27.54:1756/api/bridge/sik/api/status/change";

// Pin config
const int RelayPin = D7;

void setup() {
  Serial.begin(9600);
  
  // setup pinmode
  pinMode(RelayPin, INPUT_PULLUP);

  // connect wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop() {
  // wifi status check
  if (WiFi.status() == WL_CONNECTED) {
    // read input value
    int SwitchValue = digitalRead(RelayPin);
    if(SwitchValue == 1){ // if switch is currently on or it mean washing machine is working
      updateStatus(1, 1);
      // SwitchValue = 0;
    }
    else {
      updateStatus(1, 0);
      // SwitchValue = 1;
    }
  }
  
  // Send the request every 25 seconds
  delay(10 * 1000); 
}


void updateStatus(int machine_on, int change_to){
  WiFiClient client; // Use WiFiClient for HTTP
  HTTPClient http;

  http.setTimeout(30000); // 10 seconds

  // Construct the JSON payload
  DynamicJsonDocument jsonDoc(256);
  jsonDoc["machine_no"] = String(machine_on);
  jsonDoc["change_to"] = String(change_to);
  
  String payload;
  serializeJson(jsonDoc, payload);

  http.begin(client, serverUrl); // Use the client with the URL
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response Code: " + String(httpResponseCode));
    Serial.println("HTTP Response: " + response);
    
    // const char* Const_response_data = response.c_str();
    // const char* SUCCESS = "SUCCESS";
    // if (strstr(Const_response_data, SUCCESS) != NULL) {
    //   const char* ok = 
    //   Serial.println('isok');
    // }
    // else {
    //   Serial.println('"isnt ok"');
    // }
  } else {
    Serial.print("Error on HTTP request. Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}