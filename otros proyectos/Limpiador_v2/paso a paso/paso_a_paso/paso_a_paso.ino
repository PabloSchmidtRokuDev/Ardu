// Include the AccelStepper Library
#include <AccelStepper.h>

// Define pin connections
const int dirPin = 5;
const int stepPin = 2;

// Define motor interface type
#define motorInterfaceType 1

// Creates an instance
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  // set the maximum speed, acceleration factor,
  // initial speed and the target position
  myStepper.setMaxSpeed(3000);
  myStepper.setAcceleration(100);
  myStepper.setSpeed(300);
  myStepper.moveTo(400);
  Serial.println("ready");
  myStepper.run();
}

void loop() {
  // Change direction once the motor reaches target position
  
  if (myStepper.distanceToGo() == 0) {
    //myStepper.moveTo(-myStepper.currentPosition());
    Serial.print("go to ?? 1: ");
  }
  else
  {
    Serial.print("go to ?? 2: ");
    
    //myStepper.moveTo(200);
  
  }
  Serial.println(myStepper.currentPosition());
  // Move the motor one step
  //myStepper.run();
  
}
