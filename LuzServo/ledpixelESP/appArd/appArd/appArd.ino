#include <SoftwareSerial.h>


const int pinESPrx = 2;
const int pinESPtx = 3;

/* Create object named SIM900 of the class SoftwareSerial */
SoftwareSerial serialEsp(pinESPrx, pinESPtx);

void setup() {
  // put your setup code here, to run once:
  serialEsp.begin(115200);  /* Define baud rate for software serial communication */
  Serial.begin(115200);

  serialEsp.println("ArduioNano started."); 
  Serial.println("ArduioNano started."); 
}

String myString;

void loop() {
  
  while (Serial.available() > 0)
  {
    myString = Serial.readString();

    serialEsp.println("from Ardu" + myString);
    
  }

    while (serialEsp.available() > 0)
  {
    myString = serialEsp.readString();

    Serial.println("From ardu" + myString);
    
  }

}
