#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
String a = "";
int check_serial = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  mySerial.begin(9600);
}

void loop()
{
  check_serial = mySerial.available();
  if (check_serial) {
    char c = mySerial.read(); // Read a single character

    if (c != '\n' && c != '\r') { // Ignore newline and carriage return characters
      a += c;
    }

    if (check_serial == 1){
      Serial.println(a);
      a = "";
    }
  }
  delay(100);
}
