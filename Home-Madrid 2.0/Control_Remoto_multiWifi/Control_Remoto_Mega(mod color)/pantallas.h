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


int colorSelG1 =0;
int colorSelG2 =30;
int colorSelG3 =60;
int colorSelG4 =90;
int colorSelG5 =120;

String destinoComando = "";
String currentScreen = "";
String previosScreen = "";

Adafruit_GFX_Button btn_PowerAll, btn_remotePower, btn_Habitacion, btn_Living, btn_Puertas;

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
  Serial.println("drawMainScreen...");
  tft.fillScreen(BLACK);
  showmsgXY(20, 15, 1, &FreeSans9pt7b, "Main - Home");


  tft.setFont(NULL);

  //btn_PowerAll.initButton(&tft, 60, 50, 110, 50, WHITE, RED, YELLOW, "Power Off", 1, 1);
  //btn_remotePower.initButton(&tft, 180, 50, 110, 50, WHITE, RED, YELLOW, "Control off", 1);

  btn_Habitacion.initButton(&tft, 60, 50, 110, 50, WHITE, RED, YELLOW, "Habitacion", 1);
  btn_Living.initButton(&tft, 180, 50, 110, 50, WHITE, RED, YELLOW, "Living", 1);

  //btn_Puertas.initButton(&tft, 60, 170, 110, 50, WHITE, RED, YELLOW, "Puertas", 1);

  // btn_PowerAll.drawButton(false);
  // btn_remotePower.drawButton(false);
  btn_Habitacion.drawButton(false);
  btn_Living.drawButton(false);
  //btn_Puertas.drawButton(false);


  previosScreen = "";
  currentScreen = "main";
}

Adafruit_GFX_Button btn_power, btn_colores, btn_Brillo100, btn_Brillo70, btn_Brillo40, btn_Brillo15;
Adafruit_GFX_Button btn_SelectEfecto1, btn_SelectEfecto2, btn_SelectEfecto3, btn_SelectEfecto4, btn_SelectEfecto5, btn_SelectEfecto6;
Adafruit_GFX_Button btn_Bola, btn_UV, btn_Audio, btn_volver;

void drawLivingScreen() {
  Serial.println("drawLivingScreen...");
  tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, ">>>>> Living  <<<<<");
  showmsgXY(20, 95, 1, &FreeSans9pt7b, "<< Brillo >>");
  showmsgXY(20, 170, 1, &FreeSans9pt7b, "<< Efecto >>");
  tft.setFont(NULL);

  //void initButton(Adafruit_GFX *gfx, int x, int y, int w, int h, int outline, int fill, int textcolor, char *label, int textsize);

  btn_power.initButton(&tft, 60, 50, 110, 40, WHITE, RED, YELLOW, "Power", 1);
  btn_colores.initButton(&tft, 180, 50, 110, 40, WHITE, RED, YELLOW, "Color", 1);


  btn_Brillo100.initButton(&tft, 30, 125, 50, 40, WHITE, RED, YELLOW, "100", 1);
  btn_Brillo70.initButton(&tft, 90, 125, 50, 40, WHITE, RED, YELLOW, "70", 1);
  btn_Brillo40.initButton(&tft, 150, 125, 50, 40, WHITE, RED, YELLOW, "40", 1);
  btn_Brillo15.initButton(&tft, 210, 125, 50, 40, WHITE, RED, YELLOW, "15", 1);


  btn_SelectEfecto1.initButton(&tft, 30, 200, 50, 40, WHITE, RED, YELLOW, "1", 1);
  btn_SelectEfecto2.initButton(&tft, 90, 200, 50, 40, WHITE, RED, YELLOW, "2", 1);
  btn_SelectEfecto3.initButton(&tft, 150, 200, 50, 40, WHITE, RED, YELLOW, "3", 1);
  btn_SelectEfecto4.initButton(&tft, 205, 200, 50, 40, WHITE, RED, YELLOW, "4", 1);

  btn_SelectEfecto5.initButton(&tft, 30, 245, 50, 40, WHITE, RED, YELLOW, "5", 1);
  btn_SelectEfecto6.initButton(&tft, 90, 245, 50, 40, WHITE, RED, YELLOW, "6", 1);

  btn_Bola.initButton(&tft, 150, 245, 50, 40, WHITE, RED, YELLOW, "Plasma", 1);
  btn_UV.initButton(&tft, 205, 245, 50, 40, WHITE, RED, YELLOW, "Ultra V.", 1);


  btn_Audio.initButton(&tft, 60, 300, 90, 40, WHITE, RED, YELLOW, "Audio", 1);

  btn_volver.initButton(&tft, 180, 300, 110, 40, WHITE, RED, YELLOW, "Volver", 1);

  btn_power.drawButton(false);
  btn_colores.drawButton(false);
  btn_Brillo100.drawButton(false);
  btn_Brillo70.drawButton(false);
  btn_Brillo40.drawButton(false);
  btn_Brillo15.drawButton(false);

  btn_SelectEfecto1.drawButton(false);
  btn_SelectEfecto2.drawButton(false);
  btn_SelectEfecto3.drawButton(false);
  btn_SelectEfecto4.drawButton(false);
  btn_SelectEfecto5.drawButton(false);
  btn_SelectEfecto6.drawButton(false);

  btn_Bola.drawButton(false);
  btn_UV.drawButton(false);
  btn_Audio.drawButton(false);
  btn_volver.drawButton(false);

  previosScreen = "";
  currentScreen = "living";
}

