/*Example sketch to control a stepper motor with A4988 stepper motor driver, AccelStepper library and Arduino: acceleration and deceleration. More info: https://www.makerguides.com */

// Include the AccelStepper library:
#include <AccelStepper.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin1 14
#define stepPin1 27

#define dirPin2 12
#define stepPin2 13

#define dirPin3 2
#define stepPin3 15

#define dirPin4 16
#define stepPin4 4

#define dirPin5 25
#define stepPin5 25

#define dirPin6 33
#define stepPin6 32



#define motorInterfaceType 1

// Create a new instance of the AccelStepper class:
AccelStepper stepper1 = AccelStepper(motorInterfaceType, stepPin1, dirPin1);
 AccelStepper stepper2 = AccelStepper(motorInterfaceType, stepPin2, dirPin2);
AccelStepper stepper3 = AccelStepper(motorInterfaceType, stepPin3, dirPin3);
AccelStepper stepper4 = AccelStepper(motorInterfaceType, stepPin4, dirPin4);
AccelStepper stepper5 = AccelStepper(motorInterfaceType, stepPin5, dirPin5);
AccelStepper stepper6 = AccelStepper(motorInterfaceType, stepPin6, dirPin6);



void setSpeedAceleracion(int speed, int aceleracion) {

  //150, 10
  stepper1.setMaxSpeed(50);
  stepper1.setAcceleration(5);

   stepper2.setMaxSpeed(50);
   stepper2.setAcceleration(5);

  stepper3.setMaxSpeed(500);
  stepper3.setAcceleration(100);

  stepper4.setMaxSpeed(500);
  stepper4.setAcceleration(100);

  stepper5.setMaxSpeed(speed);
  stepper5.setAcceleration(aceleracion);

  stepper6.setMaxSpeed(speed);
  stepper6.setAcceleration(aceleracion);
}

void setup() {
  // Set the maximum speed and acceleration:

  Serial.begin(115200);

  Serial.println("start!!");

  //setPinsInverted
  setSpeedAceleracion(150, 10);

  stepper1.setPinsInverted(true);
  stepper2.setPinsInverted(true);
  //--stepper3.setPinsInverted(true);
   stepper4.setPinsInverted(true);
   stepper5.setPinsInverted(true);
   stepper6.setPinsInverted(true);

  // stepper1.moveTo(30000);  //aro chico 1
  // stepper1.runToPosition();
  //  stepper2.moveTo(30000);
  //  stepper2.runToPosition();
  // stepper3.moveTo(30000);
  // stepper3.runToPosition();
  // stepper4.moveTo(30000);
  // stepper4.runToPosition();
  // stepper5.moveTo(10000);
  // stepper6.moveTo(10000);


      // stepper2.moveTo(10000);
      // stepper3.moveTo(10000);
      // stepper4.moveTo(10000);
      // stepper5.moveTo(10000);
      // stepper6.moveTo(10000);
      // stepper1.moveTo(10000);


  // Serial.println("m1");
  //    stepper1.runToPosition();//aro chico 1
  //     Serial.println("m2");
  //    stepper2.runToPosition();
  //    Serial.println("m3");
  //    stepper3.runToPosition();
  //    Serial.println("m4");
  //    stepper4.runToPosition();
  //    Serial.println("m5");
  //    stepper5.runToPosition();
  //    Serial.println("m6");
  //    stepper6.runToPosition();
       Serial.println("start!! 2");
}

enum calibrandoEstadoEnum {
  cal_Pendiente,
  cal_go_0_all,
  cal_go_0_m1,
  cal_go_0_m2,
  cal_go_0_m3,
  cal_go_0_m4,
  cal_go_0_m5,
  cal_go_0_m6,
  cal_Completo
};

calibrandoEstadoEnum calibrandoEstado = cal_Pendiente;
bool running = false;

int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int c = 0;

bool end = false;
String myString = "";

void loop() {
   stepper1.run();//br3 o 4
   stepper2.run();//br3 o 4
   stepper3.run();//br cam

    stepper4.run();//br2
    stepper5.run();//br cam
    stepper6.run();

  // if (Calibrado()) {

    // while (Serial.available() > 0) {
    //   myString = Serial.readString();
    //   String letra = myString.substring(0, 1);

    //   int textLen = myString.length() - 1;
    //   String resto = myString.substring(1, textLen);

    //   Serial.print("letra: ");
    //   Serial.println(letra);

    //   if (letra == "p") {
    //     int Valor = resto.toInt();

    //   stepper2.moveTo(Valor);
    //   stepper3.moveTo(Valor);
    //   stepper4.moveTo(Valor);
    //   stepper5.moveTo(Valor);
    //   stepper6.moveTo(Valor);
    //   stepper1.moveTo(Valor);

    //   } else if (letra == "s") {
    //   }
    // }


    bool f = stepper1.currentPosition() == pos1;
    // f = f && stepper2.currentPosition() == pos2;
    // f = f && stepper3.currentPosition() == pos3;

    // if (c==1000){
    // Serial.print("m1: " + String(stepper.currentPosition()) + " %% " + String(pos1));
    // Serial.print("\t m2: " + String(stepper2.currentPosition()) + " %% " + String(pos2));
    // Serial.println("\t m3: " + String(stepper3.currentPosition()) + " %% " + String(pos3));

    // c=0;
    // }
    // c++;



     if (f) {
       //if (!end) {}
       if (stepper1.currentPosition() == 0) {
         pos1 = random(0, 1000);
       } else {
         pos1 = 0;
       }



       stepper2.moveTo(pos1);
       stepper3.moveTo(pos1*8);
       stepper4.moveTo(pos1*8);
       stepper5.moveTo(pos1*8);
       stepper6.moveTo(pos1);
       stepper1.moveTo(pos1);
     }
   
}

