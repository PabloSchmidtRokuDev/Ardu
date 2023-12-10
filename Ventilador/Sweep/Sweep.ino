/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(115200);
  myservo.attach(2, 1000, 2000);  // attaches the servo on pin 9 to the servo object
  myservo.write(10);  
  delay(5000);
    for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
    Serial.print(pos);
  Serial.println("+ ");
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(150);                       // waits 15 ms for the servo to reach the position
  }
}

void loop() {
  for (pos = 100; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    Serial.print(pos);
  Serial.println("+ ");
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(150);                       // waits 15 ms for the servo to reach the position
  }
delay(5000);
  for (pos = 180; pos >= 100; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
        Serial.print(pos);
  Serial.println("- ");
    delay(150);                       // waits 15 ms for the servo to reach the position
  }
}