void drawHabitacionScreen() {
  Serial.println("drawHabitacionScreen...");

  tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, ">>>>> Habitacion <<<<<");
  showmsgXY(20, 95, 1, &FreeSans9pt7b, "<< Brillo >>");
  showmsgXY(20, 170, 1, &FreeSans9pt7b, "<< Efecto >>");

  tft.setFont(NULL);

  //void initButton(Adafruit_GFX *gfx, int x, int y, int w, int h, int outline, int fill, int textcolor, char *label, int textsize);

  btn_power.initButton(&tft, 60, 50, 110, 40, WHITE, RED, YELLOW, "Power", 1);
  btn_colores.initButton(&tft, 180, 50, 110, 40, WHITE, RED, YELLOW, "Color", 1);


  btn_Brillo100.initButton(&tft, 30, 125, 50, 40, WHITE, RED, YELLOW, "100", 1);
  btn_Brillo70.initButton(&tft, 90, 125, 50, 40, WHITE, RED, YELLOW, "70", 1);
  btn_Brillo40.initButton(&tft, 150, 125, 50, 40, WHITE, RED, YELLOW, "40", 1);
  btn_Brillo15.initButton(&tft, 210, 125, 50, 40, WHITE, RED, YELLOW, "15", 1);



  btn_SelectEfecto1.initButton(&tft, 30, 200, 50, 40, WHITE, RED, YELLOW, "1", 1);
  btn_SelectEfecto2.initButton(&tft, 90, 200, 50, 40, WHITE, RED, YELLOW, "2", 1);
  btn_SelectEfecto3.initButton(&tft, 150, 200, 50, 40, WHITE, RED, YELLOW, "3", 1);
  btn_SelectEfecto4.initButton(&tft, 205, 200, 50, 40, WHITE, RED, YELLOW, "4", 1);

  btn_SelectEfecto5.initButton(&tft, 30, 245, 50, 40, WHITE, RED, YELLOW, "5", 1);
  btn_SelectEfecto6.initButton(&tft, 90, 245, 50, 40, WHITE, RED, YELLOW, "6", 1);


  btn_volver.initButton(&tft, 180, 300, 110, 40, WHITE, RED, YELLOW, "Volver", 1);

  btn_power.drawButton(false);
  btn_colores.drawButton(false);
  btn_Brillo100.drawButton(false);
  btn_Brillo70.drawButton(false);
  btn_Brillo40.drawButton(false);
  btn_Brillo15.drawButton(false);

  btn_SelectEfecto1.drawButton(false);
  btn_SelectEfecto2.drawButton(false);
  btn_SelectEfecto3.drawButton(false);
  btn_SelectEfecto4.drawButton(false);
  btn_SelectEfecto5.drawButton(false);
  btn_SelectEfecto6.drawButton(false);

  btn_volver.drawButton(false);

  previosScreen = "";

  currentScreen = "habitacion";
}

