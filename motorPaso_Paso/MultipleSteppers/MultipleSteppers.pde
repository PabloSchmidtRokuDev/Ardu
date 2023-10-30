// MultiStepper.pde
// -*- mode: C++ -*-
//
// Shows how to multiple simultaneous steppers
// Runs one stepper forwards and backwards, accelerating and decelerating
// at the limits. Runs other steppers at the same time
//
// Copyright (C) 2009 Mike McCauley
// $Id: MultiStepper.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Define some steppers and the pins the will use
AccelStepper stepper1(AccelStepper::FULL2WIRE, 2,5);
AccelStepper stepper2(AccelStepper::FULL2WIRE, 3,6);

void setup()
{  
    stepper1.setMaxSpeed(25000);
    stepper1.setAcceleration(1000.0);
    stepper1.moveTo(1000);
    
    stepper2.setMaxSpeed(25000);
    stepper2.setAcceleration(1000.0);
    stepper2.moveTo(10000);
    
}

void loop()
{
      stepper1.run();
    stepper2.run();
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0) stepper1.moveTo(stepper1.currentPosition()-1);
    if (stepper2.distanceToGo() == 0) stepper2.moveTo(stepper2.currentPosition()-1);

}
