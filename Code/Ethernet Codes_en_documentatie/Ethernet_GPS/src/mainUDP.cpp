#include <Arduino.h>
#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SoftwareSerial.h>

// class nmeaReader  {
//   private:
//     String message;
//         //A function to calculate the checksum of a string
//     String checkSumCalculator(String currentString) {
//       int checkSum = 0;
//       // loop trough the whole string
//       for (unsigned int i = 0U; i < currentString.length(); i++) {
//         char x = currentString[i];
//         byte y = x;
//         // the XOR
//         checkSum ^= y;
//       }
//       // return the checksum in string format
//       String hex = String(checkSum, HEX);
//       return hex;
//     }
//   public:
//     nmeaReader(String input): message(input) {};
//     void newMessage(String input)  {
//       message = input;
//     }

//     bool check()  {
//       String checksum = message.substring(message.indexOf("*") + 1, message.length());
//       String data = message.substring(1, message.indexOf("*"));
//       // check if message is recieved correctly
//       if (checksum.toInt() == checkSumCalculator(data).toInt()) {
//         return true;
//       } else {
//         //        // print both of the checksums
//         //        Serial.print(checksum);
//         //        Serial.print('\t');
//         //        Serial.print(checkSumCalculator(data));
//         return false;
//       }
//     }
//     String header() {
//       String data = message.substring(1, message.indexOf("*"));
//       return data.substring(0, 5);
//     }
//     String field(int index) {
//       String data = message.substring(6, message.indexOf("*"));
//       int i, j;
//       i = data.indexOf(",");
//       j = data.indexOf(",", i + 1);
//       if (index >= 0) {
//         for (int h = 0; h < index; h++)  {
//           i = j;
//           j = data.indexOf(",", i + 1);
//           if (j < 0) {
//             // set to end, because it is the last field
//             j = data.length();
//           }
//           // check if field is valid
//           if (i < 0 || i + 1 >= j)  {
//             return "index not found";
//           }
//         }
//         return data.substring(i + 1, j);
//       } else {
//         return "invalid index";
//       }
//     }
// };

// class nmeaMaker  {
//   private:
//     String header;
//     int fields;

//     //A function to calculate the checksum of a string
//     String checkSumCalculator(String currentString) {
//       int checkSum = 0;
//       // loop trough the whole string
//       for (unsigned int i = 0U; i < currentString.length(); i++) {
//         char x = currentString[i];
//         byte y = x;
//         // the XOR
//         checkSum ^= y;
//       }
//       // return the checksum in string format
//       String hex = String(checkSum, HEX);
//       return hex;
//     }
//   public:
//     nmeaMaker(String header, int numberOfFieleds)  {
//       this->header = header;
//       fields = numberOfFieleds;
//     };
//     // number is amount of fields, start counting from 1
//     String make(String field[])  {
//       // start making the nmea data
//       String nmea = header; // devices code
//       for (int i = 0; i < fields; i++)  {
//         nmea += "," + String(field[i]);
//       }
//       // add checksum
//       nmea = "!" + nmea + "*" + checkSumCalculator(nmea);
//       return nmea;
//     }
// };


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE2 };
IPAddress broadcast(192, 168, 100, 255);
// IPAddress ip(192, 168, 100, 178);

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
  Ethernet.begin(mac);
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
