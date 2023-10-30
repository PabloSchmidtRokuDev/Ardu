#include <SoftwareSerial.h>
#include <Buzzer.h>
//#include <Servo.h>

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#include <TouchScreen.h>

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>
SoftwareSerial wifiSerial(52, 53);  //Seleccionamos los pines 7 como Rx y 8 como Tx

MCUFRIEND_kbv tft;

#include "pantallas.h"

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define destino_Habitacion "habitacion          "
#define destino_Living "living              "
#define destino_doble_barra "doble_barra         "
#define destino_faros "faros               "

Buzzer buzzer(34, 35);

#define PIN_servo1 24
#define PIN_servo2 26



int pin_R = 10;
int pin_B = 11;
int pin_G = 12;

bool powerFaro = false;

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;

unsigned long cTiempo1 = 0;
unsigned long cTiempo2 = 0;
unsigned long cTiempoSegundos = 0;

unsigned long sTiempo1 = 0;
unsigned long sTiempo2 = 0;
unsigned long sTiempoSegundos = 0;

const int START_BUTTON_PIN = 31;
int lastStartButtonState = LOW;

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP = 8, XM = A2;  //240x320 ID=0x9341
const int YP = A3, YM = 9;

const int TS_LEFT = 884;
const int TS_RT = 131;
const int TS_TOP = 903;
const int TS_BOT = 82;
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


class colorX {
public:
  bool select = false;
  uint16_t colorHEX;
  int r;
  int g;
  int b;
  String nombre;

  int posX;
  int posY;

  void setEstado(String _nombre, int _r, int _g, int _b, uint16_t _colorHex, int _posX, int _posY);
};


void colorX::setEstado(String _nombre, int _r, int _g, int _b, uint16_t _colorHex, int _posX, int _posY) {
  nombre = _nombre;
  r = _r;
  g = _g;
  b = _b;
  colorHEX = _colorHex;
  posX = _posX;
  posY = _posY;

};


const int maxColors = 150;
colorX colorXList[maxColors];  // An array of MyClass objects using default constructor

