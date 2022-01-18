
#include <SPI.h>
#include <Ethernet.h>

const int serverPort = 4080;

byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x03};
IPAddress serverAddress(192, 168, 100, 51);
EthernetClient TCPclient;

void setup() {
  Serial.begin(9600);
  Serial.println("ARDUINO #1: TCP CLIENT + A BUTTON/SWITCH");
  // Initialize Ethernet Shield:
  if (Ethernet.begin(mac) == 0)
    Serial.println("Failed to configure Ethernet using DHCP");
  // connect to TCP server (Arduino #2)
  if (TCPclient.connect(serverAddress, serverPort))
    Serial.println("Connected to TCP server");
  else
    Serial.println("Failed to connect to TCP server");
}

void loop() {

  if (!TCPclient.connected()) {
    Serial.println("Connection is disconnected");
    TCPclient.stop();
    // reconnect to TCP server (Arduino #2)
    if (TCPclient.connect(serverAddress, serverPort))
      Serial.println("Reconnected to TCP server");
    else
      Serial.println("Failed to reconnect to TCP server");
  }
    char ping[10] = "hello";
    TCPclient.write(ping);
    //TCPclient.flush();
    Serial.println("send");
    String response = TCPclient.readString();
    Serial.println(response);

    delay(100);
}
