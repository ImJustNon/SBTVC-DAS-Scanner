#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Nonlnwza_2.4GHz";
const char* password = "gulnwzamakmak";

// API endpoint and JSON data
const char* apiEndpoint = "sbtvc-das-api.nonlnwza.xyz";
const char* jsonData = "{\"text\": \"lnwza\"}";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  // Send POST request
  sendPostRequest();
}

void loop() {
  // Nothing to do here
}

void sendPostRequest() {
  // Create a WiFiClient object
  WiFiClient client;

  // Connect to the server
  if (!client.connect(apiEndpoint, 80)) {
    Serial.println("Connection failed");
    return;
  }

  // Prepare the HTTP POST request
  String url = "/api/test/chi_test_post";  // Replace with the actual API endpoint
  String contentType = "application/json";

  // Create the JSON document
  DynamicJsonDocument jsonDocument(256);
  deserializeJson(jsonDocument, jsonData);

  // Serialize the JSON document to a string
  String jsonPayload;
  serializeJson(jsonDocument, jsonPayload);

  // Send the POST request
  client.print("POST ");
  client.print(url);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(apiEndpoint);
  client.println("Connection: close");
  client.print("Content-Type: ");
  client.println(contentType);
  client.print("Content-Length: ");
  client.println(jsonPayload.length());
  client.println();
  client.println(jsonPayload);

  Serial.println("POST request sent");

  // Read and display the server response
  String response = "";
  while (client.connected() || client.available()) {
    if (client.available()) {
      char c = client.read();
      response += c;
    }
  }

  Serial.println("Server response received");

  // Parse the JSON response
  DynamicJsonDocument jsonResponse(256);
  deserializeJson(jsonResponse, response);

  // Access the data in the JSON response
  // Serial.println(jsonResponse);
  // Serial.println(response);
  const char* responseText = jsonResponse[""];

  Serial.print("Response text: ");
  Serial.println(responseText);

  // Disconnect from the server
  client.stop();
}
