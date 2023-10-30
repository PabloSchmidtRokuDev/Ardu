#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(52, 53);  //Seleccionamos los pines 7 como Rx y 8 como Tx

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

#include "pantallas.h"

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define destino_Habitacion "habitacion          "
#define destino_Living "living              "
#define destino_doble_barra "doble_barra         "
#define destino_faros "faros               "


#include <Buzzer.h>

Buzzer buzzer(34, 35);

#include <Servo.h>

#include "colores.hpp"

#define PIN_servo1 24
#define PIN_servo2 26


Servo servo1;
Servo servo2;

int pin_R = 10;
int pin_B = 11;
int pin_G = 12;

bool powerFaro = false;

const int START_BUTTON_PIN = 31;
int lastStartButtonState = LOW;

// *** COPY-PASTE from Serial Terminal:
const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=127,TS_RT=901,TS_TOP=88,TS_BOT=894;

//PORTRAIT  CALIBRATION     240 x 320
// x = map(p.x, LEFT=127, RT=901, 0, 240)
// y = map(p.y, TOP=88, BOT=894, 0, 320)

// //LANDSCAPE CALIBRATION     320 x 240
// x = map(p.y, LEFT=88, RT=894, 0, 320)
// y = map(p.x, TOP=901, BOT=127, 0, 240)


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int pixel_x, pixel_y;  //Touch_getXY() updates global vars
bool Touch_getXY(void) {
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);  //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);  //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());  //.kbv makes sense to me
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;

unsigned long cTiempo1 = 0;
unsigned long cTiempo2 = 0;
unsigned long cTiempoSegundos = 0;

unsigned long sTiempo1 = 0;
unsigned long sTiempo2 = 0;
unsigned long sTiempoSegundos = 0;

void setup() {
  wifiSerial.begin(115200);  // Iniciamos puerto serial virtual
  Serial.begin(115200);      // Iniciamos puerto serial

  pinMode(START_BUTTON_PIN, INPUT_PULLUP);

  servo1.attach(PIN_servo1);
  servo1.write(0);
  servo2.attach(PIN_servo2);
  servo2.write(180);

  pinMode(pin_R, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_G, OUTPUT);

  cargarVecctorColores();


  Serial.println("Iniciado...");

  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  if (ID == 0xD3D3) ID = 0x9486;  // write-only shield
  tft.begin(ID);
  tft.setRotation(1);  //PORTRAIT

  drawMainScreen();

  tiempo1 = millis();
}

colorX currentColor = colorXList[8];
colorX cFrom;

long randomNumber, randomNumber2;
int sa1 = 0;
int sa2 = 180;
int cont = 0;

void loop() {

  if (powerFaro) {
    changeColor();
    moverServos();
  } else {
    analogWrite(pin_R, 0);
    analogWrite(pin_B, 0);
    analogWrite(pin_G, 0);
  }



  // int startButtonState = digitalRead(START_BUTTON_PIN);
  // if (lastStartButtonState == HIGH && startButtonState == LOW) {
  //   llamar();
  // }

  // lastStartButtonState = startButtonState;

  comunicadatos();

  if (currentScreen == "main" || currentScreen == "") {
    checkBtnMain();
  } else if (currentScreen == "habitacion") {
    checkBtnHabitacion();
  } else if (currentScreen == "living") {
    checkBtnLiving();
  } else if (currentScreen == "barras") {
    checkBtnBarras();
  } else if (currentScreen == "faros") {
    checkBtnFaros();
  } else if (currentScreen == "color") {
    checkBtnColor();
  }
}

int cR, cG, cB;
void changeColor() {
  tiempo2 = millis();
  if (tiempo2 > (tiempo1 + 5000)) {  //Si ha pasado 1 segundo ejecuta el IF
    tiempo1 = millis();
    colorX cl = colorXList[random(0, 150)];
    cFrom = currentColor;
    currentColor = cl;
     cR = cFrom.r;
   cG = cFrom.g;
   cB = cFrom.b;
  }

  cTiempo2 = millis();
  if (cTiempo2 > cTiempo1 + 10 && (cR != currentColor.r || cG != currentColor.g || cB != currentColor.b)) {
    cTiempo1 = millis();

    if (cR != currentColor.r) {
      if (cR < currentColor.r) cR++;
      else cR--;
    }

    if (cG != currentColor.g) {
      if (cG < currentColor.g) cG++;
      else cG--;
    }

    if (cB != currentColor.b) {
      if (cB < currentColor.b) cB++;
      else cB--;
    }

    analogWrite(pin_R, cR);
    analogWrite(pin_B, cB);
    analogWrite(pin_G, cG);
  }
}

void moverServos() {

  sTiempo2 = millis();
  if (sTiempo2 > sTiempo1 + 40) {
    sTiempo1 = millis();

    if (sa1 == randomNumber && sa2 == randomNumber2) {
      randomNumber = random(1, 90);
      randomNumber2 = random(120, 180);
    } else {

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
    }
  }
}

