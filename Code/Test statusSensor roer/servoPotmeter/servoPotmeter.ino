#include <Servo.h>

Servo myservo; // create servo object to control a servo

int potPin = A0; // analog pin used to connect the potentiometer
int servoPin = 9; // pwm pin used to connect the servo

int speed = 100; // time between new degrees of the servo
int deg = 70;

void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin); // attaches the servo on pin 9 to the servo object
  myservo.write(deg); // write angle to the servo
}

void loop() {
  delay(100); // just to keep it calm at the serial monitor
  Serial.println(NMEA());
  myservo.write(deg); // add function that will change the deg variable

  // set degrees via the serial monitor
  if (Serial.available())  {
    int i = Serial.parseInt(); // recieve new angle
    if (i > 0)  {
      deg = i;
    }
    myservo.write(deg); // write angle to the servo
  }
  
//   move all directions
//    for (int i = 10; i <= 175; i++) {
//      myservo.write(i); // sets the servo position according to the scaled value
//      delay(speed);
//      Serial.print(calDeg());
//      Serial.print("\t");
//      Serial.println(i);
//    }
//    for (int i = 175; i >= 10; i--) {
//      myservo.write(i); // sets the servo position according to the scaled value
//      delay(speed);
//      Serial.print(calDeg());
//      Serial.print("\t");
//      Serial.println(i);
//    }
}

// return NMEA string of the rudder
String NMEA() {
  // start making the nmea data
  String nmea = "RRAAA"; // devices code
  // add calculated degrees form [0 - 1000]
  int data = map(calDeg(), 0, 180, 0, 1000);
  nmea += "," + String(data); 
  // add set degrees form [0 - 1000]
  data = map(deg, 0, 180, 0, 1000);
  nmea += "," + String(data); 
  // add checksum
  nmea = "!" + nmea + "*" + checkSumCalculator(nmea);
  return nmea;
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

//  // show data for the Serial.plotter
//  Serial.print(calDeg());
//  Serial.print("\t");
//  Serial.println(deg);
//  delay(5);
