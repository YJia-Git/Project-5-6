#include <Arduino.h>
#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Servo.h>

class nmeaReader  {
  private:
    String message;
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
    nmeaReader(String input): message(input) {};
    void newMessage(String input)  {
      message = input;
    }

    bool check()  {
      String checksum = message.substring(message.indexOf("*") + 1, message.length());
      String data = message.substring(1, message.indexOf("*"));
      // check if message is recieved correctly
      if (checksum.toInt() == checkSumCalculator(data).toInt()) {
        return true;
      } else {
        //        // print both of the checksums
        //        Serial.print(checksum);
        //        Serial.print('\t');
        //        Serial.print(checkSumCalculator(data));
        return false;
      }
    }
    String header() {
      String data = message.substring(1, message.indexOf("*"));
      return data.substring(0, 5);
    }
    String field(int index) {
      String data = message.substring(6, message.indexOf("*"));
      int i, j;
      i = data.indexOf(",");
      j = data.indexOf(",", i + 1);
      if (index >= 0) {
        for (int h = 0; h < index; h++)  {
          i = j;
          j = data.indexOf(",", i + 1);
          if (j < 0) {
            // set to end, because it is the last field
            j = data.length();
          }
          // check if field is valid
          if (i < 0 || i + 1 >= j)  {
            return "index not found";
          }
        }
        return data.substring(i + 1, j);
      } else {
        return "invalid index";
      }
    }
};

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
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };
IPAddress broadcast(192, 168, 100, 255);
// IPAddress ip(192, 168, 100, 178);

unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,


// fields 
Servo myservo; // create servo object to control a servo
int potPin = A0; // analog pin used to connect the potentiometer
int servoPin = 9; // pwm pin used to connect the servo
int deg = 70;

// degrees of the status sensor
int calDeg();

void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin); // attaches the servo on pin 9 to the servo object
  myservo.write(deg); // write angle to the servo
  // start the Ethernet and UDP:
  Ethernet.begin(mac);
  Udp.begin(localPort);
  Serial.println(Ethernet.localIP());
}

void loop() {
  // send the nmea message over UDP
  static unsigned long timer = millis();
  if (timer + 1000 < millis())  {
    timer = millis();
    nmeaMaker rudder("RRAAA", 2);
    String fields[2] = {String(map(calDeg(), 0, 180, 0, 1000)), String(map(deg, 0, 180, 0, 1000))};
    // Serial.println(rudder.make(fields));
    Udp.beginPacket(broadcast, 8888);
    Udp.write(rudder.make(fields).c_str());
    if (!Udp.endPacket()) {
      Serial.println("Failed to send");
    }
  }
  
  // try to read the message
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println(packetBuffer);
    nmeaReader test(packetBuffer);
    if (test.check()) {
      // Serial.println(test.header());
      if (test.header() == "BSRRA") {
        int field0 = test.field(0).toInt();
        // Serial.println(field0);
        deg = map(field0, 0, 1000, 0, 180);
        myservo.write(deg); // write angle to the servo
      }
      //  Serial.println(test.field(0));
      //  Serial.println(test.field(1));
      //  Serial.println(test.field(2));
      //  Serial.println(test.field(3));
      //  Serial.println(test.field(4));
    } else {
      Serial.println(packetBuffer);
    }
  }
  // !BSRRA,500*49
}


// calculate the degree of the sensor(potMeter)
int calDeg()  {
  //return map(input, 10, 907, 175, 5); // linear 1
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
