#include <SoftwareSerial.h>
static const int RXPin = 4, TXPin = 3;  // pins for the GPS-module
static const uint32_t GPSBaud = 9600;   // serial baundrate of the module
SoftwareSerial ss(RXPin, TXPin);        // setup serial connection

void setup()  {
  Serial.begin(9600);   // start connection with pc
  ss.begin(GPSBaud);    // start connection with module
}

void loop() {
  // when data available, read it and send directly to the pc as char's
  while(ss.available() > 0) {     
    Serial.print((char)ss.read());
  }
}
