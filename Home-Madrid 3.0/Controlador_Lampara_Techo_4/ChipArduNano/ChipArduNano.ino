#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define MIN_PULSE_WIDTH 544
#define MAX_PULSE_WIDTH 2400
#define FREQUENCY 200

int servoPin[6] = {1, 2, 3, 5, 6, 7};
int ledPin[4] = {8, 9, 10, 11};

int LED_Brillo_value[4] = {0,0,0,0};
int LED_Current_value[4] = {0,0,0,0};
int LED_Pulse_value[4] = {0,0,0,0};

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

unsigned long tiempoMotor1 = 0;
unsigned long tiempoMotor2 = 0;

unsigned long tiempoLed1 = 0;
unsigned long tiempoLed2 = 0;


#include "colores.hpp"
#include "fsnVarios.hpp"

void setup() {

  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
cargarVecctorColores();

  tiempo1 = millis();
  tiempoLed1 = millis();
  tiempoMotor1 = millis();
  Serial.println("start..");
}

String SerialData = "";
void loop() {

  if (mover_servos() == 2) {
    //delay(3000);
    //ServosPos(random(0,4));
    Clasico();
  }
  
  if (setLedColor() == 2) {
   colorX ccl = colorXList[random(0,52)];
   LED_Brillo_value[0] = ccl.r;
   LED_Brillo_value[1] = ccl.g;
   LED_Brillo_value[2] = ccl.b;
   LED_Brillo_value[3] = random(10, 50);
  }

 //  if (Serial.available() > 0) {
 //    char c = Serial.read();
 //    SerialData += c;

  //   if (c == '\n') {
  //     Serial.print(SerialData);
  //     SerialData = "";
  //   }
  // }
}
