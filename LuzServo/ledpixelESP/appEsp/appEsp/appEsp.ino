#include <SoftwareSerial.h>

const int pinESPrx = 12;
const int pinESPtx = 13;

/* Create object named SIM900 of the class SoftwareSerial */
SoftwareSerial serialArdu(pinESPrx, pinESPtx);

void setup() {
  // put your setup code here, to run once:
  serialArdu.begin(115200);  /* Define baud rate for software serial communication */
  Serial.begin(115200);

  serialArdu.println("Esp started."); 
  Serial.println("Esp started."); 

}

String myString;

void loop() {
  
  while (Serial.available() > 0)
  {
    myString = Serial.readString();

    serialArdu.println("leido del serial From esp:  " + myString);
    
  }

    while (serialArdu.available() > 0)
  {
    myString = serialArdu.readString();

    Serial.println(" - leido de arduino From esp:  " + myString);
    
  }

}
