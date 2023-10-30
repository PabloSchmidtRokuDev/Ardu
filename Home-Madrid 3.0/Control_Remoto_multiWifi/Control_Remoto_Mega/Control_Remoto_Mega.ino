#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(52, 53);  //Seleccionamos los pines 7 como Rx y 8 como Tx

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>


#include <FreeDefaultFonts.h>

#include <ezBuzzer.h>  // ezBuzzer library

ezBuzzer buzzer(33);  // create ezBuzzer object that attach to a pin;

#define RAW_BUFFER_LENGTH 180

#define MARK_EXCESS_MICROS 20  // Adapt it to your IR receiver module. 20 is recommended for the cheap VS1838 modules.

#include <IRremote.hpp>

int melodya1[] = { NOTE_E5, NOTE_E5, NOTE_E5 };
int melodya2[] = { NOTE_E5, NOTE_E5, NOTE_E5 };
int melodya3[] = { NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5 };
int melodya4[] = { NOTE_E5 };
int melodya5[] = { NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5 };
int melodya6[] = { NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5 };
int melodya7[] = { NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5 };
int melodya8[] = { NOTE_D5, NOTE_G5 };

int noteDurations1[] = { 8, 8, 4 };
int noteDurations2[] = { 8, 8, 4 };
int noteDurations3[] = { 8, 8, 8, 8 };
int noteDurations4[] = { 2 };
int noteDurations5[] = { 8, 8, 8, 8 };
int noteDurations6[] = { 8, 8, 8, 16, 16 };
int noteDurations7[] = { 8, 8, 8, 8 };
int noteDurations8[] = { 4, 4 };

void ejecutarSonido(int numSon) {

  buzzer.stop();
  if (numSon == 0) {
    buzzer.beep(100);
  }
  if (numSon == 1) {
    buzzer.beep(100);
    delay(100);
    buzzer.beep(100);
  }
  if (numSon == 2) {
    buzzer.playMelody(melodya1, noteDurations1, sizeof(noteDurations1) / sizeof(int));
  }
  if (numSon == 3) {
    buzzer.playMelody(melodya2, noteDurations2, sizeof(noteDurations2) / sizeof(int));
  }
  if (numSon == 4) {
    buzzer.playMelody(melodya3, noteDurations3, sizeof(noteDurations3) / sizeof(int));
  }
  if (numSon == 5) {
    buzzer.playMelody(melodya4, noteDurations4, sizeof(noteDurations4) / sizeof(int));
  }
  if (numSon == 6) {
    buzzer.playMelody(melodya5, noteDurations5, sizeof(noteDurations5) / sizeof(int));
  }
  if (numSon == 7) {
    buzzer.playMelody(melodya6, noteDurations6, sizeof(noteDurations6) / sizeof(int));
  }
  if (numSon == 8) {
    buzzer.playMelody(melodya7, noteDurations7, sizeof(noteDurations7) / sizeof(int));
  }
  if (numSon == 9) {
    buzzer.playMelody(melodya8, noteDurations8, sizeof(noteDurations8) / sizeof(int));
  }
}

#include "pantallas.h"
#include "codigosIR.hpp"

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define CMD_PowerAll "CMD_PowerAll"
#define CMD_powerFinChill "CMD_powerFinChill"
#define CMD_Abrir_Entrada "CMD_Abrir_Entrada"
#define CMD_powerLed "CMD_powerLed"
#define CMD_PowerMegaLed "CMD_PowerMegaLed"
#define CMD_efecto "CMD_efecto"
#define CMD_brillo "CMD_brillo"
#define CMD_brilloMegaLedMas "CMD_brilloMegaLedMas"
#define CMD_PowerUV "CMD_PowerUV"
#define CMD_brilloCirculoMas "CMD_brilloCirculoMas"
#define CMD_Color "CMD_Color"
#define CMD_brilloCirculoMenos "CMD_brilloCirculoMenos"

const int START_BUTTON_PIN = 30;
int lastStartButtonState = LOW;

//*** COPY-PASTE from Serial Terminal:
const int XP = 7, XM = A1, YP = A2, YM = 6;  //480x320 ID=0x9486
const int TS_LEFT = 934, TS_RT = 190, TS_TOP = 193, TS_BOT = 900;
/*

Testing : (A1, D7) = 34
Testing : (A2, D6) = 21
ID = 0x9486


PORTRAIT  CALIBRATION     480 x 320
x = map(p.x, LEFT=934, RT=190, 0, 480)
y = map(p.y, TOP=193, BOT=900, 0, 320)

LANDSCAPE CALIBRATION     320 x 480
x = map(p.y, LEFT=193, RT=900, 0, 320)
y = map(p.x, TOP=190, BOT=934, 0, 480)
*/
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 900);

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

void setup() {
  wifiSerial.begin(9600);  // Iniciamos puerto serial virtual
  Serial.begin(115200);    // Iniciamos puerto serial

  Serial.println("Iniciado...");

  IrReceiver.begin(30);

  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  if (ID == 0xD3D3) ID = 0x9486;  // write-only shield
  tft.begin(ID);
  tft.setRotation(3);  //PORTRAIT

  CargarCodeIR();
  drawMainScreen();
  ejecutarSonido(1);
}

void loop() {
  buzzer.loop();
  checkIR();
  comunicadatos();
  checkBtnMain();
}

