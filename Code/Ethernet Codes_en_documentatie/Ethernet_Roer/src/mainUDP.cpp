#include <Arduino.h>
#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Servo.h>
#include "nmea.h"
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE1 };
IPAddress ip(192, 168, 100, 101);
IPAddress broadcast(192, 168, 100, 255);
unsigned int localPort = 8888;      // local port to listen on
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
// buffers for receiving and sending data
char packetBuffer[500];  // buffer to hold incoming packet,

// fields 
Servo myservo; // create servo object to control a servo
int potPin = A0; // analog pin used to connect the potentiometer
int servoPin = 9; // pwm pin used to connect the servo
int deg = 70;
nmea rudder;
bool recieved = false;
// degrees of the status sensor
int calDeg();

void setup() {
  Serial.begin(9600);
  Serial.print("starting");
  myservo.attach(servoPin); // attaches the servo on pin 9 to the servo object
  myservo.write(deg); // write angle to the servo
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  rudder.begin("RRAAA", 2);
  Serial.println("done");
}

void loop() {
  // run this code once a second
  static unsigned long timer = millis();
  static unsigned long timer1 = millis();
  if (timer + 100 < millis())  {
    timer = millis();
    // make the nmea message
    String fields[2] = {String(map(calDeg(), 0, 180, 0, 1000)), String(map(deg, 0, 180, 0, 1000))};
    // Serial.println(rudder.make(fields)); // show the nmea message on the serial monitor
    Udp.beginPacket(broadcast, 8888); // send the message as a broadcast on the network
    Udp.write(rudder.make(fields).c_str()); // load the nmea message
    if (!Udp.endPacket()) { // send the message
      Serial.println("Failed to send");
    }
  }
  if (timer1 + 5000 < millis())  {
    Udp.flush();
  }
  
  // try to load a message
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // read the packet into packetBufffer
    Udp.read(packetBuffer, constrain(packetSize, 1, UDP_TX_PACKET_MAX_SIZE));
    if (rudder.check(packetBuffer)) { // check if nmea is correctly recieved
      if (rudder.getHeader() == "BSRRA") { // filter on nmea header
        int field0 = rudder.getField(0).toInt(); // load the first field
        // Serial.println(field0);
        deg = map(field0, 0, 1000, 0, 180); // map value for the servo
        myservo.write(deg); // write angle to the servo
      }
    } else {
      // print the message
      Serial.println(packetBuffer);
    }
  } 
}

// calculate the degree of the sensor(potMeter)
int calDeg()  {
  int input = analogRead(potPin);
  // calculate degrees with 3 linaer maps
  if (input > 230)  {
    return map(input, 229, 907, 100, 5);
  } else if (input > 50) {
    return map(input, 50, 229, 144, 103);
  } else {
    return map(input, 10, 50, 175, 144);
  }
}