void drawPuertasScreen() {
  tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, "Puertas");

  tft.setFont(&FreeSmallFont);

  btn_power.initButton(&tft, 60, 40, 110, 50, WHITE, CYAN, BLACK, "Power", 1);
  btn_colores.initButton(&tft, 180, 40, 110, 50, WHITE, CYAN, BLACK, "Naranja fuerte", 1);

  btn_Brillo100.initButton(&tft, 60, 40 * 2 + 10, 110, 50, WHITE, CYAN, BLACK, "Green", 1);
  btn_Brillo40.initButton(&tft, 60, 110, 110, 50, WHITE, RED, YELLOW, "40", 1);
  btn_Brillo70.initButton(&tft, 180, 40 * 2 + 10, 110, 50, WHITE, CYAN, BLACK, "Verde claro", 1);
  btn_Brillo15.initButton(&tft, 180, 110, 110, 50, WHITE, RED, YELLOW, "15", 1);

  btn_volver.initButton(&tft, 180, 40 * 6 + 10 * 6, 110, 50, WHITE, CYAN, BLACK, "Volver", 1);

  btn_power.drawButton(false);
  btn_colores.drawButton(false);
  btn_Brillo100.drawButton(false);
  btn_Brillo70.drawButton(false);

  //  btn_sinFuncion.drawButton(false);

  btn_volver.drawButton(false);

  previosScreen = "";
  currentScreen = "puertas";
}

Adafruit_GFX_Button btn_Red, btn_Naranja_fuerte, btn_Naranja_medio, btn_Naranja_debil, btn_Amarillo;
Adafruit_GFX_Button btn_Green, btn_Verde_claro, btn_Celeste, btn_Navy, btn_Verde_oscuro, btn_Blue;
Adafruit_GFX_Button btn_Azul_claro, btn_Marron, btn_Violeta, btn_Rosado, btn_White;


void drawColoresScreen(String previosS, String desComando) {
  Serial.println("drawHabitacionScreen...");

  tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, "Color fijo");

  tft.setFont(NULL);

  btn_SelectEfecto1.initButton(&tft, 20, 195, 40, 40, WHITE, RED, YELLOW, "1", 1);
  btn_SelectEfecto2.initButton(&tft, 60, 195, 40, 40, WHITE, RED, YELLOW, "2", 1);
  btn_SelectEfecto3.initButton(&tft, 100, 195, 40, 40, WHITE, RED, YELLOW, "3", 1);
  btn_SelectEfecto4.initButton(&tft, 140, 195, 40, 40, WHITE, RED, YELLOW, "4", 1);
  btn_SelectEfecto5.initButton(&tft, 180, 195, 40, 40, WHITE, RED, YELLOW, "5", 1);
  btn_SelectEfecto6.initButton(&tft, 220, 195, 40, 40, WHITE, RED, YELLOW, "6", 1);



  btn_Red.initButton(&tft, 30, 80, 40, 40, WHITE, c_Red, BLACK, "", 1);
  btn_Naranja_fuerte.initButton(&tft, 75, 80, 40, 40, WHITE, c_NaranjaFuerte, BLACK, "", 1);
  btn_Naranja_medio.initButton(&tft, 125, 80, 40, 40, WHITE, c_NaranjaMedio, BLACK, "", 1);
  btn_Naranja_debil.initButton(&tft, 170, 80, 40, 40, WHITE, c_NaranjaDebil, BLACK, "", 1);
  btn_Amarillo.initButton(&tft, 215, 80, 40, 40, WHITE, c_Amarillo, BLACK, "", 1);

  btn_Green.initButton(&tft, 30, 135, 40, 40, WHITE, c_Green, BLACK, "", 1);
  btn_Verde_claro.initButton(&tft, 75, 135, 40, 40, WHITE, c_VerdeClaro, BLACK, "", 1);
  btn_Celeste.initButton(&tft, 125, 135, 40, 40, WHITE, c_Celeste, BLACK, "", 1);
  btn_Navy.initButton(&tft, 170, 135, 40, 40, WHITE, c_Navy, BLACK, "", 1);
  btn_Verde_oscuro.initButton(&tft, 215, 135, 40, 40, WHITE, c_VerdeOscuro, BLACK, "", 1);

  btn_Blue.initButton(&tft, 30, 195, 40, 40, WHITE, c_Blue, BLACK, "", 1);
  btn_Azul_claro.initButton(&tft, 75, 195, 40, 40, WHITE, c_AzulClaro, BLACK, "", 1);
  btn_Marron.initButton(&tft, 125, 195, 40, 40, WHITE, c_Marron, BLACK, "", 1);
  btn_Violeta.initButton(&tft, 170, 195, 40, 40, WHITE, c_Violeta, BLACK, "", 1);
  btn_Rosado.initButton(&tft, 215, 195, 40, 40, WHITE, c_Rosado, BLACK, "", 1);

  btn_White.initButton(&tft, 70, 250, 70, 40, WHITE, c_White, BLACK, "", 1);

  btn_volver.initButton(&tft, 180, 250, 110, 40, WHITE, RED, YELLOW, "Volver", 1);

  btn_Red.drawButton(false);
  btn_Naranja_fuerte.drawButton(false);
  btn_Naranja_medio.drawButton(false);
  btn_Naranja_debil.drawButton(false);
  btn_Amarillo.drawButton(false);
  btn_Green.drawButton(false);
  btn_Verde_claro.drawButton(false);
  btn_Celeste.drawButton(false);
  btn_Navy.drawButton(false);
  btn_Verde_oscuro.drawButton(false);
  btn_Blue.drawButton(false);
  btn_Azul_claro.drawButton(false);
  btn_Marron.drawButton(false);
  btn_Violeta.drawButton(false);
  btn_Rosado.drawButton(false);
  btn_White.drawButton(false);

  btn_volver.drawButton(false);

  previosScreen = previosS;
  destinoComando = desComando;
  currentScreen = "color";
}

