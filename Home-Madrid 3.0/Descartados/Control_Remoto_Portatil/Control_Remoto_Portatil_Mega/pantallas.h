#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF


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

String currentScreen = "";

Adafruit_GFX_Button btn1,btn2, btn3, btn4,btn5,btn6,btn7, btn8, btn9,btn10;//,btn11,btn12,btn13,btn14, btn15;

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

void drawMainScreen() {
  Serial.println("drawLivingScreen...");
  tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, ">>>>> Living  <<<<<");
  showmsgXY(20, 95, 1, &FreeSans9pt7b, "<< Brillo >>");
  showmsgXY(20, 170, 1, &FreeSans9pt7b, "<< Efecto >>");
  tft.setFont(NULL);

  btn1.initButton(&tft, 60, 50, 110, 40, WHITE, RED, YELLOW, "Power", 1);
  btn2.initButton(&tft, 180, 50, 110, 40, WHITE, RED, YELLOW, "Color", 1);

  btn3.initButton(&tft, 30, 125, 50, 40, WHITE, RED, YELLOW, "100", 1);
  btn4.initButton(&tft, 90, 125, 50, 40, WHITE, RED, YELLOW, "70", 1);
  btn5.initButton(&tft, 150, 125, 50, 40, WHITE, RED, YELLOW, "40", 1);

  btn7.initButton(&tft, 30, 200, 50, 40, WHITE, RED, YELLOW, "1", 1);
  btn8.initButton(&tft, 90, 200, 50, 40, WHITE, RED, YELLOW, "2", 1);
  btn9.initButton(&tft, 150, 200, 50, 40, WHITE, RED, YELLOW, "3", 1);
  btn10.initButton(&tft, 205, 200, 50, 40, WHITE, RED, YELLOW, "4", 1);

  // btn11.initButton(&tft, 30, 245, 50, 40, WHITE, RED, YELLOW, "5", 1);
  // btn12.initButton(&tft, 90, 245, 50, 40, WHITE, RED, YELLOW, "6", 1);

  // btn13.initButton(&tft, 150, 245, 50, 40, WHITE, RED, YELLOW, "Plasma", 1);
  // btn14.initButton(&tft, 205, 245, 50, 40, WHITE, RED, YELLOW, "Ultra V.", 1);
  // btn15.initButton(&tft, 60, 300, 90, 40, WHITE, RED, YELLOW, "Audio", 1);

  btn1.drawButton(false);
  btn2.drawButton(false);
  btn3.drawButton(false);
  btn4.drawButton(false);
  btn5.drawButton(false);

  btn7.drawButton(false);
  btn8.drawButton(false);
  btn9.drawButton(false);
  btn10.drawButton(false);
  // btn11.drawButton(false);
  // btn12.drawButton(false);

  // btn13.drawButton(false);
  // btn14.drawButton(false);
  // btn15.drawButton(false);

  currentScreen = "living";
}

int ypos = 0;
int getYpos() {
  ypos += 43;
  return ypos;
}
void drawColoresScreen() {
  Serial.println("drawHabitacionScreen...");

  tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, "Color fijo");

  tft.setFont(NULL);

  ypos = 0;

  btn1.initButton(&tft, 50, getYpos(), 100, 40, WHITE, c_Red, BLACK, "Rojo -Naranja", 1);
  btn2.initButton(&tft, 50, getYpos(), 100, 40, WHITE, c_NaranjaFuerte, BLACK, "Naranja - Amarillo", 1);
  btn3.initButton(&tft, 50, getYpos(), 100, 40, WHITE, c_NaranjaMedio, BLACK, "Amarillo - Verde claro", 1);
  btn4.initButton(&tft, 50, getYpos(), 100, 40, WHITE, c_NaranjaDebil, BLACK, "Verde  claro - Verde", 1);
  btn5.initButton(&tft, 50, getYpos(), 100, 40, WHITE, c_Amarillo, BLACK, "Verde - Azulado", 1);

  ypos = 0;

  btn6.initButton(&tft, 170, getYpos(), 100, 40, WHITE, c_Green, BLACK, "Azulado - Azul Claro", 1);
  btn7.initButton(&tft, 170, getYpos(), 100, 40, WHITE, c_VerdeClaro, BLACK, "Azul Claro - Azul", 1);
  btn8.initButton(&tft, 170, getYpos(), 100, 40, WHITE, c_Celeste, BLACK, "Azul - Violeta", 1);
  btn9.initButton(&tft, 170, getYpos(), 100, 40, WHITE, c_Navy, BLACK, "Violeta - Rosita", 1);
  btn10.initButton(&tft, 170, getYpos(), 100, 40, WHITE, c_VerdeOscuro, BLACK, "Rosita - Rosa", 1);


  // btn11.initButton(&tft, 180, 270, 110, 40, WHITE, RED, YELLOW, "Volver", 1);

  btn1.drawButton(false);
  btn2.drawButton(false);
  btn3.drawButton(false);
  btn4.drawButton(false);
  btn5.drawButton(false);
  btn6.drawButton(false);
  btn7.drawButton(false);
  btn8.drawButton(false);
  btn9.drawButton(false);
  btn10.drawButton(false);
  // btn11.drawButton(false);

  currentScreen = "color";
}