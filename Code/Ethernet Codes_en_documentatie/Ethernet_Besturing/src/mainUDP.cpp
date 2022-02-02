#include <Arduino.h>
#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "nmea.h"

// Enter a MAC address and IP address for your controller below.+
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE0 };
IPAddress ip(192, 168, 100, 100);
IPAddress broadcast(192, 168, 100, 255);
unsigned int localPort = 8888;      // local port to listen on
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,

int PWM_pin = 3;
nmea rudder;

void setup() {
  Serial.begin(9600);
  Serial.println("starting");
  pinMode(PWM_pin, INPUT);
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  delay(200); // wait so it can connect
  Udp.begin(localPort);  
  rudder.begin("BSRRA", 1);
  Serial.println("done");
}

void loop() {
  // run this code once a second
  static unsigned long timer = millis();
  if (timer + 20 < millis())  {
    timer = millis();
    double pwm_value = pulseIn(PWM_pin, HIGH); // read the pulse ont the pin
    double pwm_valueFloat = constrain(pwm_value, 900, 2000)/1000; // pwm in milliseconds
    // Serial.println(pwm_valueFloat);
    // make the nmea message
    String fields[1] = {String(map(pwm_value, 900, 2000, 0, 1000))};
    // Serial.println(rudder.make(fields)); // show the nmea message on the serial monitor
    delay(5);
    Udp.beginPacket(broadcast, 8888); // send the message as a broadcast on the network
    Udp.write(rudder.make(fields).c_str()); // load the nmea message
    if (!Udp.endPacket()) { // send the message
      Serial.println("Failed to send");
    }
  }
  delay(5);
  /*** this is put as command because in this case we don't need te listen to any messages ***/
  // // try to load a message
  // int packetSize = Udp.parsePacket();
  // if (packetSize) {
  //   // read the packet into packetBufffer
  //   Udp.read(packetBuffer, constrain(packetSize, 1, UDP_TX_PACKET_MAX_SIZE));
  //   Serial.println(packetBuffer); // show the recieved message on the serial monitor
  //   if (rudder.check(packetBuffer)) { // check if nmea is correctly recieved
  //     if (rudder.getHeader() == "BSRRA") { // filter on nmea header
  //       int field0 = rudder.getField(0).toInt(); // load the first field
  //       // Serial.println(field0);
  //     }
  //   } else {
  //     // print the message
  //     Serial.println(packetBuffer);
  //   }
  // } 
}


