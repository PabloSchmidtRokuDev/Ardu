#include <SoftwareSerial.h>

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>

#include <Fonts/FreeSans9pt7b.h>


#include "pantallas.h"

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define destino_Habitacion "habitacion          "
#define destino_Living "living              "
#define destino_doble_barra "doble_barra         "

const int XP=6,XM=A2,YP=A1,YM=7; //320x480 ID=0x9486
const int TS_LEFT=902,TS_RT=220,TS_TOP=929,TS_BOT=212;

//PORTRAIT  CALIBRATION     320 x 480
//x = map(p.x, LEFT=902, RT=220, 0, 320)
//y = map(p.y, TOP=929, BOT=212, 0, 480)

//LANDSCAPE CALIBRATION     480 x 320
// int x = map(p.y, LEFT=929, RT=212, 0, 480)
// int y = map(p.x, TOP=220, BOT=902, 0, 320)

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
    Serial.println("pixel_x..."  + String(pixel_x));
    Serial.println("pixel_y..."+ String(pixel_y));
  }
  return pressed;
}

SoftwareSerial wifiSerial(13, 12); 


void setup() {
  Serial.begin(115200);  // Iniciamos puerto serial
wifiSerial.begin(115200);  // Iniciamos puerto serial virtual

  Serial.println("Iniciado...");

  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  if (ID == 0xD3D3) ID = 0x9486;  // write-only shield
  tft.begin(ID);
  tft.setRotation(1);  //PORTRAIT

  drawMainScreen();
}

void loop() {

  bool down = Touch_getXY();

  btn1.press(down && btn1.contains(pixel_x, pixel_y));
  btn2.press(down && btn2.contains(pixel_x, pixel_y));

  btn3.press(down && btn3.contains(pixel_x, pixel_y));
  btn4.press(down && btn4.contains(pixel_x, pixel_y));
  btn5.press(down && btn5.contains(pixel_x, pixel_y));
  btn6.press(down && btn6.contains(pixel_x, pixel_y));

  btn7.press(down && btn7.contains(pixel_x, pixel_y));
  btn8.press(down && btn8.contains(pixel_x, pixel_y));
  btn9.press(down && btn9.contains(pixel_x, pixel_y));
  btn10.press(down && btn10.contains(pixel_x, pixel_y));
  // btn11.press(down && btn11.contains(pixel_x, pixel_y));






  if (currentScreen == "main" || currentScreen == "") {

    // btn12.press(down && btn12.contains(pixel_x, pixel_y));
    // btn13.press(down && btn13.contains(pixel_x, pixel_y));
    // btn14.press(down && btn14.contains(pixel_x, pixel_y));
    // btn15.press(down && btn15.contains(pixel_x, pixel_y));

    // if (btn12.justReleased()) { btn12.drawButton(false); }
    // if (btn13.justReleased()) { btn13.drawButton(false); }
    // if (btn14.justReleased()) { btn14.drawButton(false); }
    // if (btn15.justReleased()) { btn15.drawButton(false); }
    checkBtnMain();
  } else if (currentScreen == "color") {
    checkBtnColor();
  }



  if (btn1.justReleased()) { btn1.drawButton(false); }
  if (btn2.justReleased()) { btn2.drawButton(false); }

  if (btn3.justReleased()) { btn3.drawButton(false); }
  if (btn4.justReleased()) { btn4.drawButton(false); }
  if (btn5.justReleased()) { btn5.drawButton(false); }
  if (btn6.justReleased()) { btn6.drawButton(false); }

  if (btn7.justReleased()) { btn7.drawButton(false); }
  if (btn8.justReleased()) { btn8.drawButton(false); }
  if (btn9.justReleased()) { btn9.drawButton(false); }
  if (btn10.justReleased()) { btn10.drawButton(false); }
  // if (btn11.justReleased()) { btn11.drawButton(false); }

}

int audioSelected = 1;

