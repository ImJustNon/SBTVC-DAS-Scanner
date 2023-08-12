#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* serverUrl = "API_ENDPOINT";


WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");


}

void sendPostRequest() {
  HTTPClient http;
  
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/json");

  // Create a JSON object to send in the request
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["key1"] = "value1";
  jsonDoc["key2"] = "value2";

  String requestBody;
  serializeJson(jsonDoc, requestBody);

  int httpResponseCode = http.POST(requestBody);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    String response = http.getString();
    Serial.println("Response:");
    Serial.println(response);
  } else {
    Serial.print("Error sending POST request. Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void loop() {
  
  sendPostRequest();

  delay(5000);
}
