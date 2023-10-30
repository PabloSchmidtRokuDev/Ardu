
#define DEBUG_SERIAL Serial

int pin_R_o = 2;
int pin_G_o = 4;
int pin_B_o = 15;

int pin_R = 13;
int pin_G = 27;
int pin_B = 12;
int pin_W = 26;
int pin_Y = 14;



//definicion de pins
const int motor2Pin1 = 0;  //14 28BYJ48 In1
const int motor2Pin2 = 0;  //12 28BYJ48 In2
const int motor2Pin3 = 0;  //13 28BYJ48 In3
const int motor2Pin4 = 0;  //15 28BYJ48 In4

const int motorPin1 = 0;  //5 28BYJ48 In1
const int motorPin2 = 0;  //4 28BYJ48 In2
const int motorPin3 = 0;  //0 28BYJ48 In3
const int motorPin4 = 0;  //2 28BYJ48 In4

int estado_servo_1 = 0;
int estado_servo_2 = 0;

int goto_servo_1 = 0;
int goto_servo_2 = 0;


int LED_R_o_value = 0;
int LED_G_o_value = 0;
int LED_B_o_value = 0;

int LED_R_value = 0;
int LED_G_value = 0;
int LED_B_value = 0;
int LED_W_value = 0;
int LED_Y_value = 0;

int LED_R_o_Brillo_value = 0;
int LED_G_o_Brillo_value = 0;
int LED_B_o_Brillo_value = 0;

int LED_R_Brillo_value = 0;
int LED_G_Brillo_value = 0;
int LED_B_Brillo_value = 0;
int LED_W_Brillo_value = 0;
int LED_Y_Brillo_value = 0;

int LED_R_o_Current_value = 0;
int LED_G_o_Current_value = 0;
int LED_B_o_Current_value = 0;

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
#include "colores.hpp"
//#include "conexion.hpp"

//#include "motores.hpp"
String CL = "";
void setup() {
  Serial.begin(115200);

  pinMode(pin_R_o, OUTPUT);
  pinMode(pin_B_o, OUTPUT);
  pinMode(pin_G_o, OUTPUT);

  pinMode(pin_R, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_W, OUTPUT);
  pinMode(pin_Y, OUTPUT);


  analogWrite(pin_R_o, 0);
  analogWrite(pin_B_o, 0);
  analogWrite(pin_G_o, 0);

  analogWrite(pin_R, 0);
  analogWrite(pin_B, 0);
  analogWrite(pin_G, 0);
  analogWrite(pin_W, 0);
  analogWrite(pin_Y, 0);

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
  //setupWifi();

cargarVecctorColores();

  tiempo1 = millis();
  LED_R_o_Brillo_value = 0;
  LED_R_Brillo_value = 0;

  LED_G_o_Brillo_value = 0;
  LED_G_Brillo_value = 0;

  LED_B_o_Brillo_value = 0;
  LED_B_Brillo_value = 0;

  LED_W_Brillo_value = 0;
  LED_Y_Brillo_value = 0;

  Serial.println("LED_R_   o_Brillo_value..");

      LED_R_Brillo_value = 255;
      LED_R_o_Brillo_value = 255;

  CL = "Rojo O";
}


void loop() {


  int estado = setLedColor();
//  Serial.println("estado.."+ String(estado));
  if (estado == 2) {
delay(12);
    colorX ccl = colorXList[random(0, 52)];
    LED_R_Brillo_value = ccl.r;
    LED_R_o_Brillo_value = ccl.r;

    LED_G_Brillo_value = ccl.g;
    LED_G_o_Brillo_value = ccl.g;

    LED_B_Brillo_value = ccl.b;
    LED_B_o_Brillo_value= ccl.b;

    LED_W_Brillo_value = random(1, 50);
    LED_Y_Brillo_value = random(1, 10);
   Serial.println("cambio.. "+ ccl.nombre);
  }

  // if (setLedColor()) {
  //   Serial.print("cambio completo.. --> " + CL + "\t");
  //  // delay(3000);


  //   if (LED_R_Current_value == 255) {
  //     Serial.println("LED_G  _Brillo_value..");

  //     LED_R_o_Brillo_value = 0;
  //     LED_G_o_Brillo_value = 255;

  //     LED_R_Brillo_value = 0;
  //     LED_G_Brillo_value = 255;
  //     CL = "Verde";

  //   } else if (LED_G_Current_value == 255) {
  //     Serial.println("LED_B  _Brillo_value..");

  //     LED_G_o_Brillo_value = 0;
  //     LED_B_o_Brillo_value = 255;

  //     LED_G_Brillo_value = 0;
  //     LED_B_Brillo_value = 255;
  //     CL = "Azul";

  //   } else if (LED_B_Current_value == 255) {
  //     Serial.println("LED_Y  _Brillo_value..");

  //     LED_B_o_Brillo_value = 0;
  //     LED_B_Brillo_value = 0;

  //     LED_Y_Brillo_value = 255;
  //     CL = "Amarillo ";

  //   } else if (LED_Y_Current_value == 255) {
  //     Serial.println("LED_W  _Brillo_value..");

  //     LED_W_Brillo_value = 255;
  //     LED_Y_Brillo_value = 0;

  //     CL = "Blanco ";
  //   } else if (LED_W_Current_value == 255) {
  //     Serial.println("LED_R  _Brillo_value..");

  //     LED_R_Brillo_value = 255;
  //     LED_R_o_Brillo_value = 255;
  //     LED_W_Brillo_value = 0;
  //     CL = "Blanco ";
  //   }
  // }

    while (Serial.available() > 0) {
    String myString = Serial.readString();
    String letra = myString.substring(0, 1);

    int textLen = myString.length() - 1;
    String resto = myString.substring(1, textLen);

    Serial.print("letra: ");
    Serial.println(letra);

    if (letra == "R") {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      LED_R_o_Brillo_value = Valor;

    } else if (letra == "G") {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      LED_G_o_Brillo_value = Valor;

    } else if (letra == "B") {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      LED_B_o_Brillo_value = Valor;

    }

    else if (letra == "r") {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      LED_R_Brillo_value = Valor;

    } else if (letra == "g") {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      LED_G_Brillo_value = Valor;

    } else if (letra == "b") {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      LED_B_Brillo_value = Valor;

    } else if (letra == "w") {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      LED_W_Brillo_value = Valor;

    } else if (letra == "y") {
      int Valor = resto.toInt();

      Serial.print("valor: ");
      Serial.println(Valor);
      LED_Y_Brillo_value = Valor;
    }
  }
  //webSocket.loop();
}