Adafruit_GFX_Button btn_backG1, btn_nextG1, btn_backG2, btn_nextG2, btn_backG3, btn_nextG3, btn_backG4, btn_nextG4, btn_backG5, btn_nextG5;
Adafruit_GFX_Button btn_select1, btn_select2, btn_select3, btn_select4, btn_select5, btn_aplicar;

void drawColoresScreen2(String previosS, String desComando) {
  Serial.println("drawHabitacionScreen...");

  tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, "Color fijo");

  tft.setFont(NULL);

  btn_SelectEfecto1.initButton(&tft, 20, 50, 40, 40, WHITE, RED, YELLOW, "1", 1);
  btn_SelectEfecto2.initButton(&tft, 60, 50, 40, 40, WHITE, RED, YELLOW, "2", 1);
  btn_SelectEfecto3.initButton(&tft, 100, 50, 40, 40, WHITE, RED, YELLOW, "3", 1);
  btn_SelectEfecto4.initButton(&tft, 140, 50, 40, 40, WHITE, RED, YELLOW, "4", 1);

  btn_select1.initButton(&tft, 60, 80, 100, 40, WHITE, RED, YELLOW, "color 1", 1);
  btn_select2.initButton(&tft, 60, 135, 100, 40, WHITE, RED, YELLOW, "color 2", 1);
  btn_select3.initButton(&tft, 60, 190, 100, 40, WHITE, RED, YELLOW, "color 3", 1);
  btn_select4.initButton(&tft, 60, 245, 100, 40, WHITE, RED, YELLOW, "color 4", 1);
  btn_select5.initButton(&tft, 60, 300, 100, 40, WHITE, RED, YELLOW, "color 5", 1);


  btn_backG1.initButton(&tft, 30, 80, 40, 40, WHITE, c_Red, BLACK, "<", 1);
  btn_nextG1.initButton(&tft, 215, 80, 40, 40, WHITE, c_Red, BLACK, ">", 1);

  btn_backG2.initButton(&tft, 30, 135, 40, 40, WHITE, c_Red, BLACK, "<", 1);
  btn_nextG2.initButton(&tft, 215, 135, 40, 40, WHITE, c_Red, BLACK, ">", 1);

  btn_backG3.initButton(&tft, 30, 190, 40, 40, WHITE, c_Red, BLACK, "<", 1);
  btn_nextG3.initButton(&tft, 215, 190, 40, 40, WHITE, c_Red, BLACK, ">", 1);

  btn_backG4.initButton(&tft, 30, 245, 40, 40, WHITE, c_Red, BLACK, "<", 1);
  btn_nextG4.initButton(&tft, 215, 245, 40, 40, WHITE, c_Red, BLACK, ">", 1);

  btn_backG5.initButton(&tft, 30, 300, 40, 40, WHITE, c_Red, BLACK, "<", 1);
  btn_nextG5.initButton(&tft, 215, 300, 40, 40, WHITE, c_Red, BLACK, ">", 1);

  btn_aplicar.initButton(&tft, 20, 250, 110, 40, WHITE, RED, YELLOW, "Aplicar", 1);
  btn_volver.initButton(&tft, 180, 250, 110, 40, WHITE, RED, YELLOW, "Volver", 1);


  btn_SelectEfecto1.drawButton(false);
  btn_SelectEfecto2.drawButton(false);
  btn_SelectEfecto3.drawButton(false);
  btn_SelectEfecto4.drawButton(false);

  btn_select1.drawButton(false);
  btn_select2.drawButton(false);
  btn_select3.drawButton(false);
  btn_select4.drawButton(false);
  btn_select5.drawButton(false);


  btn_backG1.drawButton(false);
  btn_nextG1.drawButton(false);
  btn_backG2.drawButton(false);
  btn_nextG2.drawButton(false);
  btn_backG3.drawButton(false);
  btn_nextG3.drawButton(false);
  btn_backG4.drawButton(false);
  btn_nextG4.drawButton(false);
  btn_backG5.drawButton(false);
  btn_nextG5.drawButton(false);

  btn_aplicar.drawButton(false);
  btn_volver.drawButton(false);

  previosScreen = previosS;
  destinoComando = desComando;
  currentScreen = "color";
}