void cargarVecctorColores() {
  colorXList[0].setEstado("AliceBlue", 240, 248, 255, 0xF0F8FF, 0, 0);
  colorXList[1].setEstado("Amethyst", 153, 102, 204, 0x9966CC, 1, 0);
  colorXList[2].setEstado("AntiqueWhite", 250, 235, 215, 0xFAEBD7, 2, 0);
  colorXList[3].setEstado("Aqua", 0, 255, 255, 0x00FFFF, 3, 0);
  colorXList[4].setEstado("Aquamarine", 127, 255, 212, 0x7FFFD4, 4, 0);
  colorXList[5].setEstado("Azure", 240, 255, 255, 0xF0FFFF, 5, 0);
  colorXList[6].setEstado("Beige", 245, 245, 220, 0xF5F5DC, 6, 0);
  colorXList[7].setEstado("Bisque", 255, 228, 196, 0xFFE4C4, 7, 0);
  colorXList[8].setEstado("Black", 0, 0, 0, 0x000000, 0, 1);
  colorXList[9].setEstado("BlanchedAlmond", 255, 235, 205, 0xFFEBCD, 1, 1);
  colorXList[10].setEstado("Blue", 0, 0, 255, 0x0000FF, 2, 1);
  colorXList[11].setEstado("BlueViolet", 138, 43, 226, 0x8A2BE2, 3, 1);
  colorXList[12].setEstado("Brown", 165, 42, 42, 0xA52A2A, 4, 1);
  colorXList[13].setEstado("BurlyWood", 222, 184, 135, 0xDEB887, 5, 1);
  colorXList[14].setEstado("CadetBlue", 95, 158, 160, 0x5F9EA0, 6, 1);
  colorXList[15].setEstado("Chartreuse", 127, 255, 0, 0x7FFF00, 7, 1);
  colorXList[16].setEstado("Chocolate", 210, 105, 30, 0xD2691E, 0, 2);
  colorXList[17].setEstado("Coral", 255, 127, 80, 0xFF7F50, 1, 2);
  colorXList[18].setEstado("CornflowerBlue", 100, 149, 237, 0x6495ED, 2, 2);
  colorXList[19].setEstado("Cornsilk", 255, 248, 220, 0xFFF8DC, 3, 2);
  colorXList[20].setEstado("Crimson", 220, 20, 60, 0xDC143C, 4, 2);
  colorXList[21].setEstado("Cyan", 0, 255, 255, 0x00FFFF, 5, 2);
  colorXList[22].setEstado("DarkBlue", 0, 0, 139, 0x00008B, 6, 2);
  colorXList[23].setEstado("DarkCyan", 0, 139, 139, 0x008B8B, 7, 2);
  colorXList[24].setEstado("DarkGoldenrod", 184, 134, 11, 0xB8860B, 0, 3);
  colorXList[25].setEstado("DarkGray", 169, 169, 169, 0xA9A9A9, 1, 3);
  colorXList[26].setEstado("DarkGrey", 169, 169, 169, 0xA9A9A9, 2, 3);
  colorXList[27].setEstado("DarkGreen", 0, 100, 0, 0x006400, 3, 3);
  colorXList[28].setEstado("DarkKhaki", 189, 183, 107, 0xBDB76B, 4, 3);
  colorXList[29].setEstado("DarkMagenta", 139, 0, 139, 0x8B008B, 5, 3);
  colorXList[30].setEstado("DarkOliveGreen", 85, 107, 47, 0x556B2F, 6, 3);
  colorXList[31].setEstado("DarkOrange", 255, 140, 0, 0xFF8C00, 7, 3);
  colorXList[32].setEstado("DarkOrchid", 153, 50, 204, 0x9932CC, 0, 4);
  colorXList[33].setEstado("DarkRed", 139, 0, 0, 0x8B0000, 1, 4);
  colorXList[34].setEstado("DarkSalmon", 233, 150, 122, 0xE9967A, 2, 4);
  colorXList[35].setEstado("DarkSeaGreen", 143, 188, 143, 0x8FBC8F, 3, 4);
  colorXList[36].setEstado("DarkSlateBlue", 72, 61, 139, 0x483D8B, 4, 4);
  colorXList[37].setEstado("DarkSlateGray", 47, 79, 79, 0x2F4F4F, 5, 4);
  colorXList[38].setEstado("DarkSlateGrey", 47, 79, 79, 0x2F4F4F, 6, 4);
  colorXList[39].setEstado("DarkTurquoise", 0, 206, 209, 0x00CED1, 7, 4);
  colorXList[40].setEstado("DarkViolet", 148, 0, 211, 0x9400D3, 0, 5);
  colorXList[41].setEstado("DeepPink", 255, 20, 147, 0xFF1493, 1, 5);
  colorXList[42].setEstado("DeepSkyBlue", 0, 191, 255, 0x00BFFF, 2, 5);
  colorXList[43].setEstado("DimGray", 105, 105, 105, 0x696969, 3, 5);
  colorXList[44].setEstado("DimGrey", 105, 105, 105, 0x696969, 4, 5);
  colorXList[45].setEstado("DodgerBlue", 30, 144, 255, 0x1E90FF, 5, 5);
  colorXList[46].setEstado("FireBrick", 178, 34, 34, 0xB22222, 6, 5);
  colorXList[47].setEstado("FloralWhite", 255, 250, 240, 0xFFFAF0, 7, 5);
  colorXList[48].setEstado("ForestGreen", 34, 139, 34, 0x228B22, 0, 6);
  colorXList[49].setEstado("Fuchsia", 255, 0, 255, 0xFF00FF, 1, 6);
  colorXList[50].setEstado("Gainsboro", 220, 220, 220, 0xDCDCDC, 2, 6);
  colorXList[51].setEstado("GhostWhite", 248, 248, 255, 0xF8F8FF, 3, 6);
  colorXList[52].setEstado("Gold", 255, 215, 0, 0xFFD700, 4, 6);
  colorXList[53].setEstado("Goldenrod", 218, 165, 32, 0xDAA520, 5, 6);
  colorXList[54].setEstado("Gray", 128, 128, 128, 0x808080, 6, 6);
  colorXList[55].setEstado("Grey", 128, 128, 128, 0x808080, 7, 6);
  colorXList[56].setEstado("Green", 0, 128, 0, 0x008000, 0, 7);
  colorXList[57].setEstado("GreenYellow", 173, 255, 47, 0xADFF2F, 1, 7);
  colorXList[58].setEstado("Honeydew", 240, 255, 240, 0xF0FFF0, 2, 7);
  colorXList[59].setEstado("HotPink", 255, 105, 180, 0xFF69B4, 3, 7);
  colorXList[60].setEstado("IndianRed", 205, 92, 92, 0xCD5C5C, 4, 7);
  colorXList[61].setEstado("Indigo", 75, 0, 130, 0x4B0082, 5, 7);
  colorXList[62].setEstado("Ivory", 255, 255, 240, 0xFFFFF0, 6, 7);
  colorXList[63].setEstado("Khaki", 240, 230, 140, 0xF0E68C, 7, 7);
  colorXList[64].setEstado("Lavender", 230, 230, 250, 0xE6E6FA, 0, 8);
  colorXList[65].setEstado("LavenderBlush", 255, 240, 245, 0xFFF0F5, 1, 8);
  colorXList[66].setEstado("LawnGreen", 124, 252, 0, 0x7CFC00, 2, 8);
  colorXList[67].setEstado("LemonChiffon", 255, 250, 205, 0xFFFACD, 3, 8);
  colorXList[68].setEstado("LightBlue", 173, 216, 230, 0xADD8E6, 4, 8);
  colorXList[69].setEstado("LightCoral", 240, 128, 128, 0xF08080, 5, 8);
  colorXList[70].setEstado("LightCyan", 224, 255, 255, 0xE0FFFF, 6, 8);
  colorXList[71].setEstado("LightGoldenrodYellow", 250, 250, 210, 0xFAFAD2, 7, 8);
  colorXList[72].setEstado("LightGreen", 144, 238, 144, 0x90EE90, 0, 9);
  colorXList[73].setEstado("LightGrey", 211, 211, 211, 0xD3D3D3, 1, 9);
  colorXList[74].setEstado("LightPink", 255, 182, 193, 0xFFB6C1, 2, 9);
  colorXList[75].setEstado("LightSalmon", 255, 160, 122, 0xFFA07A, 3, 9);
  colorXList[76].setEstado("LightSeaGreen", 32, 178, 170, 0x20B2AA, 4, 9);
  colorXList[77].setEstado("LightSkyBlue", 135, 206, 250, 0x87CEFA, 5, 9);
  colorXList[78].setEstado("LightSlateGray", 119, 136, 153, 0x778899, 6, 9);
  colorXList[79].setEstado("LightSlateGrey", 119, 136, 153, 0x778899, 7, 9);
  colorXList[80].setEstado("LightSteelBlue", 176, 196, 222, 0xB0C4DE, 0, 10);
  colorXList[81].setEstado("LightYellow", 255, 255, 224, 0xFFFFE0, 1, 10);
  colorXList[82].setEstado("Lime", 0, 255, 0, 0x00FF00, 2, 10);
  colorXList[83].setEstado("LimeGreen", 50, 205, 50, 0x32CD32, 3, 10);
  colorXList[84].setEstado("Linen", 250, 240, 230, 0xFAF0E6, 4, 10);
  colorXList[85].setEstado("Magenta", 255, 0, 255, 0xFF00FF, 5, 10);
  colorXList[86].setEstado("Maroon", 128, 0, 0, 0x800000, 6, 10);
  colorXList[87].setEstado("MediumAquamarine", 102, 205, 170, 0x66CDAA, 7, 10);
  colorXList[88].setEstado("MediumBlue", 0, 0, 205, 0x0000CD, 0, 11);
  colorXList[89].setEstado("MediumOrchid", 186, 85, 211, 0xBA55D3, 1, 11);
  colorXList[90].setEstado("MediumPurple", 147, 112, 219, 0x9370DB, 2, 11);
  colorXList[91].setEstado("MediumSeaGreen", 60, 179, 113, 0x3CB371, 3, 11);
  colorXList[92].setEstado("MediumSlateBlue", 123, 104, 238, 0x7B68EE, 4, 11);
  colorXList[93].setEstado("MediumSpringGreen", 0, 250, 154, 0x00FA9A, 5, 11);
  colorXList[94].setEstado("MediumTurquoise", 72, 209, 204, 0x48D1CC, 6, 11);
  colorXList[95].setEstado("MediumVioletRed", 199, 21, 133, 0xC71585, 7, 11);
  colorXList[96].setEstado("MidnightBlue", 25, 25, 112, 0x191970, 0, 12);
  colorXList[97].setEstado("MintCream", 245, 255, 250, 0xF5FFFA, 1, 12);
  colorXList[98].setEstado("MistyRose", 255, 228, 225, 0xFFE4E1, 2, 12);
  colorXList[99].setEstado("Moccasin", 255, 228, 181, 0xFFE4B5, 3, 12);
  colorXList[100].setEstado("NavajoWhite", 255, 222, 173, 0xFFDEAD, 4, 12);
  colorXList[101].setEstado("Navy", 0, 0, 128, 0x000080, 5, 12);
  colorXList[102].setEstado("OldLace", 253, 245, 230, 0xFDF5E6, 6, 12);
  colorXList[103].setEstado("Olive", 128, 128, 0, 0x808000, 7, 12);
  colorXList[104].setEstado("OliveDrab", 107, 142, 35, 0x6B8E23, 0, 13);
  colorXList[105].setEstado("Orange", 255, 165, 0, 0xFFA500, 1, 13);
  colorXList[106].setEstado("OrangeRed", 255, 69, 0, 0xFF4500, 2, 13);
  colorXList[107].setEstado("Orchid", 218, 112, 214, 0xDA70D6, 3, 13);
  colorXList[108].setEstado("PaleGoldenrod", 238, 232, 170, 0xEEE8AA, 4, 13);
  colorXList[109].setEstado("PaleGreen", 152, 251, 152, 0x98FB98, 5, 13);
  colorXList[110].setEstado("PaleTurquoise", 175, 238, 238, 0xAFEEEE, 6, 13);
  colorXList[111].setEstado("PaleVioletRed", 219, 112, 147, 0xDB7093, 7, 13);

  colorXList[112].setEstado("PapayaWhip", 255, 239, 213, 0xFFEFD5, 0, 14);
  colorXList[113].setEstado("PeachPuff", 255, 218, 185, 0xFFDAB9, 1, 14);
  colorXList[114].setEstado("Peru", 205, 133, 63, 0xCD853F, 2, 14);
  colorXList[115].setEstado("Pink", 255, 192, 203, 0xFFC0CB, 3, 14);
  colorXList[116].setEstado("Plaid", 204, 85, 51, 0xCC5533, 4, 14);
  colorXList[117].setEstado("Plum", 221, 160, 221, 0xDDA0DD, 5, 14);
  colorXList[118].setEstado("PowderBlue", 176, 224, 230, 0xB0E0E6, 6, 14);
  colorXList[119].setEstado("Purple", 128, 0, 128, 0x800080, 7, 14);
}

