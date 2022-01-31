#include <Arduino.h>
#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SoftwareSerial.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE2 };
IPAddress broadcast(192, 168, 100, 255);
IPAddress ip(192, 168, 100, 102);

unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,

// fields 
static String SerialBuffer = "";
static const int RXPin = 4, TXPin = 3;  // pins for the GPS-module
static const uint32_t GPSBaud = 9600;   // serial baundrate of the module
SoftwareSerial ss(RXPin, TXPin);        // setup serial connection

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);    // start connection with GPSmodule
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  Serial.println(Ethernet.localIP());
}

void loop() {
  // read the GPS module and save data in buffer
  while(ss.available() > 0) {
    SerialBuffer += (char)ss.read();
  } 
  // run this code once a second
  static unsigned long timer = millis();
  if (timer + 20 < millis())  {
    timer = millis();
    // if buffer not emty read message from buffer and send it via udp
    if(SerialBuffer.length() > 0) {
      String message = SerialBuffer.substring(0, SerialBuffer.indexOf("\n"));
      SerialBuffer = SerialBuffer.substring(SerialBuffer.indexOf("\n")+1, SerialBuffer.length());
      Serial.println(message);
      Udp.beginPacket(broadcast, 8888); // send the message as a broadcast on the network
      Udp.write(message.c_str()); // load the nmea message
      if (!Udp.endPacket()) { // send the message
        Serial.println("Failed to send");
      }
    }
  }
  // // try to load a message
  // int packetSize = Udp.parsePacket();
  // if (packetSize) {
  //   // read the packet into packetBufffer
  //   Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
  //   Serial.println(packetBuffer); // show the recieved message on the serial monitor
  //   nmeaReader test(packetBuffer);
  //   if (test.check()) { // check if nmea is correctly recieved
  //     Serial.println("message got lost");
  //   } else {
  //     // print the message
  //     Serial.println(packetBuffer);
  //   }
  // } 
}
