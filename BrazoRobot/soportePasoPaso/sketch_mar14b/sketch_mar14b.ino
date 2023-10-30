/*Example sketch to control a stepper motor with A4988 stepper motor driver, AccelStepper library and Arduino: acceleration and deceleration. More info: https://www.makerguides.com */
// Include the AccelStepper library:
#include <AccelStepper.h>
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin1 5
#define stepPin1 2
#define motorInterfaceType 1

#define dirPin2 6
#define stepPin2 3
#define motorInterfaceType 1

#define dirPin3 7
#define stepPin3 4
#define motorInterfaceType 1

#define dirPin4 13
#define stepPin4 12
#define motorInterfaceType 1

// Create a new instance of the AccelStepper class:
AccelStepper stepper1 = AccelStepper(motorInterfaceType, stepPin1, dirPin1);
AccelStepper stepper2 = AccelStepper(motorInterfaceType, stepPin2, dirPin2);
AccelStepper stepper3 = AccelStepper(motorInterfaceType, stepPin3, dirPin3);
AccelStepper stepper4 = AccelStepper(motorInterfaceType, stepPin4, dirPin4);

void setup() {
  // Set the maximum speed and acceleration:
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(400);

  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(400);

  stepper3.setMaxSpeed(1000);
  stepper3.setAcceleration(400);

  stepper4.setMaxSpeed(1000);
  stepper4.setAcceleration(400);
}

void loop() {
  // Set the target position:
  stepper1.moveTo(500);
  // Run to target position with set speed and acceleration/deceleration:
  stepper1.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper1.moveTo(0);
  stepper1.runToPosition();
  delay(1000);


  stepper2.moveTo(500);
  // Run to target position with set speed and acceleration/deceleration:
  stepper2.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper2.moveTo(0);
  stepper2.runToPosition();
  delay(1000);

stepper3.moveTo(500);
  // Run to target position with set speed and acceleration/deceleration:
  stepper3.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper3.moveTo(0);
  stepper3.runToPosition();
  delay(1000);






    stepper1.moveTo(500);
  // Run to target position with set speed and acceleration/deceleration:
  stepper1.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper1.moveTo(0);
  stepper1.runToPosition();
  delay(1000);


    stepper4.moveTo(500);
  // Run to target position with set speed and acceleration/deceleration:
  stepper4.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper4.moveTo(0);
  stepper4.runToPosition();
  delay(1000);


    stepper3.moveTo(300);
  // Run to target position with set speed and acceleration/deceleration:
  stepper3.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper3.moveTo(0);
  stepper3.runToPosition();
  delay(1000);



   stepper2.moveTo(200);
  // Run to target position with set speed and acceleration/deceleration:
  stepper2.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper2.moveTo(0);
  stepper2.runToPosition();
  delay(1000);


    stepper1.moveTo(600);
  // Run to target position with set speed and acceleration/deceleration:
  stepper1.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper1.moveTo(0);
  stepper1.runToPosition();
  delay(1000);


    stepper4.moveTo(400);
  // Run to target position with set speed and acceleration/deceleration:
  stepper4.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper4.moveTo(0);
  stepper4.runToPosition();
  delay(1000);

    // Set the target position:
  stepper1.moveTo(-100);
  // Run to target position with set speed and acceleration/deceleration:
  stepper1.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper1.moveTo(500);
  stepper1.runToPosition();
  delay(1000);




    // Set the target position:
  stepper1.moveTo(400);
  // Run to target position with set speed and acceleration/deceleration:
  stepper1.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper1.moveTo(0);
  stepper1.runToPosition();
  delay(1000);





    // Set the target position:
  stepper1.moveTo(600);
  // Run to target position with set speed and acceleration/deceleration:
  stepper1.runToPosition();
  delay(1000);
  // Move back to zero:
  stepper1.moveTo(0);
  stepper1.runToPosition();
  delay(1000);
}
