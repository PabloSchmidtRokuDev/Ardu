

#define DEBUG_SERIAL Serial

// int pin_R = 22;
// int pin_B = 19;
// int pin_G = 23;
//definicion de pins
const int motorPin1 = 12;    // 28BYJ48 In1
const int motorPin2 = 14;    // 28BYJ48 In2
const int motorPin3 = 27;   // 28BYJ48 In3
const int motorPin4 = 26;   // 28BYJ48 In4

const int motor1Pin =  15;    // 28BYJ48 In1
const int motor2Pin =  2;    // 28BYJ48 In2
const int motor3Pin =  0;   // 28BYJ48 In3
const int motor4Pin =  4;   // 28BYJ48 In4
const int motor5Pin =  16;
const int motor6Pin =  17;

int estado_motor[6] = {0,0,0,0,0,0};
int goto_motor[6] = {-1,1,-1,1,-1,1};

// int LED_R_value = 0;
// int LED_G_value = 0;
// int LED_B_value = 0;

// int LED_R_Brillo_value = 0;
// int LED_G_Brillo_value = 0;
// int LED_B_Brillo_value = 0;

// int LED_R_Current_value = 0;
// int LED_G_Current_value = 0;
// int LED_B_Current_value = 0;

// String LED_Nombre_value = "Black";

// int brillo = 30;
// int brillo_goto = 0;
// 
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;

// #include "luces.hpp"
// #include "conexion.hpp"

#include "motores.hpp"

void setup()
{
  Serial.begin(115200);
  // pinMode(pin_R, OUTPUT);
  // pinMode(pin_B, OUTPUT);
  // pinMode(pin_G, OUTPUT);

  //declarar pines como salida
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  pinMode(motor3Pin, OUTPUT);
  pinMode(motor4Pin, OUTPUT);
  pinMode(motor5Pin, OUTPUT);
  pinMode(motor6Pin, OUTPUT);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  Serial.println("start..");
  DEBUG_SERIAL.begin(115200);
 // setupWifi();

 tiempo1 = millis();
}

void loop()
{
  
 mover_motores();
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