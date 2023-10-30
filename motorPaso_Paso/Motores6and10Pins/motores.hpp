

//definicion variables
int motorSpeed = 5000;  //variable para fijar la velocidad
int stepCounter1 = 0;   // contador para los pasos
int stepCounter2 = 0;
int stepsPerRev = 512;  // pasos para una vuelta completa

const int numSteps = 8;
const int stepsLookup[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };
//const int stepsLookup[8] = { B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001 };
//const int stepsLookup[8] = { B1100, B1110, B0110, B0111,  B0011, B1101,  B1001, B1101 };
void setOutputM1(int step, bool withDelay = true) {

  digitalWrite(motorPin1, bitRead(stepsLookup[step], 0));
  digitalWrite(motorPin2, bitRead(stepsLookup[step], 1));
  digitalWrite(motorPin3, bitRead(stepsLookup[step], 2));
  digitalWrite(motorPin4, bitRead(stepsLookup[step], 3));
  if (withDelay) delayMicroseconds(motorSpeed);
}

void clockwiseM(int motorNum) {
  Serial.println("clockwiseM1..");

setOutputM1(estado_motor[motorNum], false);

if (motorNum == 0) digitalWrite(motor1Pin, HIGH);
if (motorNum == 1) digitalWrite(motor2Pin, HIGH);
if (motorNum == 2) digitalWrite(motor3Pin, HIGH);
if (motorNum == 3) digitalWrite(motor4Pin, HIGH);
if (motorNum == 4) digitalWrite(motor5Pin, HIGH);
if (motorNum == 5) digitalWrite(motor6Pin, HIGH);

  estado_motor[motorNum]++;
  if (estado_motor[motorNum] >= numSteps) estado_motor[motorNum] = 0;
  setOutputM1(estado_motor[motorNum]);
}

void anticlockwiseM(int motorNum) {
  Serial.println("anticlockwiseM1..");

  setOutputM1(estado_motor[motorNum], false);

if (motorNum == 0) digitalWrite(motor1Pin, HIGH);
if (motorNum == 1) digitalWrite(motor2Pin, HIGH);
if (motorNum == 2) digitalWrite(motor3Pin, HIGH);
if (motorNum == 3) digitalWrite(motor4Pin, HIGH);
if (motorNum == 4) digitalWrite(motor5Pin, HIGH);
if (motorNum == 5) digitalWrite(motor6Pin, HIGH);


  estado_motor[motorNum]--;
  if (estado_motor[motorNum] < 0) estado_motor[motorNum] = numSteps - 1;
 setOutputM1(estado_motor[motorNum]);
}

void mover_motores() {
  for (int i=0; i<6;i++){
    if (goto_motor[i] > 0) clockwiseM(i);
    if (goto_motor[i] < 0) anticlockwiseM(i);
  }

digitalWrite(motor1Pin, LOW);
digitalWrite(motor2Pin, LOW);
digitalWrite(motor3Pin, LOW);
digitalWrite(motor4Pin, LOW);
digitalWrite(motor5Pin, LOW);
digitalWrite(motor6Pin, LOW);
}
