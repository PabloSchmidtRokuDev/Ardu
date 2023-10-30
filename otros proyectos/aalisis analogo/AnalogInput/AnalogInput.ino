/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13 through 220 ohm resistor
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInput
*/

int sensorPin1 = A1;    // select the input pin for the potentiometer
int sensorPin2 = A2;
int sensorPin3 = A3;
int sensorPin4 = A4;
int sensorPin5 = A5;


int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;
int sensorValue4 = 0;
int sensorValue5 = 0;

int sensorValueCont = 0; 
int sensorMin = 1023;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value

void setup() {
 Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  sensorValue3 = analogRead(sensorPin3);
  sensorValue4 = analogRead(sensorPin4);
  sensorValue5 = analogRead(sensorPin5);

  //sensorValueCont = constrain(sensorValue1, sensorMin, sensorMax);
  //sensorValue = map(sensorValueCont, sensorMin, sensorMax, 0, 255);
  //Serial.println("a1: " + String(sensorValue1) + " - sensorValueCont: " + String(sensorValueCont)+ " - sensorValue: " + String(sensorValue));
Serial.print(String(sensorValue1));


  // sensorValueCont = constrain(sensorValue2, sensorMin, sensorMax);
 //  sensorValue = map(sensorValueCont, sensorMin, sensorMax, 0, 255);
  //Serial.println("a2: " + String(sensorValue2) + " - sensorValueCont: " + String(sensorValueCont)+ " - sensorValue: " + String(sensorValue));
Serial.print( " --  " + String(sensorValue2));

 //  sensorValueCont = constrain(sensorValue3, sensorMin, sensorMax);
 //  sensorValue = map(sensorValueCont, sensorMin, sensorMax, 0, 255);
  //Serial.println("a3: " + String(sensorValue3) + " - sensorValueCont: " + String(sensorValueCont)+ " - sensorValue: " + String(sensorValue));
Serial.print( " --  " + String(sensorValue3));


 //  sensorValueCont = constrain(sensorValue4, sensorMin, sensorMax);
 //  sensorValue = map(sensorValueCont, sensorMin, sensorMax, 0, 255);
  //Serial.println("a4: " + String(sensorValue4) + " - sensorValueCont: " + String(sensorValueCont)+ " - sensorValue: " + String(sensorValue));
Serial.print( " --  " + String(sensorValue4));


  // sensorValueCont = constrain(sensorValue5, sensorMin, sensorMax);
  // sensorValue = map(sensorValueCont, sensorMin, sensorMax, 0, 255);
  //Serial.println("a5: " + String(sensorValue5) + " - sensorValueCont: " + String(sensorValueCont)+ " - sensorValue: " + String(sensorValue));
  Serial.println( " --  " + String(sensorValue5));

delay(500);
}
