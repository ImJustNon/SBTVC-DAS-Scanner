#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
char a[128]; // Character array to store incoming data
int dataIndex = 0; // Index to track the position in the array

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  mySerial.begin(9600);
}

void loop()
{
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
        } else {
          // Access JSON fields and do something with them
          const char* type = doc["type"];
          const char* for_ = doc["for_"];
          const char* auth_token = doc["auth_token"];
          unsigned long create_on = doc["create_on"];
          
          // Print parsed values
          Serial.print("Type: ");
          Serial.println(type);
          Serial.print("For: ");
          Serial.println(for_);
          Serial.print("Auth Token: ");
          Serial.println(auth_token);
          Serial.print("Create On: ");
          Serial.println(create_on);
        }
        
        dataIndex = 0; // Reset the index for the next data
      }
    } else {
      if (dataIndex < sizeof(a) - 1) {
        a[dataIndex++] = c; // Store the character in the array
      }
    }
  }
}