unsigned long codigoColor[] = {0xF0F8FF, 0x9966CC, 0xFAEBD7, 0x00FFFF, 0x7FFFD4, 0xF0FFFF, 0xF5F5DC, 0xFFE4C4, 0x000000, 0xFFEBCD, 0x0000FF, 0x8A2BE2, 0xA52A2A, 0xDEB887, 0x5F9EA0, 0x7FFF00, 0xD2691E, 0xFF7F50, 0x6495ED, 0xFFF8DC, 0xDC143C, 0x00FFFF, 0x00008B, 0x008B8B, 0xB8860B, 0xA9A9A9, 0xA9A9A9, 0x006400, 0xBDB76B, 0x8B008B, 0x556B2F, 0xFF8C00, 0x9932CC, 0x8B0000, 0xE9967A, 0x8FBC8F, 0x483D8B, 0x2F4F4F, 0x2F4F4F, 0x00CED1, 0x9400D3, 0xFF1493, 0x00BFFF, 0x696969, 0x696969, 0x1E90FF, 0xB22222, 0xFFFAF0, 0x228B22, 0xFF00FF, 0xDCDCDC, 0xF8F8FF, 0xFFD700, 0xDAA520, 0x808080, 0x808080, 0x008000, 0xADFF2F, 0xF0FFF0, 0xFF69B4, 0xCD5C5C, 0x4B0082, 0xFFFFF0, 0xF0E68C, 0xE6E6FA, 0xFFF0F5, 0x7CFC00, 0xFFFACD, 0xADD8E6, 0xF08080, 0xE0FFFF, 0xFAFAD2, 0x90EE90, 0xD3D3D3, 0xFFB6C1, 0xFFA07A, 0x20B2AA, 0x87CEFA, 0x778899, 0x778899, 0xB0C4DE, 0xFFFFE0, 0x00FF00, 0x32CD32, 0xFAF0E6, 0xFF00FF, 0x800000, 0x66CDAA, 0x0000CD, 0xBA55D3, 0x9370DB, 0x3CB371, 0x7B68EE, 0x00FA9A, 0x48D1CC, 0xC71585, 0x191970, 0xF5FFFA, 0xFFE4E1, 0xFFE4B5, 0xFFDEAD, 0x000080, 0xFDF5E6, 0x808000, 0x6B8E23, 0xFFA500, 0xFF4500, 0xDA70D6, 0xEEE8AA, 0x98FB98, 0xAFEEEE, 0xDB7093, 0xFFEFD5, 0xFFDAB9, 0xCD853F, 0xFFC0CB, 0xCC5533, 0xDDA0DD, 0xB0E0E6, 0x800080, 0xFF0000, 0xBC8F8F, 0x4169E1, 0x8B4513, 0xFA8072, 0xF4A460, 0x2E8B57, 0xFFF5EE, 0xA0522D, 0xC0C0C0, 0x87CEEB, 0x6A5ACD, 0x708090, 0x708090, 0xFFFAFA, 0x00FF7F, 0x4682B4, 0xD2B48C, 0x008080, 0xD8BFD8, 0xFF6347, 0x40E0D0, 0xEE82EE, 0xF5DEB3, 0xFFFFFF, 0xF5F5F5, 0xFFFF00, 0x9ACD32, 0xFFE42D, 0xFF9D2A};
String nombreColor[] = {"AliceBlue", "Amethyst", "AntiqueWhite", "Aqua", "Aquamarine", "Azure", "Beige", "Bisque", "Black", "BlanchedAlmond", "Blue", "BlueViolet", "Brown", "BurlyWood", "CadetBlue", "Chartreuse", "Chocolate", "Coral", "CornflowerBlue", "Cornsilk", "Crimson", "Cyan", "DarkBlue", "DarkCyan", "DarkGoldenrod", "DarkGray", "DarkGrey", "DarkGreen", "DarkKhaki", "DarkMagenta", "DarkOliveGreen", "DarkOrange", "DarkOrchid", "DarkRed", "DarkSalmon", "DarkSeaGreen", "DarkSlateBlue", "DarkSlateGray", "DarkSlateGrey", "DarkTurquoise", "DarkViolet", "DeepPink", "DeepSkyBlue", "DimGray", "DimGrey", "DodgerBlue", "FireBrick", "FloralWhite", "ForestGreen", "Fuchsia", "Gainsboro", "GhostWhite", "Gold", "Goldenrod", "Gray", "Grey", "Green", "GreenYellow", "Honeydew", "HotPink", "IndianRed", "Indigo", "Ivory", "Khaki", "Lavender", "LavenderBlush", "LawnGreen", "LemonChiffon", "LightBlue", "LightCoral", "LightCyan", "LightGoldenrodYellow", "LightGreen", "LightGrey", "LightPink", "LightSalmon", "LightSeaGreen", "LightSkyBlue", "LightSlateGray", "LightSlateGrey", "LightSteelBlue", "LightYellow", "Lime", "LimeGreen", "Linen", "Magenta", "Maroon", "MediumAquamarine", "MediumBlue", "MediumOrchid", "MediumPurple", "MediumSeaGreen", "MediumSlateBlue", "MediumSpringGreen", "MediumTurquoise", "MediumVioletRed", "MidnightBlue", "MintCream", "MistyRose", "Moccasin", "NavajoWhite", "Navy", "OldLace", "Olive", "OliveDrab", "Orange", "OrangeRed", "Orchid", "PaleGoldenrod", "PaleGreen", "PaleTurquoise", "PaleVioletRed", "PapayaWhip", "PeachPuff", "Peru", "Pink", "Plaid", "Plum", "PowderBlue", "Purple", "Red", "RosyBrown", "RoyalBlue", "SaddleBrown", "Salmon", "SandyBrown", "SeaGreen", "Seashell", "Sienna", "Silver", "SkyBlue", "SlateBlue", "SlateGray", "SlateGrey", "Snow", "SpringGreen", "SteelBlue", "Tan", "Teal", "Thistle", "Tomato", "Turquoise", "Violet", "Wheat", "White", "WhiteSmoke", "Yellow", "YellowGreen", "FairyLight", "FairyLightNCC"};

