/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = A0;  // analog pin used to connect the potentiometer
int val=0;    // variable to read the value from the analog pin

void setup() {
  myservo.attach(2,500,2400);  // attaches the servo on pin 9 to the servo object
}

void loop() {

    for(int i=0; i<180;i++){
    myservo.write(i);  
    delay(100);
  }
  delay(5000);

  for(int i=180; i>0;i--){
    myservo.write(i);  
    delay(100);
  }
                  // sets the servo position according to the scaled value
  delay(1000);  
                     // waits for the servo to get there
}
