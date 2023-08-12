#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

const char* serverUrl = "http://127.0.0.1/api/auth/esp/auth_receiver";
const char* auth_token = "your_auth_token";
const char* type = "your_type";
const char* for_ = "your_for";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client; // Use WiFiClient for HTTP
    HTTPClient http;

    // Construct the JSON payload
    DynamicJsonDocument jsonDoc(256);
    jsonDoc["location_auth_id"] = auth_token;
    jsonDoc["type"] = type;
    jsonDoc["for_"] = for_;
    
    String payload;
    serializeJson(jsonDoc, payload);

    http.begin(client, serverUrl); // Use the client with the URL
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response Code: " + String(httpResponseCode));
      Serial.println("HTTP Response: " + response);
      
      // Parse and handle the response JSON here if needed
    } else {
      Serial.print("Error on HTTP request. Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(5000); // Send the request every 5 seconds
}
