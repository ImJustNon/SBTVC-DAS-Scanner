#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// Wifi setup
const char* ssid = "Galaxy A24";
const char* password = "disconnected";

// api setup
const char* serverUrl = "http://45.141.27.54:1756/api/bridge/sbtvc-das/api/auth/esp/auth_reseiver";
const char* key = "nonlnwza";

// Scanner setup
const int scannerRxPin = D6;
const int scannerTxPin = D5;

// SofwareSerial setup
SoftwareSerial mySerial(scannerTxPin, scannerRxPin); // TX , RX
char a[128];
int dataIndex = 0;

// led status setup
const int errorLedPin = D2;
const int warningLedPin = D3;
const int successLedPin = D4;
const int readyLedPin = D1;

void setup() {
  // board serial setup
  Serial.begin(9600);

  // led pinmode setup
  pinMode(errorLedPin, OUTPUT);
  pinMode(warningLedPin, OUTPUT);
  pinMode(successLedPin, OUTPUT);
  pinMode(readyLedPin, OUTPUT);

  //wifi setup 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  // serial setup
  while (!Serial){
    delay(1000);
    Serial.println("Connecting to Serial...");
  };
  mySerial.begin(9600);

  // print status
  Serial.println("WiFi Connected ");
  Serial.println("Serial Connected ");
  
  // set default led status 
  errorLed(0);
  warningLed(0);
  successLed(0);
  // turn ready led on
  readyLed(1);
}

void loop() {
  // wifi connected check
  if (WiFi.status() == WL_CONNECTED) {
    
    // loop when have serial come
    while (mySerial.available()) {
      char c = mySerial.read();

      if (c == '\n' || c == '\r') { // Ignore newline and carriage return characters
        if (dataIndex > 0) {
          a[dataIndex] = '\0'; // Null-terminate the character array
          
          // Parse the received JSON data
          StaticJsonDocument<256> doc;
          DeserializationError error = deserializeJson(doc, a);
          
          if (error) {
            Serial.print("Error parsing JSON: ");
            Serial.println(error.c_str());
            errorLed(1);
          } else {
            // Access JSON fields and do something with them
            const char* type = doc["type"];
            const char* for_ = doc["for_"];
            const char* auth_token = doc["auth_token"];
            unsigned long create_on = doc["create_on"];
            
            // Print parsed values
            // Serial.print("Type: ");
            // Serial.println(type);
            // Serial.print("For: ");
            // Serial.println(for_);
            // Serial.print("Auth Token: ");
            // Serial.println(auth_token);
            // Serial.print("Create On: ");
            // Serial.println(create_on);
            warningLed(1);
            HttpRequest(auth_token, type, for_);
          }
            delay(1000);
            resetAllStatusLed();
          dataIndex = 0; // Reset the index for the next data
        }
      } else {
        if (dataIndex < sizeof(a) - 1) {
          a[dataIndex++] = c; // Store the character in the array
        }
      }

    }
  }
}




void HttpRequest(String auth_token, String type, String for_){
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client; // Use WiFiClientSecure for HTTPS
    HTTPClient http;

    http.setTimeout(30000); // 10 seconds
    
    // Construct the JSON payload
    DynamicJsonDocument jsonDoc(256);
    jsonDoc["secret_key"] = String(key);
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
      Serial.println("HTTP Response: " + response);
      successLed(1);
    } else {
      Serial.print("Error on HTTP request. Error code: ");
      Serial.println(httpResponseCode);
      errorLed(1);
    }

    http.end();
  }
}

void errorLed(int status){
  digitalWrite(errorLedPin, status);
}
void warningLed(int status){
  digitalWrite(warningLedPin, status);
}
void successLed(int status){
  digitalWrite(successLedPin, status);
}
void readyLed(int status){
  digitalWrite(readyLedPin, status);
}
void resetAllStatusLed(){
  errorLed(0);
  warningLed(0);
  successLed(0);
}