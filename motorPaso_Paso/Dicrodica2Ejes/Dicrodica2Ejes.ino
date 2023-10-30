

#define DEBUG_SERIAL Serial

int pin_Rele = 16;
//definicion de pins
const int motor2Pin1 = 15;  //14 28BYJ48 In1
const int motor2Pin2 = 13;  //12 28BYJ48 In2
const int motor2Pin3 = 12;  //13 28BYJ48 In3
const int motor2Pin4 = 14;  //15 28BYJ48 In4

const int motorPin1 = 2;  //5 28BYJ48 In1
const int motorPin2 = 0;  //4 28BYJ48 In2
const int motorPin3 = 4;  //0 28BYJ48 In3
const int motorPin4 = 5;  //2 28BYJ48 In4

int estado_servo_1 = 0;
int estado_servo_2 = 0;

int goto_servo_1 = 0;
int goto_servo_2 = 0;

bool LED_Rele_value = false;

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;

#include "conexion.hpp"

#include "motores.hpp"

void setup() {
  Serial.begin(115200);
  pinMode(pin_Rele, OUTPUT);

  //declarar pines como salida
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor2Pin3, OUTPUT);
  pinMode(motor2Pin4, OUTPUT);
  Serial.println("start..");
  DEBUG_SERIAL.begin(115200);
  // setupWifi();

  tiempo1 = millis();
}

void loop() {

  //mover_motores();

  for (int i = 0; i < 500; i++) {
    clockwiseM1();
  }

  for (int i = 0; i < 500; i++) {
    clockwiseM2();
  }


  for (int i = 0; i < 500; i++) {

    anticlockwiseM2();
  }

  for (int i = 0; i < 500; i++) {
    anticlockwiseM1();
  }



  delay(1000);
  //webSocket.loop();
}