// bool Calibrado() {
//   bool flag = false;

//   if (calibrandoEstado != cal_Completo) {
//     if (running) {
//       //Serial.print(" Sensor: ");
//       //Serial.print(analogRead(A6));
//       if (DigitalRead(A6) <= 100) {
//         Serial.println(" Sensor....");
//         running = false;
//       } else {
//         // Serial.println(" running.");

//         stepper1.run();
//         stepper2.run();
//         stepper3.run();
//         stepper4.run();
//         stepper5.run();
//         stepper6.run();
//       }

//     } else if (calibrandoEstado == cal_Pendiente) {
//       Serial.println(" cal_Pendiente....");
//      // setSpeedAceleracion(2000, 500);

//       stepper1.moveTo(-300);
//       stepper2.moveTo(-300);
//       stepper3.moveTo(-300);
//       stepper4.moveTo(-300);
//       stepper5.moveTo(-300);
//       stepper6.moveTo(-300);

//       calibrandoEstado = cal_go_0_all;
//       running = true;
//     } else if (calibrandoEstado == cal_go_0_all) {
//       Serial.println(" cal_go_0_all....");

//       stepper1.setCurrentPosition(0);
//       stepper2.setCurrentPosition(0);
//       stepper3.setCurrentPosition(0);
//       stepper4.setCurrentPosition(0);
//       stepper5.setCurrentPosition(0);
//       stepper6.setCurrentPosition(0);

//       stepper1.runToNewPosition(500);
//       stepper2.runToNewPosition(500);
//       stepper3.runToNewPosition(500);
//       stepper4.runToNewPosition(500);
//       stepper5.runToNewPosition(500);
//       stepper6.runToNewPosition(500);

//       stepper1.moveTo(-300);
//       running = true;
//       calibrandoEstado = cal_go_0_m1;

//     } else if (calibrandoEstado == cal_go_0_m1) {
//       Serial.println(" cal_go_0_m1....");

//     //  stepper1.stop();

//       stepper1.setCurrentPosition(0);
//       stepper1.runToNewPosition(500);
//       stepper1.setCurrentPosition(0);

//       stepper2.moveTo(-3000);
//       running = true;
//       calibrandoEstado = cal_go_0_m2;

//     } else if (calibrandoEstado == cal_go_0_m2) {
//       Serial.println(" cal_go_0_m2....");

//   //    stepper2.stop();

//       stepper2.setCurrentPosition(0);
//       stepper2.runToNewPosition(500);
//       stepper2.setCurrentPosition(0);

//       stepper3.moveTo(-3000);
//       running = true;
//       calibrandoEstado = cal_go_0_m3;

//     } else if (calibrandoEstado == cal_go_0_m3) {
//       Serial.println(" cal_go_0_m3....");

//     //  stepper3.stop();
//        stepper3.setCurrentPosition(0);
//       stepper3.runToNewPosition(500);
//        stepper3.setCurrentPosition(0);

//       stepper4.moveTo(-3000);
//       running = true;
//       calibrandoEstado = cal_go_0_m4;

//     } else if (calibrandoEstado == cal_go_0_m4) {
//       Serial.println(" cal_go_0_m4....");

//       //stepper4.stop();
//        stepper4.setCurrentPosition(0);
//       stepper4.runToNewPosition(500);
//        stepper4.setCurrentPosition(0);


//       stepper5.moveTo(-3000);
//       running = true;
//       calibrandoEstado = cal_go_0_m5;

//     } else if (calibrandoEstado == cal_go_0_m5) {
//       Serial.println(" cal_go_0_m5....");

//       //stepper5.stop();

//       stepper5.setCurrentPosition(0);
//       stepper5.runToNewPosition(500);
//       stepper5.setCurrentPosition(0);


//       stepper6.moveTo(-3000);
//       running = true;
//       calibrandoEstado = cal_go_0_m6;

//     } else if (calibrandoEstado == cal_go_0_m6) {
//       Serial.println(" cal_go_0_m6....");

//     //  stepper6.stop();
//      stepper6.setCurrentPosition(0);
//       stepper6.runToNewPosition(500);
     
//       stepper1.setCurrentPosition(0);
//       stepper2.setCurrentPosition(0);
//       stepper3.setCurrentPosition(0);
//       stepper4.setCurrentPosition(0);
//       stepper5.setCurrentPosition(0);
//       stepper6.setCurrentPosition(0);
//       calibrandoEstado = cal_Completo;


//       flag = true;
//     }


//   } else {
//     flag = true;
//   }

//   return flag;
// }