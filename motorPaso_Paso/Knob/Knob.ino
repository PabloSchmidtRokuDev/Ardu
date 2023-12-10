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
int val = 0;      // variable to read the value from the analog pin
bool stado = false;


int currentPos = 0;

int setSpeed(int from, int to) {

  if (from < to) {
    for (int i = from; i < to; i++) {
      myservo.write(i);
      delay(150);
    }
  } else {
    for (int i = from; i < to; i--) {
      myservo.write(i);
      delay(150);
    }
  }

  return to;
}

void setup() {
   Serial.begin(115200);
  //pinMode(0, INPUT);
  pinMode(4, INPUT);

  myservo.attach(2, 1000, 2000);  // attaches the servo on pin 9 to the servo object

  delay(5000);
  setSpeed(currentPos, currentPos + 30);
delay(2000);
  myservo.write(0);
  delay(5000);
   Serial.println("start..");
   currentPos = setSpeed(currentPos, currentPos + 100);
}


int SpeedSelect = 0;
void loop() {

//  Serial.println("boton estado.."+ String(digitalRead(4)));

//   if (digitalRead(4) == HIGH) {

//     if (SpeedSelect >= 0 && SpeedSelect <= 6) {
//       currentPos = setSpeed(currentPos, currentPos + 30);
//       SpeedSelect++;
//     } else {
//       currentPos = setSpeed(currentPos, 0);
//       SpeedSelect = 0;
      
// delay(5000);
//     }
//   }

  // if (digitalRead(4) == HIGH && currentPos > 0){
  // currentPos = setSpeed(currentPos, currentPos - 10);
  // }
  //   for(int i=30; i<180;i++){
  //   myservo.write(i);
  //   delay(150);
  // }
  // delay(5000);

  // for(int i=180; i>0;i--){
  //   myservo.write(i);
  //   delay(150);
  // }
  // stado = !stado;

  // digitalWrite(0, stado);

  // delay(1000);
  // waits for the servo to get there
}
