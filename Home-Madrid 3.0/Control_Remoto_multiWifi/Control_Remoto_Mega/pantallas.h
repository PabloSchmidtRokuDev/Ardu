#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GRAY 0x8410

#define c_Red 0xff0000
#define c_NaranjaFuerte 0xFF4500
#define c_NaranjaMedio 0xFA8072
#define c_NaranjaDebil 0xFFA07A
#define c_Amarillo 0xFFFFE0
#define c_Green 0x32CD32
#define c_VerdeClaro 0x90EE90
#define c_Celeste 0x5F9EA0
#define c_Navy 0xFF7F50
#define c_VerdeOscuro 0x006400
#define c_Blue 0x0000FF
#define c_AzulClaro 0x0000CD
#define c_Marron 0x800000
#define c_Violeta 0xEE82EE
#define c_Rosado 0xFFC0CB
#define c_White 0xF8F8FF
#define c_DarkGray 0xA9A9A9
#define c_LightSlateGray 0x778899
#define c_Gray 0x808080
/*
class botonX {
public:

  botonX(String Text, int x1, int y1, int x2, int y2, uint16_t cFont, uint16_t cBG, uint16_t cBorder, uint16_t cSelect);
  String _Text;
  int _x1;
  int _y1;
  int _x2;
  int _y2;
  uint16_t _cFont;
  uint16_t _cBG;
  uint16_t _cBorder;
  uint16_t _cSelect;

  void Draw();
}

botonX::botonX(String Text, int x1, int y1, int x2, int y2, uint16_t cFont, uint16_t cBG, uint16_t cBorder, uint16_t cSelect) {
  _Text = Text;
  _x1 = x1;
  _y1 = y1;
  _x2 = x2;
  _y2 = y2;
  _cFont = cFont;
  _cBG = cBG;
  _cBorder = cBorder;
  _cSelect = cSelect;
}

void botonX::Draw() {
  //tft.setColor(_cBorder);
  //tft.setBackColor(_cBG);
  // tft.drawRoundRect(_x1, _y1, _x2, _y2);
  // tft.print(_Text, _x1 +10, _y1 +10)
}
*/

Adafruit_GFX_Button btn_PowerAll, btn_powerFinChill, btn_Abrir_Entrada, btn_powerCirculos, btn_power_ML, btn_brilloCirculosMas, btn_brilloML, btn_power_UV, btn_nextEfeto, btn_brilloCirculosMenos, btn_NextColor;


void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg) {
  int16_t x1, y1;
  uint16_t wid, ht;
  tft.drawFastHLine(0, y + 2, tft.width(), WHITE);
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(GREEN);
  tft.setTextSize(sz);
  tft.print(msg);
}

int MARGIN_LEFT_1 = 80;
int MARGIN_LEFT_2 = 250;
int MARGIN_LEFT_3 = 400;


int SPACE_OBJ = 20;

int MARGIN_TOP1 = 60;

int MARGIN_TOP = 40;

int BTN_W1 = 160;
int BTN_H1 = 80;

int BTN_W2 = 135;
int BTN_H2 = 55;

#include "icon/entrada.h"

void drawMainScreen() {
  Serial.println("drawMainScreen...");
  tft.fillScreen(BLACK);

  tft.drawFastVLine(169, 0, tft.height(), WHITE);
  tft.drawFastVLine(171, 0, tft.height(), WHITE);
  tft.drawFastVLine(325, 0, tft.height(), WHITE);
     
  tft.setFont(&FreeMono9pt7b);
  tft.setTextSize(1);

  btn_PowerAll.initButton(&tft, MARGIN_LEFT_1, MARGIN_TOP1, BTN_W1, BTN_H1, BLUE, BLACK, WHITE, "Power Off", 1);
  btn_powerFinChill.initButton(&tft, MARGIN_LEFT_1, MARGIN_TOP1 + BTN_H1 + SPACE_OBJ, BTN_W1, BTN_H1, BLUE, BLACK, WHITE, "End Chill", 1);
  btn_Abrir_Entrada.initButton(&tft, MARGIN_LEFT_1, MARGIN_TOP1 + BTN_H1 * 2 + SPACE_OBJ * 2, BTN_W1, BTN_H1, BLUE, GREEN, BLACK, "Abrir Puerta", 1);

  btn_powerCirculos.initButton(&tft, MARGIN_LEFT_2, MARGIN_TOP, BTN_W2, BTN_H2, BLUE, BLACK, WHITE, "Power Circulos", 1);
  btn_power_ML.initButton(&tft, MARGIN_LEFT_2, MARGIN_TOP + SPACE_OBJ + BTN_H2, BTN_W2, BTN_H2, BLUE, BLACK, WHITE, "Power ML", 1);
  btn_power_UV.initButton(&tft, MARGIN_LEFT_2, MARGIN_TOP + SPACE_OBJ * 2 + BTN_H2 * 2, BTN_W2, BTN_H2, BLUE, BLACK, WHITE, "Power UV", 1);
  btn_nextEfeto.initButton(&tft, MARGIN_LEFT_2, MARGIN_TOP + SPACE_OBJ * 3 + BTN_H2 * 3, BTN_W2, BTN_H2, BLUE, BLACK, WHITE, "Efecto", 1);

  btn_brilloML.initButton(&tft, MARGIN_LEFT_3, MARGIN_TOP, BTN_W2, BTN_H2, BLUE, BLACK, WHITE, "Brillo ML", 1);
  btn_brilloCirculosMas.initButton(&tft, MARGIN_LEFT_3, MARGIN_TOP + SPACE_OBJ + BTN_H2, BTN_W2, BTN_H2, BLUE, BLACK, WHITE, "B Cirulos +", 1);
  btn_brilloCirculosMenos.initButton(&tft, MARGIN_LEFT_3, MARGIN_TOP + SPACE_OBJ * 2 + BTN_H2 * 2, BTN_W2, BTN_H2, BLUE, BLACK, WHITE, "B Cirulos -", 1);
  btn_NextColor.initButton(&tft, MARGIN_LEFT_3, MARGIN_TOP + SPACE_OBJ * 3 + BTN_H2 * 3, BTN_W2, BTN_H2, BLUE, BLACK, WHITE, "Color", 1);

  btn_PowerAll.drawButton(false);
  btn_powerFinChill.drawButton(false);
  btn_Abrir_Entrada.drawButton(false);
  btn_powerCirculos.drawButton(false);
  btn_power_ML.drawButton(false);
  btn_brilloCirculosMas.drawButton(false);
  btn_brilloML.drawButton(false);
  btn_power_UV.drawButton(false);
  btn_nextEfeto.drawButton(false);
  btn_brilloCirculosMenos.drawButton(false);
  btn_NextColor.drawButton(false);
}
