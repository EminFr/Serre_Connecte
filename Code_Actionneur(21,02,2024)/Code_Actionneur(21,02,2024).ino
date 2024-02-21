/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo myservo_droit;  // create servo object to control a servo
Servo myservo_gauche;  // create servo object to control a servo
#define Ventilator 2
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo_droit.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo_droit.write(0);
  myservo_gauche.attach(10);  // attaches the servo on pin 9 to the servo object
  myservo_gauche.write(90);
  pinMode(Ventilator, OUTPUT);

}

void loop() {

  activate_ventilation();
  delay(11000);

  /*
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo_droit.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo_droit.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }*/
  

}

void activate_ventilation()
{
    digitalWrite(Ventilator, HIGH);
  for (pos = 60; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo_droit.write(pos);              // tell servo to go to position in variable 'pos'
    myservo_gauche.write(90-pos);
    delay(30);                       // waits 15 ms for the servo to reach the position
  }
  delay(15000);
  digitalWrite(Ventilator, LOW);
  for (pos = 0; pos <= 60; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo_droit.write(pos);              // tell servo to go to position in variable 'pos'
    myservo_gauche.write(90-pos);
    delay(30);                       // waits 15 ms for the servo to reach the position
  }
  delay(4000);
}