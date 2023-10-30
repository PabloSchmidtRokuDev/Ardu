#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(53, 52);  //Seleccionamos los pines 7 como Rx y 8 como Tx

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

#define destino_Habitacion "0"
#define destino_Living "1"
#define destino_puerta "2"

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP = 8, XM = A2, YP = A3, YM = 9;  //240x320 ID=0x9341
const int TS_LEFT = 143, TS_RT = 861, TS_TOP = 117, TS_BOT = 920;

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


void setup() {
  wifiSerial.begin(115200);  // Iniciamos puerto serial virtual
  Serial.begin(115200);      // Iniciamos puerto serial
  Serial.println("Iniciado...");

  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  if (ID == 0xD3D3) ID = 0x9486;  // write-only shield
  tft.begin(ID);
  tft.setRotation(0);  //PORTRAIT

  drawMainScreen();
}

void loop() {
  comunicadatos();

  if (currentScreen == "main" || currentScreen == "") {
    checkBtnMain();
  } else if (currentScreen == "habitacion") {
    checkBtnHabitacion();
  } else if (currentScreen == "living") {
    checkBtnLiving();
  } else if (currentScreen == "puertas") {
  } else if (currentScreen == "color") {
    checkBtnColor();
  }
}

void checkBtnMain() {

  bool down = Touch_getXY();

  // btn_PowerAll.press(down && btn_PowerAll.contains(pixel_x, pixel_y));
  // btn_remotePower.press(down && btn_remotePower.contains(pixel_x, pixel_y));
  btn_Habitacion.press(down && btn_Habitacion.contains(pixel_x, pixel_y));
  btn_Living.press(down && btn_Living.contains(pixel_x, pixel_y));
  //btn_Puertas.press(down && btn_Puertas.contains(pixel_x, pixel_y));

  if (btn_PowerAll.justReleased()) { btn_PowerAll.drawButton(false); }
  if (btn_remotePower.justReleased()) { btn_remotePower.drawButton(false); }
  if (btn_Habitacion.justReleased()) { btn_Habitacion.drawButton(false); }
  if (btn_Living.justReleased()) { btn_Living.drawButton(false); }
  //if (btn_Puertas.justReleased()) { btn_Puertas.drawButton(false); }

  // if (btn_PowerAll.justPressed()) {  executeFunction("0", "HEX", "BF40FF00"); btn_PowerAll.drawButton(true); delay(500);  }
  // if (btn_remotePower.justPressed()) {  btn_remotePower.drawButton(true);  delay(500); executeFunction("0", "HEX", "AB54FF00"); }
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
  //if (btn_Puertas.justPressed()) { btn_Puertas.drawButton(true);  delay(500); drawPuertasScreen(); }
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
    executeFunction("0", "HEX", "BF40FF00");
    btn_power.drawButton(true);
    delay(500);
  }
  if (btn_colores.justPressed()) {
    btn_colores.drawButton(true);
    delay(500);
    drawColoresScreen("habitacion", destino_Habitacion);
  }

  if (btn_Brillo100.justPressed()) {
    executeFunction("0", "HEX", "E41BFF00");
    btn_Brillo100.drawButton(true);
    delay(500);
  }
  if (btn_Brillo70.justPressed()) {
    executeFunction("0", "HEX", "E01FFF00");
    btn_Brillo70.drawButton(true);
    delay(500);
  }
  if (btn_Brillo40.justPressed()) {
    executeFunction("0", "HEX", "B34CFF00");
    btn_Brillo40.drawButton(true);
    delay(500);
  }
  if (btn_Brillo15.justPressed()) {
    executeFunction("0", "HEX", "B748FF00");
    btn_Brillo15.drawButton(true);
    delay(500);
  }

  if (btn_SelectEfecto1.justPressed()) {
    executeFunction("0", "HEX", "F20DFF00");
    btn_SelectEfecto1.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto2.justPressed()) {
    executeFunction("0", "HEX", "F10EFF00");
    btn_SelectEfecto2.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto3.justPressed()) {
    executeFunction("0", "HEX", "F00FFF00");
    btn_SelectEfecto3.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto4.justPressed()) {
    executeFunction("0", "HEX", "F708FF00");
    btn_SelectEfecto4.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto5.justPressed()) {
    executeFunction("0", "HEX", "F609FF00");
    btn_SelectEfecto5.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto6.justPressed()) {
    executeFunction("0", "HEX", "F50AFF00");
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
    executeFunction("1", "HEX", "BF40FF00");
    btn_power.drawButton(true);
    delay(500);
  }
  if (btn_colores.justPressed()) {
    btn_colores.drawButton(true);
    delay(500);
    drawColoresScreen("living", destino_Living);
  }

  if (btn_Brillo100.justPressed()) {
    executeFunction("1", "HEX", "E41BFF00");
    btn_Brillo100.drawButton(true);
    delay(500);
  }
  if (btn_Brillo70.justPressed()) {
    executeFunction("1", "HEX", "E01FFF00");
    btn_Brillo70.drawButton(true);
    delay(500);
  }
  if (btn_Brillo40.justPressed()) {
    executeFunction("1", "HEX", "B34CFF00");
    btn_Brillo40.drawButton(true);
    delay(500);
  }
  if (btn_Brillo15.justPressed()) {
    executeFunction("1", "HEX", "B748FF00");
    btn_Brillo15.drawButton(true);
    delay(500);
  }

  if (btn_SelectEfecto1.justPressed()) {
    executeFunction("1", "HEX", "F20DFF00");
    btn_SelectEfecto1.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto2.justPressed()) {
    executeFunction("1", "HEX", "F10EFF00");
    btn_SelectEfecto2.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto3.justPressed()) {
    executeFunction("1", "HEX", "F00FFF00");
    btn_SelectEfecto3.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto4.justPressed()) {
    executeFunction("1", "HEX", "F708FF00");
    btn_SelectEfecto4.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto5.justPressed()) {
    executeFunction("1", "HEX", "F609FF00");
    btn_SelectEfecto5.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto6.justPressed()) {
    executeFunction("1", "HEX", "F50AFF00");
    btn_SelectEfecto6.drawButton(true);
    delay(500);
  }

  if (btn_Bola.justPressed()) {
    executeFunction("1", "HEX", "BE41FF00");
    btn_Bola.drawButton(true);
    delay(500);
  }
  if (btn_UV.justPressed()) {
    executeFunction("1", "HEX", "A25DFF00");
    btn_UV.drawButton(true);
    delay(500);
  }

  if (btn_Audio.justPressed()) {
    if (audioSelected == 1) {
      Serial.println("Envia audio 1");
      executeFunction("1", "HEX", "EF10FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }
    if (audioSelected == 2) {
      Serial.println("Envia audio 2");
      executeFunction("1", "HEX", "EE11FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }
    if (audioSelected == 3) {
      Serial.println("Envia audio 3");
      executeFunction("1", "HEX", "ED12FF00");
      btn_Audio.drawButton(true);
      delay(500);
    }
    if (audioSelected == 4) {
      Serial.println("Envia audio 4");
      executeFunction("1", "HEX", "EC13FF00");
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


void checkBtnColor2() {

  bool down = Touch_getXY();

  btn_SelectEfecto1.press(down && btn_SelectEfecto1.contains(pixel_x, pixel_y));
  btn_SelectEfecto2.press(down && btn_SelectEfecto2.contains(pixel_x, pixel_y));
  btn_SelectEfecto3.press(down && btn_SelectEfecto3.contains(pixel_x, pixel_y));
  btn_SelectEfecto4.press(down && btn_SelectEfecto4.contains(pixel_x, pixel_y));


btn_backG1.press(down && btn_backG1.contains(pixel_x, pixel_y));
btn_nextG1.press(down && btn_nextG1.contains(pixel_x, pixel_y));
btn_backG2.press(down && btn_backG2.contains(pixel_x, pixel_y));
btn_nextG2.press(down && btn_nextG2.contains(pixel_x, pixel_y));
btn_backG3.press(down && btn_backG3.contains(pixel_x, pixel_y));
btn_nextG3.press(down && btn_nextG3.contains(pixel_x, pixel_y));
btn_backG4.press(down && btn_backG4.contains(pixel_x, pixel_y));
btn_nextG4.press(down && btn_nextG4.contains(pixel_x, pixel_y));
btn_backG5.press(down && btn_backG5.contains(pixel_x, pixel_y));
btn_nextG5.press(down && btn_nextG5.contains(pixel_x, pixel_y));

 btn_select1.press(down && btn_select1.contains(pixel_x, pixel_y));
 btn_select2.press(down && btn_select2.contains(pixel_x, pixel_y));
 btn_select3.press(down && btn_select3.contains(pixel_x, pixel_y));
 btn_select4.press(down && btn_select4.contains(pixel_x, pixel_y));
 btn_select5.press(down && btn_select5.contains(pixel_x, pixel_y));
 btn_aplicar.press(down && btn_aplicar.contains(pixel_x, pixel_y));

/*

  if (btn_SelectEfecto1.justReleased()) { btn_SelectEfecto1.drawButton(false); }
  if (btn_SelectEfecto2.justReleased()) { btn_SelectEfecto2.drawButton(false); }
  if (btn_SelectEfecto3.justReleased()) { btn_SelectEfecto3.drawButton(false); }
  if (btn_SelectEfecto4.justReleased()) { btn_SelectEfecto4.drawButton(false); }
*/
  if (btn_backG1.justReleased()) { btn_backG1.drawButton(false); }
  if (btn_nextG1.justReleased()) { btn_nextG1.drawButton(false); }
  if (btn_backG2.justReleased()) { btn_backG2.drawButton(false); }
  if (btn_nextG2.justReleased()) { btn_nextG2.drawButton(false); }
  if (btn_backG3.justReleased()) { btn_backG3.drawButton(false); }
  if (btn_nextG3.justReleased()) { btn_nextG3.drawButton(false); }
  if (btn_backG4.justReleased()) { btn_backG4.drawButton(false); }
  if (btn_nextG4.justReleased()) { btn_nextG4.drawButton(false); }
  if (btn_backG5.justReleased()) { btn_backG5.drawButton(false); }
  if (btn_nextG5.justReleased()) { btn_nextG5.drawButton(false); }

  if (btn_select1.justReleased()) { btn_select1.drawButton(false); }
  if (btn_select2.justReleased()) { btn_select2.drawButton(false); }
  if (btn_select3.justReleased()) { btn_select3.drawButton(false); }
  if (btn_select4.justReleased()) { btn_select4.drawButton(false); }
  if (btn_select5.justReleased()) { btn_select5.drawButton(false); }
  if (btn_aplicar.justReleased()) { btn_aplicar.drawButton(false); }



 if (btn_SelectEfecto1.justPressed()) {
    selectEfectoColor(1);
    btn_SelectEfecto1.drawButton(true);
    btn_SelectEfecto2.drawButton(false);
    btn_SelectEfecto3.drawButton(false);
    btn_SelectEfecto4.drawButton(false);
    delay(500);
  }
  if (btn_SelectEfecto2.justPressed()) {
    selectEfectoColor(2);
    btn_SelectEfecto2.drawButton(true);
     btn_SelectEfecto1.drawButton(false);
    btn_SelectEfecto3.drawButton(false);
    btn_SelectEfecto4.drawButton(false);
    delay(500);
  }
  if (btn_SelectEfecto3.justPressed()) {
   selectEfectoColor(3);
     btn_SelectEfecto1.drawButton(false);
 btn_SelectEfecto2.drawButton(false);
 btn_SelectEfecto4.drawButton(false);
    btn_SelectEfecto3.drawButton(true);
    delay(500);
  }
  if (btn_SelectEfecto4.justPressed()) {
    selectEfectoColor(4);
     btn_SelectEfecto1.drawButton(false);
      btn_SelectEfecto2.drawButton(false);
      btn_SelectEfecto3.drawButton(false);
    btn_SelectEfecto4.drawButton(true);
    delay(500);
  }
  
  if (btn_backG1.justPressed()) {selectColor(-1); btn_backG1.drawButton(true);delay(500);}
  if (btn_nextG1.justPressed()) {selectColor(1); btn_nextG1.drawButton(true);delay(500);}
  if (btn_backG2.justPressed()) {selectColor(-2); btn_backG2.drawButton(true);delay(500);}
  if (btn_nextG2.justPressed()) {selectColor(2); btn_nextG2.drawButton(true);delay(500);}
  if (btn_backG3.justPressed()) {selectColor(-3); btn_backG3.drawButton(true);delay(500);}
  if (btn_nextG3.justPressed()) {selectColor(3); btn_nextG3.drawButton(true);delay(500);}
  if (btn_backG4.justPressed()) {selectColor(-4); btn_backG4.drawButton(true);delay(500);}
  if (btn_nextG4.justPressed()) {selectColor(4); btn_nextG4.drawButton(true);delay(500);}
  if (btn_backG5.justPressed()) {selectColor(-5); btn_backG5.drawButton(true);delay(500);}
  if (btn_nextG5.justPressed()) {selectColor(5); btn_nextG5.drawButton(true);delay(500);}

  if (btn_select1.justPressed()) {selectColorGrupo(1); btn_select1.drawButton(true);delay(500);}
  if (btn_select2.justPressed()) {selectColorGrupo(2); btn_select2.drawButton(true);delay(500);}
  if (btn_select3.justPressed()) {selectColorGrupo(3); btn_select3.drawButton(true);delay(500);}
  if (btn_select4.justPressed()) {selectColorGrupo(4); btn_select4.drawButton(true);delay(500);}
  if (btn_select5.justPressed()) {selectColorGrupo(5); btn_select5.drawButton(true);delay(500);}


if (btn_aplicar.justPressed()) {selectAplicar(); btn_aplicar.drawButton(true);delay(500);}

  if (btn_volver.justPressed()) {
    btn_volver.drawButton(true);
    delay(500);
    volverPantalla();
  }
}


void selectEfectoColor(int grupo){

}
void selectColor(int grupo){

}

void selectColorGrupo(int grupo){

}

void selectAplicar(){

}


void volverPantalla() {

  // if (currentScreen == " } else
  Serial.println("volver: " + currentScreen);

  if (currentScreen == "color" && previosScreen == "habitacion") {
    drawHabitacionScreen();

  } else if (currentScreen == "color" && previosScreen == "living") {
    drawLivingScreen();

  } else if (currentScreen == "habitacion" || currentScreen == "living" || currentScreen == "puertas") {
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