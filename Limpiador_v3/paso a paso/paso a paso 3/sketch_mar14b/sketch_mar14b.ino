/*Example sketch to control a stepper motor with A4988 stepper motor driver, AccelStepper library and Arduino: acceleration and deceleration. More info: https://www.makerguides.com */
// Include the AccelStepper library:
#include <AccelStepper.h>
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 5
#define stepPin 2
#define motorInterfaceType 1
// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  // Set the maximum speed and acceleration:
  stepper.setMaxSpeed(3000);
  stepper.setAcceleration(2000);
}
void loop() {
  // Set the target position:
  stepper.moveTo(1200);
  // Run to target position with set speed and acceleration/deceleration:
  stepper.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper.moveTo(0);
  stepper.runToPosition();
  delay(1000);



    // Set the target position:
  stepper.moveTo(-6000);
  // Run to target position with set speed and acceleration/deceleration:
  stepper.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper.moveTo(1500);
  stepper.runToPosition();
  delay(1000);




    // Set the target position:
  stepper.moveTo(4000);
  // Run to target position with set speed and acceleration/deceleration:
  stepper.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper.moveTo(0);
  stepper.runToPosition();
  delay(1000);





    // Set the target position:
  stepper.moveTo(6000);
  // Run to target position with set speed and acceleration/deceleration:
  stepper.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper.moveTo(0);
  stepper.runToPosition();
  delay(1000);
}
