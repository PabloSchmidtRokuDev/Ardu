
#define DEBUG_SERIAL Serial
int pin_R = 2;
int pin_G = 0;
int pin_B = 4;
int pin_W = 16;
int pin_Y = 17;

//definicion de pins
const int motor2Pin1 = 33;  //14 28BYJ48 In1
const int motor2Pin2 = 25;  //12 28BYJ48 In2
const int motor2Pin3 = 26;  //13 28BYJ48 In3
const int motor2Pin4 = 27;  //15 28BYJ48 In4

const int motorPin1 = 14;  //5 28BYJ48 In1
const int motorPin2 = 12;  //4 28BYJ48 In2
const int motorPin3 = 13;  //0 28BYJ48 In3
const int motorPin4 = 15;  //2 28BYJ48 In4

int estado_servo_1 = 0;
int estado_servo_2 = 0;

int goto_servo_1 = 0;
int goto_servo_2 = 0;


int LED_R_value = 0;
int LED_G_value = 0;
int LED_B_value = 0;
int LED_W_value = 0;

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

int brillo = 100;
int brillo_goto = 0;

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;

#include "luces.hpp"
#include "conexion.hpp"

#include "motores.hpp"

void setup() {
  Serial.begin(115200);
   pinMode(pin_R, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_W, OUTPUT);
  pinMode(pin_Y, OUTPUT);
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
  setupWifi();

  tiempo1 = millis();
}

void loop() {

  mover_motores();
  setLedColor();

  webSocket.loop();
}