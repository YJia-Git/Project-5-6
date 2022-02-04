
#include <Ethernet.h>
#include <EthernetUdp.h>
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

class nmeaMaker  {
  private:
    String header;
    int fields;

    //A function to calculate the checksum of a string
    String checkSumCalculator(String currentString) {
      int checkSum = 0;
      // loop trough the whole string
      for (int i = 0; i < currentString.length(); i++) {
        char x = currentString[i];
        byte y = x;
        // the XOR
        checkSum ^= y;
      }
      // return the checksum in string format
      String hex = String(checkSum, HEX);
      return hex;
    }
  public:
    nmeaMaker(String header, int numberOfFieleds)  {
      this->header = header;
      fields = numberOfFieleds;
    };
    // number is amount of fields, start counting from 1
    String make(String field[])  {
      // start making the nmea data
      String nmea = header; // devices code
      for (int i = 0; i < fields; i++)  {
        nmea += "," + String(field[i]);
      }
      // add checksum
      nmea = "!" + nmea + "*" + checkSumCalculator(nmea);
      return nmea;
    }
};

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 100, 112);
IPAddress broadcast(192, 168, 100, 255);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";        // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // start the Ethernet
  Ethernet.begin(mac, ip);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start UDP
  Udp.begin(localPort);
}

void loop() {
  // if there's data available, read a packet
  int analogValue = analogRead(A0);
  int graden = floatMap(analogValue, 0, 1023, 0, 180);
 // Serial.println(graden);
    // send a reply to the IP address and port that sent us the packet we received
    nmeaMaker rudder("BSRRA", 2);
   
    String fields[] = {String(map(graden, 0, 180, 0, 1000))};
//    Serial.println(rudder.make(fields));
    Udp.beginPacket(broadcast, 8888);
    Udp.write(rudder.make(fields).c_str());
    Udp.endPacket();


 
  int packetSize = Udp.parsePacket();
  if (packetSize) {
//    Serial.print("Received packet of size ");
//    Serial.println(packetSize);
//    Serial.print("From ");
//    IPAddress remote = Udp.remoteIP();
//    for (int i = 0; i < 4; i++) {
//      Serial.print(remote[i], DEC);
//      if (i < 3) {
//        Serial.print(".");
//      }
//    }
//    Serial.print(", port ");
//    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
//    Serial.println("Contents:");
    Serial.println(packetBuffer);

//    // send a reply to the IP address and port that sent us the packet we received
//    nmeaMaker rudder("BSRRA", 2);
//   
//    String fields[] = {String(map(graden, 0, 180, 0, 1000))};
//    Serial.println(rudder.make(fields));
//    Udp.beginPacket(broadcast, 8888);
//    Udp.write(rudder.make(fields).c_str());
//    Udp.endPacket();
    
  }
  delay(50);
}
