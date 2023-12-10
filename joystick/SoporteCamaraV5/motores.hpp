

//definicion variables
int motorSpeed =3000;  //variable para fijar la velocidad
int stepCounter1 = 0;   // contador para los pasos
int stepCounter2 = 0;
int stepsPerRev = 512;  // pasos para una vuelta completa

const int numSteps = 8;
//const int stepsLookup[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };
const int stepsLookup[8] = { B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001 };
//const int stepsLookup[8] = { B1000, B0100, B0010, B0001,  B1000, B0100,  B0010, B0001 };
void setOutputM1(int step) {

  digitalWrite(motorPin1, bitRead(stepsLookup[step], 0));
  digitalWrite(motorPin2, bitRead(stepsLookup[step], 1));
  digitalWrite(motorPin3, bitRead(stepsLookup[step], 2));
  digitalWrite(motorPin4, bitRead(stepsLookup[step], 3));
  delayMicroseconds(motorSpeed);
  
}

void setOutputM2(int step) {
  digitalWrite(motor2Pin1, bitRead(stepsLookup[step], 0));
  digitalWrite(motor2Pin2, bitRead(stepsLookup[step], 1));
  digitalWrite(motor2Pin3, bitRead(stepsLookup[step], 2));
  digitalWrite(motor2Pin4, bitRead(stepsLookup[step], 3));
  delayMicroseconds(motorSpeed);

}

void clockwiseM1(bool doblePaso = true) {
  Serial.println("clockwiseM1..");
  stepCounter1++;
  if (stepCounter1 == numSteps) stepCounter1 = 0;
  setOutputM1(stepCounter1);
  //if (doblePaso) clockwiseM1(false);
}

void anticlockwiseM1(bool doblePaso = true) {
  //Serial.println("anticlockwiseM1..");
  stepCounter1--;
  if (stepCounter1 < 0) stepCounter1 = numSteps - 1;
  setOutputM1(stepCounter1);
 // if (doblePaso) anticlockwiseM1(false);
}

void clockwiseM2(bool doblePaso = true) {
  Serial.println("clockwiseM2..");
  stepCounter2++;
  if (stepCounter2 == numSteps) stepCounter2 = 0;
  setOutputM2(stepCounter2);
  //if (doblePaso) clockwiseM2(false);
}

void anticlockwiseM2(bool doblePaso = true) {
  Serial.println("anticlockwiseM2..");
  stepCounter2--;
  if (stepCounter2 < 0) stepCounter2 = numSteps - 1;
  setOutputM2(stepCounter2);
  //if (doblePaso) anticlockwiseM2(false);
}


void mover_motores() {
  if (goto_servo_1 > 0) clockwiseM1();
  if (goto_servo_1 < 0) anticlockwiseM1();

  if (goto_servo_2 > 0) clockwiseM2();
  if (goto_servo_2 < 0) anticlockwiseM2();

if (goto_servo_1 == 0 && goto_servo_2 == 0){
  digitalWrite(motorPin1, 0);
  digitalWrite(motorPin2, 0);
  digitalWrite(motorPin3, 0);
  digitalWrite(motorPin4, 0);

  digitalWrite(motor2Pin1,0);
  digitalWrite(motor2Pin2,0);
  digitalWrite(motor2Pin3,0);
  digitalWrite(motor2Pin4,0);
}
}