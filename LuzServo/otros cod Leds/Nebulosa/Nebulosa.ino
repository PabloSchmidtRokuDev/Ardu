
#define DEBUG_SERIAL Serial

int pin_R = 14;
int pin_G = 12;
int pin_B = 15;//15
int pin_W = 13;//13
int pin_Y = 16;//16


int pinMotor = 5;

int LED_R_value = 0;
int LED_G_value = 0;
int LED_B_value = 0;
int LED_W_value = 0;
int LED_Y_value = 0;



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



#include "luces.hpp"
#include "colores.hpp"

void setup() {
  Serial.begin(115200);


  pinMode(pin_R, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_W, OUTPUT);
  pinMode(pin_Y, OUTPUT);
  pinMode(pinMotor, OUTPUT);

  analogWrite(pinMotor, 150);

  // analogWrite(pin_R, 180);
  // delay(2000);
  //   analogWrite(pin_R, 0);

  // analogWrite(pin_G, 180);
  // delay(2000);
  //   analogWrite(pin_G, 0);

  // analogWrite(pin_B, 180);
  // delay(2000);
  //   analogWrite(pin_B, 0);




  // analogWrite(pin_W, 180);
  // delay(2000);
  //   analogWrite(pin_W, 0);
  // analogWrite(pin_Y, 180);
  // delay(2000);
  //   analogWrite(pin_Y, 0);

  cargarVecctorColores();

  Serial.println("start..");
  DEBUG_SERIAL.begin(115200);
}


void loop() {

  if (setLedColor()) {
    Serial.print("_.");
    delay(3000);
    LED_R_Brillo_value = 0;
    LED_G_Brillo_value = 0;
    LED_B_Brillo_value = 0;
    LED_W_Brillo_value = 0;
    LED_Y_Brillo_value = 0;

    int x = random(1, 5);

    // if (x == 1) LED_W_Brillo_value = 255;
    // if (x == 2) LED_Y_Brillo_value = 255;
    // if (x == 3) LED_R_Brillo_value = 255;
    // if (x == 4) LED_G_Brillo_value = 255;
    // if (x == 5) LED_B_Brillo_value = 255;
    if (random(1, 5) < 2){
          if (x <= 3) LED_W_Brillo_value = 255;
    if (x > 3) LED_Y_Brillo_value = 255;
    }else{
    colorX ccl = colorXList[random(0, 51)];
    LED_R_Brillo_value = ccl.r;
    LED_G_Brillo_value = ccl.g;
    LED_B_Brillo_value = ccl.b;
    LED_W_Brillo_value = random(10, 50);
    }

  }
}
//webSocket.loop();