void checkBtnMain() {


  if (btn1.justPressed()) {
    executeFunction(destino_Living, "HEX", "BF40FF00");
    btn1.drawButton(true);
    delay(500);
  }
  if (btn2.justPressed()) {
    btn2.drawButton(true);
    delay(500);
    drawColoresScreen();
  }

  if (btn3.justPressed()) {
    executeFunction(destino_Living, "HEX", "E41BFF00");
    btn3.drawButton(true);
    delay(500);
  }
  if (btn4.justPressed()) {
    executeFunction(destino_Living, "HEX", "E01FFF00");
    btn4.drawButton(true);
    delay(500);
  }
  if (btn5.justPressed()) {
    executeFunction(destino_Living, "HEX", "B34CFF00");
    btn5.drawButton(true);
    delay(500);
  }
  if (btn6.justPressed()) {
    executeFunction(destino_Living, "HEX", "B748FF00");
    btn6.drawButton(true);
    delay(500);
  }

  if (btn7.justPressed()) {
    executeFunction(destino_Living, "HEX", "F20DFF00");
    btn7.drawButton(true);
    delay(500);
  }
  if (btn8.justPressed()) {
    executeFunction(destino_Living, "HEX", "F10EFF00");
    btn8.drawButton(true);
    delay(500);
  }
  if (btn9.justPressed()) {
    executeFunction(destino_Living, "HEX", "F00FFF00");
    btn9.drawButton(true);
    delay(500);
  }
  if (btn10.justPressed()) {
    executeFunction(destino_Living, "HEX", "F708FF00");
    btn10.drawButton(true);
    delay(500);
  }
  // if (btn11.justPressed()) {
  //   executeFunction(destino_Living, "HEX", "F609FF00");
  //   btn11.drawButton(true);
  //   delay(500);
  // }
  // if (btn12.justPressed()) {
  //   executeFunction(destino_Living, "HEX", "F50AFF00");
  //   btn12.drawButton(true);
  //   delay(500);
  // }

  // if (btn13.justPressed()) {
  //   executeFunction(destino_Living, "HEX", "BE41FF00");
  //   btn13.drawButton(true);
  //   delay(500);
  // }
  // if (btn14.justPressed()) {
  //   executeFunction(destino_Living, "HEX", "A25DFF00");
  //   btn14.drawButton(true);
  //   delay(500);
  // }

  // if (btn15.justPressed()) {
  //   if (audioSelected == 1) {
  //     Serial.println("Envia audio 1");
  //     executeFunction(destino_Living, "HEX", "EF10FF00");
  //     btn15.drawButton(true);
  //     delay(500);
  //   }
  //   if (audioSelected == 2) {
  //     Serial.println("Envia audio 2");
  //     executeFunction(destino_Living, "HEX", "EE11FF00");
  //     btn15.drawButton(true);
  //     delay(500);
  //   }
  //   if (audioSelected == 3) {
  //     Serial.println("Envia audio 3");
  //     executeFunction(destino_Living, "HEX", "ED12FF00");
  //     btn15.drawButton(true);
  //     delay(500);
  //   }
  //   if (audioSelected == 4) {
  //     Serial.println("Envia audio 4");
  //     executeFunction(destino_Living, "HEX", "EC13FF00");
  //     btn15.drawButton(true);
  //     delay(500);
  //   }

  //   audioSelected++;
  //   if (audioSelected == 5) audioSelected = 1;
  // }

  if (btn6.justPressed()) {
    btn6.drawButton(true);
    delay(500);
    volverPantalla();
  }
}


void checkBtnColor() {


  if (btn1.justPressed()) {
    executeFunction("living", "HEX", "A758FF00");
    btn1.drawButton(true);
    delay(500);
  }
  if (btn2.justPressed()) {
    executeFunction("living", "HEX", "AB54FF00");
    btn2.drawButton(true);
    delay(500);
  }
  if (btn3.justPressed()) {
    executeFunction("living", "HEX", "AF50FF00");
    btn3.drawButton(true);
    delay(500);
  }
  if (btn4.justPressed()) {
    executeFunction("living", "HEX", "E31CFF00");
    btn4.drawButton(true);
    delay(500);
  }
  if (btn5.justPressed()) {
    executeFunction("living", "HEX", "E718FF00");
    btn5.drawButton(true);
    delay(500);
  }

  if (btn6.justPressed()) {
    executeFunction("living", "HEX", "A659FF00");
    btn6.drawButton(true);
    delay(500);
  }
  if (btn7.justPressed()) {
    executeFunction("living", "HEX", "AA55FF00");
    btn7.drawButton(true);
    delay(500);
  }
  if (btn8.justPressed()) {
    executeFunction("living", "HEX", "AE51FF00");
    btn8.drawButton(true);
    delay(500);
  }
  if (btn9.justPressed()) {
    executeFunction("living", "HEX", "E21DFF00");
    btn9.drawButton(true);
    delay(500);
  }
  if (btn10.justPressed()) {
    executeFunction("living", "HEX", "E619FF00");
    btn10.drawButton(true);
    delay(500);
  }

  // if (btn11.justPressed()) {
  //   volverPantalla();
  //     btn11.drawButton(true);
  //   delay(500);
  // }
}


void volverPantalla() {

  drawMainScreen();
}

void executeFunction(String destino, String preCodigo, String code) {
  String dataX = destino + preCodigo + code;
  //Serial.println("send to wifi: " + dataX);
  Serial.println(dataX);
  wifiSerial.println(dataX);
  
}
