#include <Arduino.h>
#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "nmea.h"
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE5 };
IPAddress ip(192, 168, 100, 105);
IPAddress broadcast(192, 168, 100, 255);
unsigned int localPort = 8888;      // local port to listen on
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
// buffers for receiving and sending data
char packetBuffer[500];  // buffer to hold incoming packet,

nmea componentName;
bool recieved = false;
// degrees of the status sensor
int calDeg();

void setup() {
  Serial.begin(9600);
  Serial.print("starting");
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  // initialize NMEA
  componentName.begin("COMPO", 2);
  Serial.println("done");
}

void loop() {
  // run this every 100 miliseconds
  static unsigned long timer = millis();
  if (timer + 100 < millis())  {
    timer = millis();
    // make the nmea message
    String fields[2] = {"field0", "field1"};
    // send the message as a broadcast on the network
    Udp.beginPacket(broadcast, 8888); 
    Udp.write(componentName.make(fields).c_str()); 
    // send the message and show when failed
    if (!Udp.endPacket()) { 
      Serial.println("Failed to send");
    }
  }
  
  // try to load a message
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // read the packet into packetBufffer
    Udp.read(packetBuffer, constrain(packetSize, 1, UDP_TX_PACKET_MAX_SIZE));
    if (componentName.check(packetBuffer)) { // check if nmea is correctly recieved
      if (componentName.getHeader() == "GPSAA") { // check if message is for me
        int field0 = componentName.getField(0).toInt(); // load the first field
        Serial.println(field0);
      }
    } else {
      // print the message when not NMEA protocol
      Serial.println(packetBuffer);
    }
  } 
}
