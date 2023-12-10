
#define DEBUG_SERIAL Serial
int pin_R = 19;
int pin_G = 18;
int pin_B = 5;
int pin_W = 17;

int LED_R_value = 0;
int LED_G_value = 0;
int LED_B_value = 0;
int LED_W_value = 0;

int LED_R_Brillo_value = 10;
int LED_G_Brillo_value = 10;
int LED_B_Brillo_value = 10;
int LED_W_Brillo_value = 10;

int LED_R_Current_value = 0;
int LED_G_Current_value = 0;
int LED_B_Current_value = 0;
int LED_W_Current_value = 0;


int brillo = 100;
int brillo_goto = 0;


//definicion de pins
const int motor2Pin1 = 32;  //14 28BYJ48 In1
const int motor2Pin2 = 33;  //12 28BYJ48 In2
const int motor2Pin3 = 25;  //13 28BYJ48 In3
const int motor2Pin4 = 16;  //15 28BYJ48 In4

const int motorPin1 = 12;  //5 28BYJ48 In1
const int motorPin2 = 14;  //4 28BYJ48 In2
const int motorPin3 = 27;  //0 28BYJ48 In3
const int motorPin4 = 26;  //2 28BYJ48 In4

int estado_servo_1 = 0;
int estado_servo_2 = 0;

int goto_servo_1 = 0;
int goto_servo_2 = 0;
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

unsigned long tiempoLed1 = 0;
unsigned long tiempoLed2 = 0;


#define TOTAL_EFECTOS 2
uint8_t gCurrentPatternNumber = 0;


#include "luces.hpp"
#include "colores.hpp"
#include "conexion.hpp"

#include "motores.hpp"
void setup() {
  Serial.begin(115200);
  pinMode(pin_R, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_W, OUTPUT);


  analogWrite(pin_R, 0);
  analogWrite(pin_B, 0);
  analogWrite(pin_G, 0);
  analogWrite(pin_W, 0);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor2Pin3, OUTPUT);
  pinMode(motor2Pin4, OUTPUT);
  cargarVecctorColores();

  Serial.println("start..");
  DEBUG_SERIAL.begin(115200);
  setupWifi();

  tiempo1 = millis();

  tiempoLed1 = millis();
}

void loop() {
  int estado = setLedColor();
  //Serial.println("estado.."+ String(estado));
  // if (estado == 2) {

  //   colorX ccl = colorXList[random(0, 52)];
  //   LED_R_Brillo_value = ccl.r;
  //   LED_G_Brillo_value = ccl.g;
  //   LED_B_Brillo_value = ccl.b;
  //   LED_W_Brillo_value = random(10, 50);
  //  Serial.println("cambio.."+ String(LED_R_Brillo_value));
  // }

  setLedColor();
  mover_motores();
  webSocket.loop();
}