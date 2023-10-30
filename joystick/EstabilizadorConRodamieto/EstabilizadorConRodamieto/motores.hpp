

//definicion variables
int motorSpeed = 2000;  //variable para fijar la velocidad
int stepCounter1 = 0;   // contador para los pasos

int stepsPerRev = 512;  // pasos para una vuelta completa

const int numSteps = 8;
//const int stepsLookup[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };
const int stepsLookup[8] = { B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001 };
//const int stepsLookup[8] = { B1100, B0100, B0110, B0010,  B0011, B0001,  B1001, B1000 };
void setOutputM1(int step) {

  digitalWrite(motorPin1, bitRead(stepsLookup[step], 0));
  digitalWrite(motorPin2, bitRead(stepsLookup[step], 1));
  digitalWrite(motorPin3, bitRead(stepsLookup[step], 2));
  digitalWrite(motorPin4, bitRead(stepsLookup[step], 3));
  delayMicroseconds(motorSpeed);
}

void clockwiseM1(bool doblePaso = true) {
  Serial.println("clockwiseM1..");
  stepCounter1++;
  if (stepCounter1 >= numSteps) stepCounter1 = 0;
  setOutputM1(stepCounter1);
  //if (doblePaso) clockwiseM1(false);
}

void anticlockwiseM1(bool doblePaso = true) {
  Serial.println("anticlockwiseM1..");
  stepCounter1--;
  if (stepCounter1 < 0) stepCounter1 = numSteps - 1;
  setOutputM1(stepCounter1);
 // if (doblePaso) anticlockwiseM1(false);
}


void mover_motores() {
  if (goto_servo_1 == 1) clockwiseM1();
  if (goto_servo_1 == -1) anticlockwiseM1();

}
