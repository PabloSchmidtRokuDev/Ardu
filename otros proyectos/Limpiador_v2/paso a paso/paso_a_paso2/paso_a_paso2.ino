const int dirPin = 5;
const int stepPin = 2;
const int stepsPerRevolution = 400;
const int delayMS = 30;
const int delayMS2 = 3000;

void setup()
{
  Serial.begin(9600);
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
   Serial.println("ready go");
}
void loop()
{
  // Set motor direction clockwise
  digitalWrite(dirPin, HIGH);
Serial.println("ready go l1");
  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayMS);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayMS2);
  }
  delay(500); // Wait a second
  
  // Set motor direction counterclockwise
  digitalWrite(dirPin, LOW);
Serial.println("ready go l2");
  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {

    
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayMS);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayMS2);
  }
  delay(500); // Wait a second
}