void setup() {
  wifiSerial.begin(115200);  // Iniciamos puerto serial virtual
  Serial.begin(115200);      // Iniciamos puerto serial

  pinMode(START_BUTTON_PIN, INPUT_PULLUP);



  pinMode(pin_R, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_G, OUTPUT);

  tiempo1 = millis();
  cTiempo1 = millis();
  sTiempo1 = millis();
  
  Serial.println("Iniciado...");

  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  if (ID == 0xD3D3) ID = 0x9486;  // write-only shield
  tft.begin(ID);
  tft.setRotation(2);  //PORTRAIT

  drawMainScreen();
  cargarVecctorColores();
}

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

colorX currentColor = colorXList[8];
colorX cFrom;

long randomNumber, randomNumber2;
int sa1 = 0;
int sa2 = 180;
int cont = 0;

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
  if (sTiempo2 > sTiempo1 +20) {
    sTiempo1 = millis();

    if (sa1 == randomNumber && sa2 == randomNumber2) {
      randomNumber = random(120, 180);
      randomNumber2 = random(100, 180);
    } else {

      if (sa1 != randomNumber) {
        if (sa1 < randomNumber) sa1++;
        else sa1--;
      }

      if (sa2 != randomNumber2) {
        if (sa2 < randomNumber2) sa2++;
        else sa2--;
      }

      
    }
  }
}

void llamar(){
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
    //powerFaro = !powerFaro;
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
    Serial.println("efeto  4 " );
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

//Serial.print("-c color");

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
  Serial.println("send to wifi:d- ");
  Serial.print(destino);
  Serial.print("- p ");
  Serial.print(preCodigo);
  
  Serial.print("- c "  );
  Serial.println(code);

  String dataX = String(destino) + String(preCodigo) + String(code);
  Serial.println(dataX);
  Serial.println("--- "  );
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