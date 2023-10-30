

#define DEBUG_SERIAL Serial

int pin_R = 12;
int pin_B = 14;
int pin_G = 27;
int pin_W = 26;

int pin_R2 = 15;
int pin_B2 = 2;
int pin_G2 = 4;
int pin_W2 = 16;
//definicion de pins
const int motor2Pin1 = 14;    // 28BYJ48 In1
const int motor2Pin2 = 12;    // 28BYJ48 In2
const int motor2Pin3 = 13;   // 28BYJ48 In3
const int motor2Pin4 = 32;   // 28BYJ48 In4

const int motorPin1 =  33;    // 28BYJ48 In1
const int motorPin2 =  25;    // 28BYJ48 In2
const int motorPin3 =  26;   // 28BYJ48 In3
const int motorPin4 =  27;   // 28BYJ48 In4

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

int LED_R_Current_value = 0;
int LED_G_Current_value = 0;
int LED_B_Current_value = 0;
int LED_W_Current_value = 0;

int LED_R_Brillo_value2 = 0;
int LED_G_Brillo_value2 = 0;
int LED_B_Brillo_value2 = 0;
int LED_W_Brillo_value2 = 0;

int LED_R_Current_value2 = 0;
int LED_G_Current_value2 = 0;
int LED_B_Current_value2 = 0;
int LED_W_Current_value2 = 0;

String LED_Nombre_value = "Black";

int brillo = 30;
int brillo_goto = 0;

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;

#include "luces.hpp"
#include "conexion.hpp"

#include "motores.hpp"

void setup()
{
  Serial.begin(115200);
  pinMode(pin_R, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_W, OUTPUT);

  pinMode(pin_R2, OUTPUT);
  pinMode(pin_B2, OUTPUT);
  pinMode(pin_G2, OUTPUT);
  pinMode(pin_W2, OUTPUT);
  
  //declarar pines como salida
  // pinMode(motorPin1, OUTPUT);
  // pinMode(motorPin2, OUTPUT);
  // pinMode(motorPin3, OUTPUT);
  // pinMode(motorPin4, OUTPUT);

  // pinMode(motor2Pin1, OUTPUT);
  // pinMode(motor2Pin2, OUTPUT);
  // pinMode(motor2Pin3, OUTPUT);
  // pinMode(motor2Pin4, OUTPUT);
  Serial.println("start..");
  DEBUG_SERIAL.begin(115200);
 // setupWifi();

 tiempo1 = millis();
}

void loop()
{
  

  if (setLedColor()){
    delay(3000);
    LED_R_Brillo_value = random(0, 254);
    LED_G_Brillo_value = random(0, 254);
    LED_B_Brillo_value = random(0, 254);
    LED_W_Brillo_value = random(0, 254);

    LED_R_Brillo_value2 = random(0, 254);
    LED_G_Brillo_value2 = random(0, 254);
    LED_B_Brillo_value2 = random(0, 254);
    LED_W_Brillo_value2 = random(0, 254);
  }
// mover_motores();
//   for (int i = 0; i <2000 ; i++)
//   {
//     clockwiseM1();
//     clockwiseM2();
//     delayMicroseconds(motorSpeed);

//   }

//     for (int i = 0; i <2000 ; i++)
//   {
// anticlockwiseM1();
// anticlockwiseM2();
// delayMicroseconds(motorSpeed);
//   }


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