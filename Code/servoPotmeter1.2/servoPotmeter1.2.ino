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

// fields 
Servo myservo; // create servo object to control a servo

int potPin = A0; // analog pin used to connect the potentiometer
int servoPin = 9; // pwm pin used to connect the servo

int deg = 70;


void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin); // attaches the servo on pin 9 to the servo object
  myservo.write(deg); // write angle to the servo
//  nmeaReader test("$GPRRS,45678,HGF,65432,fgh,7654dfg*23");
//  if (test.check()) {
//    Serial.println(test.header());
//    Serial.println(test.field(0));
//    Serial.println(test.field(1));
//    Serial.println(test.field(2));
//    Serial.println(test.field(3));
//    Serial.println(test.field(4));
//  }
}

void loop() {
  delay(100); // just to keep it calm at the serial monitor
  // NMEA maker
  nmeaMaker rudder("RRAAA", 2);
  String fields[2] = {String(map(calDeg(), 0, 180, 0, 1000)), String(map(deg, 0, 180, 0, 1000))};
  Serial.println(rudder.make(fields));
  if (Serial.available())  {
    int i = Serial.parseInt(); // recieve new angle
    if (i > 0)  {
      deg = i;
    }
    myservo.write(deg); // write angle to the servo
  }
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