void llamar() {
  buzzer.begin(10);

  buzzer.sound(NOTE_A3, 500);
  buzzer.sound(NOTE_A3, 500);
  buzzer.sound(NOTE_A3, 500);
  buzzer.sound(NOTE_F3, 375);
  buzzer.sound(NOTE_C4, 125);

  buzzer.sound(NOTE_A3, 500);
  buzzer.sound(NOTE_F3, 375);
  buzzer.sound(NOTE_C4, 125);
  buzzer.sound(NOTE_A3, 1000);





  // buzzer.sound(NOTE_E4, 500);
  // buzzer.sound(NOTE_E4, 500);
  // buzzer.sound(NOTE_E4, 500);
  // buzzer.sound(NOTE_F4, 375);
  // buzzer.sound(NOTE_C4, 125);

  // buzzer.sound(NOTE_GS3, 500);
  // buzzer.sound(NOTE_F3, 375);
  // buzzer.sound(NOTE_C4, 125);
  // buzzer.sound(NOTE_A3, 1000);

  // buzzer.sound(NOTE_A4, 500);
  // buzzer.sound(NOTE_A3, 375);
  // buzzer.sound(NOTE_A3, 125);
  // buzzer.sound(NOTE_A4, 500);
  // buzzer.sound(NOTE_GS4, 375);
  // buzzer.sound(NOTE_G4, 125);

  // buzzer.sound(NOTE_FS4, 125);
  // buzzer.sound(NOTE_E4, 125);
  // buzzer.sound(NOTE_F4, 250);
  // buzzer.sound(0, 250);
  // buzzer.sound(NOTE_AS3, 250);
  // buzzer.sound(NOTE_DS4, 500);
  // buzzer.sound(NOTE_D4, 375);
  // buzzer.sound(NOTE_CS4, 125);

  // buzzer.sound(NOTE_C4, 125);
  // buzzer.sound(NOTE_B3, 125);
  // buzzer.sound(NOTE_C4, 250);
  // buzzer.sound(0, 250);
  // buzzer.sound(NOTE_F3, 250);
  // buzzer.sound(NOTE_GS3, 500);
  // buzzer.sound(NOTE_F3, 375);
  // buzzer.sound(NOTE_A3, 125);

  // buzzer.sound(NOTE_C4, 500);
  // buzzer.sound(NOTE_A3, 375);
  // buzzer.sound(NOTE_C4, 125);
  // buzzer.sound(NOTE_E4, 1000);

  // buzzer.sound(NOTE_A4, 500);
  // buzzer.sound(NOTE_A3, 375);
  // buzzer.sound(NOTE_A3, 125);
  // buzzer.sound(NOTE_A4, 500);
  // buzzer.sound(NOTE_GS4, 375);
  // buzzer.sound(NOTE_G4, 125);

  // buzzer.sound(NOTE_FS4, 125);
  // buzzer.sound(NOTE_E4, 125);
  // buzzer.sound(NOTE_F4, 250);
  // buzzer.sound(0, 250);
  // buzzer.sound(NOTE_AS3, 250);
  // buzzer.sound(NOTE_DS4, 500);
  // buzzer.sound(NOTE_D4, 375);
  // buzzer.sound(NOTE_CS4, 125);

  // buzzer.sound(NOTE_C4, 125);
  // buzzer.sound(NOTE_B3, 125);
  // buzzer.sound(NOTE_C4, 250);
  // buzzer.sound(0, 250);
  // buzzer.sound(NOTE_F3, 250);
  // buzzer.sound(NOTE_GS3, 500);
  // buzzer.sound(NOTE_F3, 375);
  // buzzer.sound(NOTE_C4, 125);

  // buzzer.sound(NOTE_A3, 500);
  // buzzer.sound(NOTE_F3, 375);
  // buzzer.sound(NOTE_C4, 125);
  // buzzer.sound(NOTE_A3, 1000);

  buzzer.end(2000);
}

void checkBtnMain() {

  bool down = Touch_getXY();

  // btn_PowerAll.press(down && btn_PowerAll.contains(pixel_x, pixel_y));
  // btn_remotePower.press(down && btn_remotePower.contains(pixel_x, pixel_y));
  btn_Habitacion.press(down && btn_Habitacion.contains(pixel_x, pixel_y));
  btn_Living.press(down && btn_Living.contains(pixel_x, pixel_y));
  btn_Faros.press(down && btn_Faros.contains(pixel_x, pixel_y));
  btn_Barras.press(down && btn_Barras.contains(pixel_x, pixel_y));



  if (btn_PowerAll.justReleased()) { btn_PowerAll.drawButton(false); }
  if (btn_remotePower.justReleased()) { btn_remotePower.drawButton(false); }
  if (btn_Habitacion.justReleased()) { btn_Habitacion.drawButton(false); }
  if (btn_Living.justReleased()) { btn_Living.drawButton(false); }
  if (btn_Faros.justReleased()) { btn_Faros.drawButton(false); }
  if (btn_Barras.justReleased()) { btn_Barras.drawButton(false); }

  // if (btn_PowerAll.justPressed()) {  executeFunction(destino_Habitacion, "HEX", "BF40FF00"); btn_PowerAll.drawButton(true); delay(500);  }
  // if (btn_remotePower.justPressed()) {  btn_remotePower.drawButton(true);  delay(500); executeFunction(destino_Habitacion, "HEX", "AB54FF00"); }
  if (btn_Habitacion.justPressed()) {
    btn_Habitacion.drawButton(true);
    delay(500);
    drawHabitacionScreen();
  }
  if (btn_Living.justPressed()) {
    btn_Living.drawButton(true);
    delay(500);
    drawLivingScreen();
  }
  if (btn_Faros.justPressed()) {
    btn_Faros.drawButton(true);
    delay(500);
    powerFaro = !powerFaro;
    //drawFarosScreen();
  }
  if (btn_Barras.justPressed()) {
    btn_Barras.drawButton(true);
    delay(500);
    drawBarrasScreen();
  }
}

