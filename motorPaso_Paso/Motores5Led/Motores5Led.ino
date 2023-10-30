
#include <RGBWConverter.h>
#define DEBUG_SERIAL Serial

int pin_R = 12;
int pin_G = 14;
int pin_B = 27;
int pin_W = 26;

int pin_R2 = 19;
int pin_G2 = 18;
int pin_B2 = 5;
int pin_W2 = 17;

RGBWConverter converter(240, 215, 200, true);

//definicion de pins

const int motorPin1 = 15;  // 28BYJ48 In1
const int motorPin2 = 2;   // 28BYJ48 In2
const int motorPin3 = 4;   // 28BYJ48 In3
const int motorPin4 = 16;  // 28BYJ48 In4

const int motor2Pin1 = 14;  // 28BYJ48 In1
const int motor2Pin2 = 12;  // 28BYJ48 In2
const int motor2Pin3 = 13;  // 28BYJ48 In3
const int motor2Pin4 = 32;  // 28BYJ48 In4

int estado_servo_1 = 0;
int estado_servo_2 = 0;

int goto_servo_1 = 0;
int goto_servo_2 = 0;

int LED_R_value = 0;
int LED_G_value = 0;
int LED_B_value = 0;

int LED_R_Brillo_value = 0;
int LED_G_Brillo_value = 0;
int LED_B_Brillo_value = 0;
int LED_W_Brillo_value = 0;
int LED_Y_Brillo_value = 0;

int LED_R_Current_value = 0;
int LED_G_Current_value = 0;
int LED_B_Current_value = 0;
int LED_W_Current_value = 0;
int LED_Y_Current_value = 0;

int brillo = 30;
int brillo_goto = 0;

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;

//#include "luces.hpp"
//#include "conexion.hpp"

#include "motores.hpp"

void setup() {
  Serial.begin(115200);
  // pinMode(pin_R, OUTPUT);
  // pinMode(pin_B, OUTPUT);
  // pinMode(pin_G, OUTPUT);
  // pinMode(pin_W, OUTPUT);

  // pinMode(pin_R2, OUTPUT);
  // pinMode(pin_B2, OUTPUT);
  // pinMode(pin_G2, OUTPUT);
  // pinMode(pin_W2, OUTPUT);

  // pinMode(pin_R2, OUTPUT);
  // pinMode(pin_B2, OUTPUT);
  // pinMode(pin_G2, OUTPUT);
  // pinMode(pin_W2, OUTPUT);

  //declarar pines como salida
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // pinMode(motor2Pin1, OUTPUT);
  // pinMode(motor2Pin2, OUTPUT);
  // pinMode(motor2Pin3, OUTPUT);
  // pinMode(motor2Pin4, OUTPUT);
  Serial.println("start..");
  DEBUG_SERIAL.begin(115200);
  // setupWifi();

  tiempo1 = millis();
}

byte red = 31;
byte green = 0;
byte blue = 0;
byte state = 0;
bool flag = false;

int cc = 0;
void loop() {

  // if (setLedColor()) {


  //   switch (state) {
  //     case 0:
  //       green++;
  //       if (green == 64) {
  //         green = 63;
  //         state = 1;
  //       }
  //       break;
  //     case 1:
  //       red--;
  //       if (red == 255) {
  //         red = 0;
  //         state = 2;
  //       }
  //       break;
  //     case 2:
  //       blue++;
  //       if (blue == 32) {
  //         blue = 31;
  //         state = 3;
  //       }
  //       break;
  //     case 3:
  //       green--;
  //       if (green == 255) {
  //         green = 0;
  //         state = 4;
  //       }
  //       break;
  //     case 4:
  //       red++;
  //       if (red == 32) {
  //         red = 31;
  //         state = 5;
  //       }
  //       break;
  //     case 5:
  //       blue--;
  //       if (blue == 255) {
  //         blue = 0;
  //         state = 0;
  //         flag = !flag;
  //       }
  //       break;
  //   }

  //   if (flag) {

  //     LED_R_Brillo_value = red;
  //     LED_G_Brillo_value = green;
  //     LED_B_Brillo_value = blue;
  //     LED_W_Brillo_value = 0;
  //   } else {
  //     auto c = converter.RGBToRGBW(red, green, blue);  //colores[iC][0], colores[iC][1], colores[iC][2]);
  //     LED_R_Brillo_value = c.r;
  //     LED_G_Brillo_value = c.g;
  //     LED_B_Brillo_value = c.b;
  //     LED_W_Brillo_value = c.w;
  //   }
  // }
  // mover_motores();
  for (int i = 0; i < 12000; i++) {
    clockwiseM1();
    // clockwiseM2();
    //delayMicroseconds(motorSpeed);
  }

  for (int i = 0; i < 12000; i++) {
    anticlockwiseM1();
    // anticlockwiseM2();
    //delayMicroseconds(motorSpeed);
  }

// cc++;
//   if (cc == 2) {
//     motorSpeed = 1800;
//      Serial.println("cambio  2- 1800 -array 1");
//      delay(2000);
//   } else if (cc == 6) {
//     motorSpeed = 1500;
//      Serial.println("cambio  3- 1500 -array 1");
//      delay(2000);
//   } else if (cc == 8) {
//     stepsLookup[0] = B1100;
//     stepsLookup[1] = B0110;
//     stepsLookup[2] = B0011;
//     stepsLookup[3] = B1001;
//     stepsLookup[4] = B1100;
//     stepsLookup[5] = B0110;
//     stepsLookup[6] = B0011;
//     stepsLookup[7] = B1001;
//     motorSpeed = 2500;
//          Serial.println("cambio 4- 2500 -array 2");
//      delay(2000);

//   } else if (cc == 10) {
//     motorSpeed = 1800;
//      Serial.println("cambio5- 1800 -array 2");
//      delay(2000);

//   } else if (cc == 12) {
//     motorSpeed = 1500;

//  Serial.println("cambio 6- 1500 -array 2");
//      delay(2000);

//   } else if (cc == 14) {
//     stepsLookup[0] = B1100;
//     stepsLookup[1] = B1110;
//     stepsLookup[2] = B0110;
//     stepsLookup[3] = B0111;
//     stepsLookup[4] = B0011;
//     stepsLookup[5] = B1101;
//     stepsLookup[6] = B1001;
//     stepsLookup[7] = B1101;
  
//   motorSpeed = 2500;
//    Serial.println("cambio 7- 2500 -array 3");
//      delay(2000);
// }
// else if (cc == 16) {
//   motorSpeed = 1800;
//    Serial.println("cambio 8- 1800 -array 3");
//      delay(2000);

// }
// else if (cc == 18) {
//   motorSpeed = 1500;
//    Serial.println("cambio 9- 1500 -array 3");
//      delay(2000);
// }
// else if (cc == 20) {
//   stepsLookup[0] = B1000;
//   stepsLookup[1] = B1100;
//   stepsLookup[2] = B0100;
//   stepsLookup[3] = B0110;
//   stepsLookup[4] = B0010;
//   stepsLookup[5] = B0011;
//   stepsLookup[6] = B0001;
//   stepsLookup[7] = B1001;

// motorSpeed = 2500;
// cc = -1;

//  Serial.println("cambio 10- 2500 -array 1");
//      delay(2000);
// }

// for (int i = 0; i < stepsPerRev * 2; i++)
// {
//   anticlockwise();
//   delayMicroseconds(motorSpeed);
//   //delay(10);
//   //Serial.print("<");
// }
// delay(1000);
// webSocket.loop();
}