void checkIR() {
  if (IrReceiver.decode()) {  // Grab an IR code

    // Check if the buffer overflowed
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
      // Serial.println(F("Try to increase the \"RAW_BUFFER_LENGTH\" value of " STR(RAW_BUFFER_LENGTH) " in " __FILE__));
      // see also https://github.com/Arduino-IRremote/Arduino-IRremote#compile-options--macros-for-this-library
    } else {
      IrReceiver.stop();
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      } else {
        if (IrReceiver.decodedIRData.decodedRawData != 0) {
          Serial.print("value:");
          Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);


          String Code = buscarCodigoEnLista(IrReceiver.decodedIRData.decodedRawData);
          if (Code != "") {
            ejecutarSonido(2);
            executeFunction(Code);
          }
          
        }
      }
      IrReceiver.start();
    }
    IrReceiver.resume();  // Prepare for the next value
  }
}
void checkBtnMain() {

  bool down = Touch_getXY();

  btn_PowerAll.press(down && btn_PowerAll.contains(pixel_x, pixel_y));
  btn_powerFinChill.press(down && btn_powerFinChill.contains(pixel_x, pixel_y));
  btn_Abrir_Entrada.press(down && btn_Abrir_Entrada.contains(pixel_x, pixel_y));
  btn_powerCirculos.press(down && btn_powerCirculos.contains(pixel_x, pixel_y));
  btn_power_ML.press(down && btn_power_ML.contains(pixel_x, pixel_y));
  btn_power_UV.press(down && btn_power_UV.contains(pixel_x, pixel_y));
  btn_nextEfeto.press(down && btn_nextEfeto.contains(pixel_x, pixel_y));
  btn_brilloML.press(down && btn_brilloML.contains(pixel_x, pixel_y));
  btn_brilloCirculosMas.press(down && btn_brilloCirculosMas.contains(pixel_x, pixel_y));
  btn_brilloCirculosMenos.press(down && btn_brilloCirculosMenos.contains(pixel_x, pixel_y));
  btn_NextColor.press(down && btn_NextColor.contains(pixel_x, pixel_y));

  if (btn_PowerAll.justReleased()) { btn_PowerAll.drawButton(false); }
  if (btn_powerFinChill.justReleased()) { btn_powerFinChill.drawButton(false); }
  if (btn_Abrir_Entrada.justReleased()) { btn_Abrir_Entrada.drawButton(false); }
  if (btn_powerCirculos.justReleased()) { btn_powerCirculos.drawButton(false); }
  if (btn_power_ML.justReleased()) { btn_power_ML.drawButton(false); }
  if (btn_power_UV.justReleased()) { btn_power_UV.drawButton(false); }
  if (btn_nextEfeto.justReleased()) { btn_nextEfeto.drawButton(false); }
  if (btn_brilloML.justReleased()) { btn_brilloML.drawButton(false); }
  if (btn_brilloCirculosMas.justReleased()) { btn_brilloCirculosMas.drawButton(false); }
  if (btn_brilloCirculosMenos.justReleased()) { btn_brilloCirculosMenos.drawButton(false); }
  if (btn_NextColor.justReleased()) { btn_NextColor.drawButton(false); }


  if (btn_PowerAll.justPressed()) {
    executeFunction(CMD_PowerAll);
    btn_PowerAll.drawButton(true);
    delay(500);
  }
  if (btn_powerFinChill.justPressed()) {
    executeFunction(CMD_powerFinChill);
    btn_powerFinChill.drawButton(true);
    delay(500);
  }
  if (btn_Abrir_Entrada.justPressed()) {
    executeFunction(CMD_Abrir_Entrada);
    btn_Abrir_Entrada.drawButton(true);
    delay(500);
  }


  if (btn_powerCirculos.justPressed()) {
    executeFunction(CMD_powerLed);
    btn_powerCirculos.drawButton(true);
    delay(500);
  }
  if (btn_power_ML.justPressed()) {
    executeFunction(CMD_PowerMegaLed);
    btn_power_ML.drawButton(true);
    delay(500);
  }
  if (btn_power_UV.justPressed()) {
    executeFunction(CMD_PowerUV);
    btn_power_UV.drawButton(true);
    delay(500);
  }
  if (btn_nextEfeto.justPressed()) {
    executeFunction(CMD_efecto);
    btn_nextEfeto.drawButton(true);
    delay(500);
  }


  if (btn_brilloML.justPressed()) {
    executeFunction(CMD_brilloMegaLedMas);
    btn_brilloML.drawButton(true);
    delay(500);
  }
  if (btn_brilloCirculosMas.justPressed()) {
    executeFunction(CMD_brilloCirculoMas);
    btn_brilloCirculosMas.drawButton(true);
    delay(500);
  }
  if (btn_brilloCirculosMenos.justPressed()) {
    executeFunction(CMD_brilloCirculoMenos);
    btn_brilloCirculosMenos.drawButton(true);
    delay(500);
  }
  if (btn_NextColor.justPressed()) {
    executeFunction(CMD_Color);
    btn_NextColor.drawButton(true);
    delay(500);
  }
}


/*
  #define CMD_PowerAll "CMD_PowerAll"
  #define CMD_powerFinChill "CMD_powerFinChill"
  #define CMD_Abrir_Entrada "CMD_Abrir_Entrada"
  #define CMD_powerLed "CMD_powerLed"
  #define CMD_power_flor "CMD_power_flor"
  #define CMD_efecto "CMD_efecto"
  #define CMD_brillo "CMD_brillo"
  #define CMD_brilloMegaLedMas "CMD_brilloMegaLedMas"
  #define CMD_PowerUV "CMD_PowerUV"
  #define CMD_audio "CMD_audio"
  #define CMD_Color "CMD_Color"
  */

void executeFunction(String codeCMD) {
  Serial.println("send to wifi: " + codeCMD);
  wifiSerial.println(codeCMD);
}

String wifiSerialData;
String SerialData;

void comunicadatos() {
  //Envíamos y recibimos datos
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