void checkBtnHabitacion() {

  bool down = Touch_getXY();

  btn_power.press(down && btn_power.contains(pixel_x, pixel_y));
  btn_colores.press(down && btn_colores.contains(pixel_x, pixel_y));

  btn_Brillo100.press(down && btn_Brillo100.contains(pixel_x, pixel_y));
  btn_Brillo70.press(down && btn_Brillo70.contains(pixel_x, pixel_y));
  btn_Brillo40.press(down && btn_Brillo40.contains(pixel_x, pixel_y));
  btn_Brillo15.press(down && btn_Brillo15.contains(pixel_x, pixel_y));

  btn_SelectEfecto1.press(down && btn_SelectEfecto1.contains(pixel_x, pixel_y));
  btn_SelectEfecto2.press(down && btn_SelectEfecto2.contains(pixel_x, pixel_y));
  btn_SelectEfecto3.press(down && btn_SelectEfecto3.contains(pixel_x, pixel_y));
  btn_SelectEfecto4.press(down && btn_SelectEfecto4.contains(pixel_x, pixel_y));
  btn_SelectEfecto5.press(down && btn_SelectEfecto5.contains(pixel_x, pixel_y));
  btn_SelectEfecto6.press(down && btn_SelectEfecto6.contains(pixel_x, pixel_y));

  btn_Bola.press(down && btn_Bola.contains(pixel_x, pixel_y));
  btn_UV.press(down && btn_UV.contains(pixel_x, pixel_y));
  btn_Audio.press(down && btn_Audio.contains(pixel_x, pixel_y));

  btn_volver.press(down && btn_volver.contains(pixel_x, pixel_y));


  if (btn_power.justReleased()) { btn_power.drawButton(false); }
  if (btn_colores.justReleased()) { btn_colores.drawButton(false); }

  if (btn_Brillo100.justReleased()) { btn_Brillo100.drawButton(false); }
  if (btn_Brillo70.justReleased()) { btn_Brillo70.drawButton(false); }
  if (btn_Brillo40.justReleased()) { btn_Brillo40.drawButton(false); }
  if (btn_Brillo15.justReleased()) { btn_Brillo15.drawButton(false); }

  if (btn_SelectEfecto1.justReleased()) { btn_SelectEfecto1.drawButton(false); }
  if (btn_SelectEfecto2.justReleased()) { btn_SelectEfecto2.drawButton(false); }
  if (btn_SelectEfecto3.justReleased()) { btn_SelectEfecto3.drawButton(false); }
  if (btn_SelectEfecto4.justReleased()) { btn_SelectEfecto4.drawButton(false); }
  if (btn_SelectEfecto5.justReleased()) { btn_SelectEfecto5.drawButton(false); }
  if (btn_SelectEfecto6.justReleased()) { btn_SelectEfecto6.drawButton(false); }

  if (btn_volver.justReleased()) { btn_volver.drawButton(false); }

  if (btn_power.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "BF40FF00");
    btn_power.drawButton(true);
    delay(500);
  }
  if (btn_colores.justPressed()) {
    btn_colores.drawButton(true);
    delay(500);
    drawColoresScreen("habitacion", destino_Habitacion);
  }

  if (btn_Brillo100.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "E41BFF00");
    btn_Brillo100.drawButton(true);
    delay(500);
  }
  if (btn_Brillo70.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "E01FFF00");
    btn_Brillo70.drawButton(true);
    delay(500);
  }
  if (btn_Brillo40.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "B34CFF00");
    btn_Brillo40.drawButton(true);
    delay(500);
  }
  if (btn_Brillo15.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "B748FF00");
    btn_Brillo15.drawButton(true);
    delay(500);
  }

  if (btn_SelectEfecto1.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "F20DFF00");
    btn_SelectEfecto1.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto2.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "F10EFF00");
    btn_SelectEfecto2.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto3.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "F00FFF00");
    btn_SelectEfecto3.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto4.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "F708FF00");
    btn_SelectEfecto4.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto5.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "F609FF00");
    btn_SelectEfecto5.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto6.justPressed()) {
    executeFunction(destino_Habitacion, "HEX", "F50AFF00");
    btn_SelectEfecto6.drawButton(true);
    delay(500);
  }

  if (btn_volver.justPressed()) {
    btn_volver.drawButton(true);
    delay(500);
    volverPantalla();
  }
}

int audioSelected = 1;

