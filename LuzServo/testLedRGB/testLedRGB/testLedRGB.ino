#include <Servo.h>

#include "colores.hpp"

#define PIN_servo1 24
#define PIN_servo2 26


Servo servo1;
Servo servo2;

int pin_R = 10;
int pin_B = 11;
int pin_G = 12;

void setup() {
  Serial.begin(115200);

  servo1.attach(PIN_servo1);
  servo1.write(0);
  servo2.attach(PIN_servo2);
  servo2.write(180);

  pinMode(pin_R, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_G, OUTPUT);

  cargarVecctorColores();
}

int ccc = 0;
int i;
colorX currentColor = colorXList[8];

long randomNumber, randomNumber2;
int sa1 = 0;
int sa2 = 180;

void loop() {

  colorX cl = colorXList[random(0, 150)];
  changeColor(currentColor, cl);
  currentColor = cl;

  delay(3000);
}

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

    servo1.write(sa1);
    servo2.write(sa2);

    delay(20);
  }
}
