#include <Arduino.h>
//#include <Servo.h>

//#include "colores.hpp"

#define PIN_servo1 18
#define PIN_servo2 19

/*
Servo servo1;
Servo servo2;
*/
#define DEBUG_SERIAL Serial

int pin_R = 26;
int pin_B = 25;
int pin_G = 33;
//definicion de pins


const int motorPin1 = 13;  // 28BYJ48 In1
const int motorPin2 = 12;  // 28BYJ48 In2
const int motorPin3 = 14;  // 28BYJ48 In3
const int motorPin4 = 27;  // 28BYJ48 In4

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

int LED_R_Current_value = 0;
int LED_G_Current_value = 0;
int LED_B_Current_value = 0;

String LED_Nombre_value = "Black";

int brillo = 100;
int brillo_goto = 0;

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;

//#include "conexion.hpp"
//#include "luces.hpp"
#include "motores.hpp"

void setup() {

  Serial.begin(115200);
  DEBUG_SERIAL.begin(115200);
  // pinMode(pin_R, OUTPUT);
  // pinMode(pin_B, OUTPUT);
  // pinMode(pin_G, OUTPUT);

  // //declarar pines como salida
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  Serial.println("start..");
  
  // setupWifi();

  tiempo1 = millis();
  /*
  servo1.attach(PIN_servo1, 500, 2400);
  servo1.write(0);
  servo2.attach(PIN_servo2, 500, 2400);
  servo2.write(180);
*/
}

// int ccc = 0;
// int i;
// colorX currentColor = colorXList[8];

// long randomNumber, randomNumber2;
// int sa1 = 0;
// int sa2 = 180;

void loop() {
 /*
  colorX cl = colorXList[random(0, 150)];
  changeColor(currentColor, cl);
  currentColor = cl;
 
  servo1.write(random(0, 180));
  servo2.write(random(0, 180));
*/

  for (int i = 0; i < 2000; i++) {
    clockwiseM1();
    delayMicroseconds(motorSpeed);
  }

  for (int i = 0; i < 2000; i++) {
    anticlockwiseM1();
    delayMicroseconds(motorSpeed);
  }

  delay(3000);
}
/*
void changeColor(colorX cFrom, colorX cTo) {
  int cR = cFrom.r;
  int cG = cFrom.g;
  int cB = cFrom.b;

  randomNumber = random(1, 90);
  randomNumber2 = random(120, 180);

  while (cR != cTo.r || cG != cTo.g || cB != cTo.b) {

    if (cR != cTo.r) {
      if (cR < cTo.r) cR++;
      else cR--;
    }

    if (cG != cTo.g) {
      if (cG < cTo.g) cG++;
      else cG--;
    }

    if (cB != cTo.b) {
      if (cB < cTo.b) cB++;
      else cB--;
    }

    analogWrite(pin_R, cR);
    analogWrite(pin_B, cB);
    analogWrite(pin_G, cG);

    if (sa1 != randomNumber) {
      if (sa1 < randomNumber) sa1++;
      else sa1--;
    }

    if (sa2 != randomNumber2) {
      if (sa2 < randomNumber2) sa2++;
      else sa2--;
    }


    delay(20);
  }
}
*/