void checkBtnLiving() {

  bool down = Touch_getXY();

  btn_power.press(down && btn_power.contains(pixel_x, pixel_y));
  btn_colores.press(down && btn_colores.contains(pixel_x, pixel_y));

  btn_Brillo100.press(down && btn_Brillo100.contains(pixel_x, pixel_y));
  btn_Brillo70.press(down && btn_Brillo70.contains(pixel_x, pixel_y));
  btn_Brillo40.press(down && btn_Brillo40.contains(pixel_x, pixel_y));
  btn_Brillo15.press(down && btn_Brillo15.contains(pixel_x, pixel_y));

  btn_SelectEfecto1.press(down && btn_SelectEfecto1.contains(pixel_x, pixel_y));
  btn_SelectEfecto2.press(down && btn_SelectEfecto2.contains(pixel_x, pixel_y));
  btn_SelectEfecto3.press(down && btn_SelectEfecto3.contains(pixel_x, pixel_y));
  btn_SelectEfecto4.press(down && btn_SelectEfecto4.contains(pixel_x, pixel_y));
  btn_SelectEfecto5.press(down && btn_SelectEfecto5.contains(pixel_x, pixel_y));
  btn_SelectEfecto6.press(down && btn_SelectEfecto6.contains(pixel_x, pixel_y));

  btn_Bola.press(down && btn_Bola.contains(pixel_x, pixel_y));
  btn_UV.press(down && btn_UV.contains(pixel_x, pixel_y));
  btn_Audio.press(down && btn_Audio.contains(pixel_x, pixel_y));
  btn_volver.press(down && btn_volver.contains(pixel_x, pixel_y));


  if (btn_power.justReleased()) { btn_power.drawButton(false); }
  if (btn_colores.justReleased()) { btn_colores.drawButton(false); }

  if (btn_Brillo100.justReleased()) { btn_Brillo100.drawButton(false); }
  if (btn_Brillo70.justReleased()) { btn_Brillo70.drawButton(false); }
  if (btn_Brillo40.justReleased()) { btn_Brillo40.drawButton(false); }
  if (btn_Brillo15.justReleased()) { btn_Brillo15.drawButton(false); }

  if (btn_SelectEfecto1.justReleased()) { btn_SelectEfecto1.drawButton(false); }
  if (btn_SelectEfecto2.justReleased()) { btn_SelectEfecto2.drawButton(false); }
  if (btn_SelectEfecto3.justReleased()) { btn_SelectEfecto3.drawButton(false); }
  if (btn_SelectEfecto4.justReleased()) { btn_SelectEfecto4.drawButton(false); }
  if (btn_SelectEfecto5.justReleased()) { btn_SelectEfecto5.drawButton(false); }
  if (btn_SelectEfecto6.justReleased()) { btn_SelectEfecto6.drawButton(false); }

  if (btn_Bola.justReleased()) { btn_Bola.drawButton(false); }
  if (btn_UV.justReleased()) { btn_UV.drawButton(false); }
  if (btn_Audio.justReleased()) { btn_Audio.drawButton(false); }

  if (btn_volver.justReleased()) { btn_volver.drawButton(false); }

  if (btn_power.justPressed()) {
    executeFunction(destino_Living, "HEX", "BF40FF00");
    btn_power.drawButton(true);
    delay(500);
  }
  if (btn_colores.justPressed()) {
    btn_colores.drawButton(true);
    delay(500);
    drawColoresScreen("living", destino_Living);
  }

  if (btn_Brillo100.justPressed()) {
    executeFunction(destino_Living, "HEX", "E41BFF00");
    btn_Brillo100.drawButton(true);
    delay(500);
  }
  if (btn_Brillo70.justPressed()) {
    executeFunction(destino_Living, "HEX", "E01FFF00");
    btn_Brillo70.drawButton(true);
    delay(500);
  }
  if (btn_Brillo40.justPressed()) {
    executeFunction(destino_Living, "HEX", "B34CFF00");
    btn_Brillo40.drawButton(true);
    delay(500);
  }
  if (btn_Brillo15.justPressed()) {
    executeFunction(destino_Living, "HEX", "B748FF00");
    btn_Brillo15.drawButton(true);
    delay(500);
  }

  if (btn_SelectEfecto1.justPressed()) {
    executeFunction(destino_Living, "HEX", "F20DFF00");
    btn_SelectEfecto1.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto2.justPressed()) {
    executeFunction(destino_Living, "HEX", "F10EFF00");
    btn_SelectEfecto2.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto3.justPressed()) {
    executeFunction(destino_Living, "HEX", "F00FFF00");
    btn_SelectEfecto3.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto4.justPressed()) {
    executeFunction(destino_Living, "HEX", "F708FF00");
    btn_SelectEfecto4.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto5.justPressed()) {
    executeFunction(destino_Living, "HEX", "F609FF00");
    btn_SelectEfecto5.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto6.justPressed()) {
    executeFunction(destino_Living, "HEX", "F50AFF00");
    btn_SelectEfecto6.drawButton(true);
    delay(500);
  }

  if (btn_Bola.justPressed()) {
    executeFunction(destino_Living, "HEX", "BE41FF00");
    btn_Bola.drawButton(true);
    delay(500);
  }
  if (btn_UV.justPressed()) {
    executeFunction(destino_Living, "HEX", "A25DFF00");
    btn_UV.drawButton(true);
    delay(500);
  }

  if (btn_Audio.justPressed()) {
    if (audioSelected == 1) {
      Serial.println("Envia audio 1");
      executeFunction(destino_Living, "HEX", "EF10FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }
    if (audioSelected == 2) {
      Serial.println("Envia audio 2");
      executeFunction(destino_Living, "HEX", "EE11FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }
    if (audioSelected == 3) {
      Serial.println("Envia audio 3");
      executeFunction(destino_Living, "HEX", "ED12FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }
    if (audioSelected == 4) {
      Serial.println("Envia audio 4");
      executeFunction(destino_Living, "HEX", "EC13FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }

    audioSelected++;
    if (audioSelected == 5) audioSelected = 1;
  }

  if (btn_volver.justPressed()) {
    btn_volver.drawButton(true);
    delay(500);
    volverPantalla();
  }
}

void checkBtnBarras() {

  bool down = Touch_getXY();

  btn_power.press(down && btn_power.contains(pixel_x, pixel_y));
  btn_colores.press(down && btn_colores.contains(pixel_x, pixel_y));

  btn_Brillo100.press(down && btn_Brillo100.contains(pixel_x, pixel_y));
  btn_Brillo70.press(down && btn_Brillo70.contains(pixel_x, pixel_y));
  btn_Brillo40.press(down && btn_Brillo40.contains(pixel_x, pixel_y));
  btn_Brillo15.press(down && btn_Brillo15.contains(pixel_x, pixel_y));

  btn_SelectEfecto1.press(down && btn_SelectEfecto1.contains(pixel_x, pixel_y));
  btn_SelectEfecto2.press(down && btn_SelectEfecto2.contains(pixel_x, pixel_y));
  btn_SelectEfecto3.press(down && btn_SelectEfecto3.contains(pixel_x, pixel_y));
  btn_SelectEfecto4.press(down && btn_SelectEfecto4.contains(pixel_x, pixel_y));
  btn_SelectEfecto5.press(down && btn_SelectEfecto5.contains(pixel_x, pixel_y));
  btn_SelectEfecto6.press(down && btn_SelectEfecto6.contains(pixel_x, pixel_y));

  // btn_Bola.press(down && btn_Bola.contains(pixel_x, pixel_y));
  // btn_UV.press(down && btn_UV.contains(pixel_x, pixel_y));
  // btn_Audio.press(down && btn_Audio.contains(pixel_x, pixel_y));
  btn_volver.press(down && btn_volver.contains(pixel_x, pixel_y));


  if (btn_power.justReleased()) { btn_power.drawButton(false); }
  if (btn_colores.justReleased()) { btn_colores.drawButton(false); }

  if (btn_Brillo100.justReleased()) { btn_Brillo100.drawButton(false); }
  if (btn_Brillo70.justReleased()) { btn_Brillo70.drawButton(false); }
  if (btn_Brillo40.justReleased()) { btn_Brillo40.drawButton(false); }
  if (btn_Brillo15.justReleased()) { btn_Brillo15.drawButton(false); }

  if (btn_SelectEfecto1.justReleased()) { btn_SelectEfecto1.drawButton(false); }
  if (btn_SelectEfecto2.justReleased()) { btn_SelectEfecto2.drawButton(false); }
  if (btn_SelectEfecto3.justReleased()) { btn_SelectEfecto3.drawButton(false); }
  if (btn_SelectEfecto4.justReleased()) { btn_SelectEfecto4.drawButton(false); }
  if (btn_SelectEfecto5.justReleased()) { btn_SelectEfecto5.drawButton(false); }
  if (btn_SelectEfecto6.justReleased()) { btn_SelectEfecto6.drawButton(false); }

  // if (btn_Bola.justReleased()) { btn_Bola.drawButton(false); }
  // if (btn_UV.justReleased()) { btn_UV.drawButton(false); }
  // if (btn_Audio.justReleased()) { btn_Audio.drawButton(false); }

  if (btn_volver.justReleased()) { btn_volver.drawButton(false); }

  if (btn_power.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "BF40FF00");
    btn_power.drawButton(true);
    delay(500);
  }
  if (btn_colores.justPressed()) {
    btn_colores.drawButton(true);
    delay(500);
    drawColoresScreen("barras", destino_doble_barra);
  }

  if (btn_Brillo100.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "E41BFF00");
    btn_Brillo100.drawButton(true);
    delay(500);
  }
  if (btn_Brillo70.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "E01FFF00");
    btn_Brillo70.drawButton(true);
    delay(500);
  }
  if (btn_Brillo40.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "B34CFF00");
    btn_Brillo40.drawButton(true);
    delay(500);
  }
  if (btn_Brillo15.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "B748FF00");
    btn_Brillo15.drawButton(true);
    delay(500);
  }

  if (btn_SelectEfecto1.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "F20DFF00");
    btn_SelectEfecto1.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto2.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "F10EFF00");
    btn_SelectEfecto2.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto3.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "F00FFF00");
    btn_SelectEfecto3.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto4.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "F708FF00");
    btn_SelectEfecto4.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto5.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "F609FF00");
    btn_SelectEfecto5.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto6.justPressed()) {
    executeFunction(destino_doble_barra, "HEX", "F50AFF00");
    btn_SelectEfecto6.drawButton(true);
    delay(500);
  }

  // if (btn_Bola.justPressed()) {
  //   executeFunction(destino_doble_barra, "HEX", "BE41FF00");
  //   btn_Bola.drawButton(true);
  //   delay(500);
  // }
  // if (btn_UV.justPressed()) {
  //   executeFunction(destino_doble_barra, "HEX", "A25DFF00");
  //   btn_UV.drawButton(true);
  //   delay(500);
  // }

  // if (btn_Audio.justPressed()) {
  //   if (audioSelected == 1) {
  //     Serial.println("Envia audio 1");
  //     executeFunction(destino_doble_barra, "HEX", "EF10FF00");
  //     btn_Audio.drawButton(true);
  //     delay(500);
  //   }
  //   if (audioSelected == 2) {
  //     Serial.println("Envia audio 2");
  //     executeFunction(destino_doble_barra, "HEX", "EE11FF00");
  //     btn_Audio.drawButton(true);
  //     delay(500);
  //   }
  //   if (audioSelected == 3) {
  //     Serial.println("Envia audio 3");
  //     executeFunction(destino_doble_barra, "HEX", "ED12FF00");
  //     btn_Audio.drawButton(true);
  //     delay(500);
  //   }
  //   if (audioSelected == 4) {
  //     Serial.println("Envia audio 4");
  //     executeFunction(destino_doble_barra, "HEX", "EC13FF00");
  //     btn_Audio.drawButton(true);
  //     delay(500);
  //   }

  //   audioSelected++;
  //   if (audioSelected == 5) audioSelected = 1;
  // }

  if (btn_volver.justPressed()) {
    btn_volver.drawButton(true);
    delay(500);
    volverPantalla();
  }
}
void checkBtnFaros() {

  bool down = Touch_getXY();

  btn_power.press(down && btn_power.contains(pixel_x, pixel_y));
  btn_colores.press(down && btn_colores.contains(pixel_x, pixel_y));

  btn_Brillo100.press(down && btn_Brillo100.contains(pixel_x, pixel_y));
  btn_Brillo70.press(down && btn_Brillo70.contains(pixel_x, pixel_y));
  btn_Brillo40.press(down && btn_Brillo40.contains(pixel_x, pixel_y));
  btn_Brillo15.press(down && btn_Brillo15.contains(pixel_x, pixel_y));

  btn_SelectEfecto1.press(down && btn_SelectEfecto1.contains(pixel_x, pixel_y));
  btn_SelectEfecto2.press(down && btn_SelectEfecto2.contains(pixel_x, pixel_y));
  btn_SelectEfecto3.press(down && btn_SelectEfecto3.contains(pixel_x, pixel_y));
  btn_SelectEfecto4.press(down && btn_SelectEfecto4.contains(pixel_x, pixel_y));
  btn_SelectEfecto5.press(down && btn_SelectEfecto5.contains(pixel_x, pixel_y));
  btn_SelectEfecto6.press(down && btn_SelectEfecto6.contains(pixel_x, pixel_y));

  btn_Bola.press(down && btn_Bola.contains(pixel_x, pixel_y));
  btn_UV.press(down && btn_UV.contains(pixel_x, pixel_y));
  btn_Audio.press(down && btn_Audio.contains(pixel_x, pixel_y));
  btn_volver.press(down && btn_volver.contains(pixel_x, pixel_y));


  if (btn_power.justReleased()) { btn_power.drawButton(false); }
  if (btn_colores.justReleased()) { btn_colores.drawButton(false); }

  if (btn_Brillo100.justReleased()) { btn_Brillo100.drawButton(false); }
  if (btn_Brillo70.justReleased()) { btn_Brillo70.drawButton(false); }
  if (btn_Brillo40.justReleased()) { btn_Brillo40.drawButton(false); }
  if (btn_Brillo15.justReleased()) { btn_Brillo15.drawButton(false); }

  if (btn_SelectEfecto1.justReleased()) { btn_SelectEfecto1.drawButton(false); }
  if (btn_SelectEfecto2.justReleased()) { btn_SelectEfecto2.drawButton(false); }
  if (btn_SelectEfecto3.justReleased()) { btn_SelectEfecto3.drawButton(false); }
  if (btn_SelectEfecto4.justReleased()) { btn_SelectEfecto4.drawButton(false); }
  if (btn_SelectEfecto5.justReleased()) { btn_SelectEfecto5.drawButton(false); }
  if (btn_SelectEfecto6.justReleased()) { btn_SelectEfecto6.drawButton(false); }

  if (btn_Bola.justReleased()) { btn_Bola.drawButton(false); }
  if (btn_UV.justReleased()) { btn_UV.drawButton(false); }
  if (btn_Audio.justReleased()) { btn_Audio.drawButton(false); }

  if (btn_volver.justReleased()) { btn_volver.drawButton(false); }

  if (btn_power.justPressed()) {
    executeFunction(destino_faros, "HEX", "BF40FF00");
    btn_power.drawButton(true);
    delay(500);
  }
  if (btn_colores.justPressed()) {
    btn_colores.drawButton(true);
    delay(500);
    drawColoresScreen("faros", destino_faros);
  }

  if (btn_Brillo100.justPressed()) {
    executeFunction(destino_faros, "HEX", "E41BFF00");
    btn_Brillo100.drawButton(true);
    delay(500);
  }
  if (btn_Brillo70.justPressed()) {
    executeFunction(destino_faros, "HEX", "E01FFF00");
    btn_Brillo70.drawButton(true);
    delay(500);
  }
  if (btn_Brillo40.justPressed()) {
    executeFunction(destino_faros, "HEX", "B34CFF00");
    btn_Brillo40.drawButton(true);
    delay(500);
  }
  if (btn_Brillo15.justPressed()) {
    executeFunction(destino_faros, "HEX", "B748FF00");
    btn_Brillo15.drawButton(true);
    delay(500);
  }

  if (btn_SelectEfecto1.justPressed()) {
    executeFunction(destino_faros, "HEX", "F20DFF00");
    btn_SelectEfecto1.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto2.justPressed()) {
    executeFunction(destino_faros, "HEX", "F10EFF00");
    btn_SelectEfecto2.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto3.justPressed()) {
    executeFunction(destino_faros, "HEX", "F00FFF00");
    btn_SelectEfecto3.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto4.justPressed()) {
    executeFunction(destino_faros, "HEX", "F708FF00");
    btn_SelectEfecto4.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto5.justPressed()) {
    executeFunction(destino_faros, "HEX", "F609FF00");
    btn_SelectEfecto5.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto6.justPressed()) {
    executeFunction(destino_faros, "HEX", "F50AFF00");
    btn_SelectEfecto6.drawButton(true);
    delay(500);
  }

  if (btn_Bola.justPressed()) {
    executeFunction(destino_faros, "HEX", "BE41FF00");
    btn_Bola.drawButton(true);
    delay(500);
  }
  if (btn_UV.justPressed()) {
    executeFunction(destino_faros, "HEX", "A25DFF00");
    btn_UV.drawButton(true);
    delay(500);
  }

  if (btn_Audio.justPressed()) {
    if (audioSelected == 1) {
      Serial.println("Envia audio 1");
      executeFunction(destino_faros, "HEX", "EF10FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }
    if (audioSelected == 2) {
      Serial.println("Envia audio 2");
      executeFunction(destino_faros, "HEX", "EE11FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }
    if (audioSelected == 3) {
      Serial.println("Envia audio 3");
      executeFunction(destino_faros, "HEX", "ED12FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }
    if (audioSelected == 4) {
      Serial.println("Envia audio 4");
      executeFunction(destino_faros, "HEX", "EC13FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }

    audioSelected++;
    if (audioSelected == 5) audioSelected = 1;
  }

  if (btn_volver.justPressed()) {
    btn_volver.drawButton(true);
    delay(500);
    volverPantalla();
  }
}

void checkBtnColor() {

  bool down = Touch_getXY();

  btn_Red.press(down && btn_Red.contains(pixel_x, pixel_y));
  btn_Naranja_fuerte.press(down && btn_Naranja_fuerte.contains(pixel_x, pixel_y));
  btn_Naranja_medio.press(down && btn_Naranja_medio.contains(pixel_x, pixel_y));
  btn_Naranja_debil.press(down && btn_Naranja_debil.contains(pixel_x, pixel_y));
  btn_Amarillo.press(down && btn_Amarillo.contains(pixel_x, pixel_y));
  btn_Green.press(down && btn_Green.contains(pixel_x, pixel_y));
  btn_Verde_claro.press(down && btn_Verde_claro.contains(pixel_x, pixel_y));
  btn_Celeste.press(down && btn_Celeste.contains(pixel_x, pixel_y));
  btn_Navy.press(down && btn_Navy.contains(pixel_x, pixel_y));
  btn_Verde_oscuro.press(down && btn_Verde_oscuro.contains(pixel_x, pixel_y));
  btn_Blue.press(down && btn_Blue.contains(pixel_x, pixel_y));
  btn_Azul_claro.press(down && btn_Azul_claro.contains(pixel_x, pixel_y));
  btn_Marron.press(down && btn_Marron.contains(pixel_x, pixel_y));
  btn_Violeta.press(down && btn_Violeta.contains(pixel_x, pixel_y));
  btn_Rosado.press(down && btn_Rosado.contains(pixel_x, pixel_y));
  btn_White.press(down && btn_White.contains(pixel_x, pixel_y));
  btn_volver.press(down && btn_volver.contains(pixel_x, pixel_y));

  if (btn_Red.justReleased()) { btn_Red.drawButton(false); }
  if (btn_Naranja_fuerte.justReleased()) { btn_Naranja_fuerte.drawButton(false); }
  if (btn_Naranja_medio.justReleased()) { btn_Naranja_medio.drawButton(false); }
  if (btn_Naranja_debil.justReleased()) { btn_Naranja_debil.drawButton(false); }
  if (btn_Amarillo.justReleased()) { btn_Amarillo.drawButton(false); }
  if (btn_Green.justReleased()) { btn_Green.drawButton(false); }
  if (btn_Verde_claro.justReleased()) { btn_Verde_claro.drawButton(false); }
  if (btn_Celeste.justReleased()) { btn_Celeste.drawButton(false); }
  if (btn_Navy.justReleased()) { btn_Navy.drawButton(false); }
  if (btn_Verde_oscuro.justReleased()) { btn_Verde_oscuro.drawButton(false); }
  if (btn_Blue.justReleased()) { btn_Blue.drawButton(false); }
  if (btn_Azul_claro.justReleased()) { btn_Azul_claro.drawButton(false); }
  if (btn_Marron.justReleased()) { btn_Marron.drawButton(false); }
  if (btn_Violeta.justReleased()) { btn_Violeta.drawButton(false); }
  if (btn_Rosado.justReleased()) { btn_Rosado.drawButton(false); }
  if (btn_White.justReleased()) { btn_White.drawButton(false); }
  if (btn_volver.justReleased()) { btn_volver.drawButton(false); }

  if (btn_Red.justPressed()) {
    executeFunction(destinoComando, "HEX", "A758FF00");
    btn_Red.drawButton(true);
    delay(500);
  }
  if (btn_Naranja_fuerte.justPressed()) {
    executeFunction(destinoComando, "HEX", "AB54FF00");
    btn_Naranja_fuerte.drawButton(true);
    delay(500);
  }
  if (btn_Naranja_medio.justPressed()) {
    executeFunction(destinoComando, "HEX", "AF50FF00");
    btn_Naranja_medio.drawButton(true);
    delay(500);
  }
  if (btn_Naranja_debil.justPressed()) {
    executeFunction(destinoComando, "HEX", "E31CFF00");
    btn_Naranja_debil.drawButton(true);
    delay(500);
  }
  if (btn_Amarillo.justPressed()) {
    executeFunction(destinoComando, "HEX", "E718FF00");
    btn_Amarillo.drawButton(true);
    delay(500);
  }

  if (btn_Green.justPressed()) {
    executeFunction(destinoComando, "HEX", "A659FF00");
    btn_Green.drawButton(true);
    delay(500);
  }
  if (btn_Verde_claro.justPressed()) {
    executeFunction(destinoComando, "HEX", "AA55FF00");
    btn_Verde_claro.drawButton(true);
    delay(500);
  }
  if (btn_Celeste.justPressed()) {
    executeFunction(destinoComando, "HEX", "AE51FF00");
    btn_Celeste.drawButton(true);
    delay(500);
  }
  if (btn_Navy.justPressed()) {
    executeFunction(destinoComando, "HEX", "E21DFF00");
    btn_Navy.drawButton(true);
    delay(500);
  }
  if (btn_Verde_oscuro.justPressed()) {
    executeFunction(destinoComando, "HEX", "E619FF00");
    btn_Verde_oscuro.drawButton(true);
    delay(500);
  }

  if (btn_Blue.justPressed()) {
    executeFunction(destinoComando, "HEX", "BA45FF00");
    btn_Blue.drawButton(true);
    delay(500);
  }
  if (btn_Azul_claro.justPressed()) {
    executeFunction(destinoComando, "HEX", "B649FF00");
    btn_Azul_claro.drawButton(true);
    delay(500);
  }
  if (btn_Marron.justPressed()) {
    executeFunction(destinoComando, "HEX", "B24DFF00");
    btn_Marron.drawButton(true);
    delay(500);
  }
  if (btn_Violeta.justPressed()) {
    executeFunction(destinoComando, "HEX", "E11EFF00");
    btn_Violeta.drawButton(true);
    delay(500);
  }
  if (btn_Rosado.justPressed()) {
    executeFunction(destinoComando, "HEX", "E51AFF00");
    btn_Rosado.drawButton(true);
    delay(500);
  }

  if (btn_White.justPressed()) {
    executeFunction(destinoComando, "HEX", "BB44FF00");
    btn_White.drawButton(true);
    delay(500);
  }

  if (btn_volver.justPressed()) {
    btn_volver.drawButton(true);
    delay(500);
    volverPantalla();
  }
}

void volverPantalla() {

  // if (currentScreen == " } else
  Serial.println("volver: " + currentScreen);

  if (currentScreen == "color" && previosScreen == "habitacion") {
    drawHabitacionScreen();

  } else if (currentScreen == "color" && previosScreen == "living") {
    drawLivingScreen();

  } else if (currentScreen == "color" && previosScreen == "barras") {
    drawBarrasScreen();

  } else if (currentScreen == "color" && previosScreen == "faros") {
    drawFarosScreen();

  } else {
    drawMainScreen();
  }
}

void executeFunction(String destino, String preCodigo, String code) {
  String dataX = destino + preCodigo + code;
  Serial.println("send to wifi: " + dataX);
  wifiSerial.println(dataX);
}

String wifiSerialData;
String SerialData;

void comunicadatos() {
  // Envíamos y recibimos datos
  if (Serial.available() > 0) {
    char c = Serial.read();
    SerialData += c;

    if (c == '\n') {
      wifiSerial.print(SerialData);
      SerialData = "";
    }
  }

  while (wifiSerial.available() > 0) {
    char c = wifiSerial.read();
    wifiSerialData += c;

    if (c == '\n') {
      Serial.print(wifiSerialData);
      wifiSerialData = "";
    }
  }
}