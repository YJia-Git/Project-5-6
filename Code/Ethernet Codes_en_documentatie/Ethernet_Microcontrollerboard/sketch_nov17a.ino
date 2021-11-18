/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/communication-between-two-arduino
 */

// ARDUINO #1: TCP CLIENT + A BUTTON/SWITCH

#include <SPI.h>
#include <Ethernet.h>
#define ir 2




const int BUTTON_PIN = 7;
const int serverPort = 4080;
//ezButton button(BUTTON_PIN);  // create ezButton that attach to pin 7;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress serverAddress(192, 168, 137, 241); 
EthernetClient TCPclient;

void setup() {
  Serial.begin(9600);
//  button.setDebounceTime(50); // set debounce time to 50 milliseconds

  Serial.println("ARDUINO #1: TCP CLIENT + A BUTTON/SWITCH");

  // Initialize Ethernet Shield:
  if (Ethernet.begin(mac) == 0)
    Serial.println("Failed to configure Ethernet using DHCP");

  // connect to TCP server (Arduino #2)
  if (TCPclient.connect(serverAddress, serverPort))
    Serial.println("Connected to TCP server");
  else
    Serial.println("Failed to connect to TCP server");
  
  
  pinMode (ir, INPUT); // sensor pin INPUT

}

void loop() {
//  button.loop(); // MUST call the loop() function first

  if (!TCPclient.connected()) {
    Serial.println("Connection is disconnected");
    TCPclient.stop();
    // reconnect to TCP server (Arduino #2)
    if (TCPclient.connect(serverAddress, serverPort))
      Serial.println("Reconnected to TCP server");
    else
      Serial.println("Failed to reconnect to TCP server");
  }


  int statusSensor = digitalRead (ir);
//  
  if (statusSensor == 0){
    TCPclient.write('1'); // LED LOW
    delay(100);
  }else
  {
    TCPclient.write('0'); // LED High
     delay(100);
  }

  Serial.println(statusSensor);


// TCPclient.write('1'); // LED LOW
// delay(1000);
//  TCPclient.write('0'); // LED LOW
//   delay(1000);
//  


}
