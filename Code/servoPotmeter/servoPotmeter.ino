#include <Servo.h>

Servo myservo; // create servo object to control a servo

int potPin = A0; // analog pin used to connect the potentiometer
int servoPin = 9; // pwm pin used to connect the servo

//int speed = 100; // time between new degrees of the servo
int deg = 70;

void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin); // attaches the servo on pin 9 to the servo object
  myservo.write(deg); // write angle to the servo
}

void loop() {
  if (Serial.available())  {
    deg = Serial.parseInt(); // recieve new angle
    myservo.write(deg); // write angle to the servo
  }
  // show data
  Serial.print(calDeg());
  Serial.print("\t");
  Serial.println(deg);
  delay(5);
  
  // move all directions
  //  for (int i = 10; i <= 175; i++) {
  //    myservo.write(i); // sets the servo position according to the scaled value
  //    delay(speed);
  //    Serial.print(calDeg());
  //    Serial.print("\t");
  //    Serial.println(i);
  //  }
  //  for (int i = 175; i >= 10; i--) {
  //    myservo.write(i); // sets the servo position according to the scaled value
  //    delay(speed);
  //    Serial.print(calDeg());
  //    Serial.print("\t");
  //    Serial.println(i);
  //  }
}

// calculate the degree of the potMeter
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
