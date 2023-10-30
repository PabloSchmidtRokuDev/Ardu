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

String destinoComando = "";
String currentScreen = "";
String previosScreen = "";

Adafruit_GFX_Button btn_PowerAll, btn_remotePower, btn_Habitacion, btn_Living, btn_Barras, btn_Faros;

void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
  int16_t x1, y1;
  uint16_t wid, ht;
  tft.drawFastHLine(0, y + 2, tft.width(), WHITE);
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(GREEN);
  tft.setTextSize(sz);
  tft.print(msg);
}

void drawMainScreen()
{
  Serial.println("drawMainScreen...");
  tft.fillScreen(BLACK);
  showmsgXY(20, 15, 1, &FreeSans9pt7b, "Main - Home");


  tft.setFont(NULL);

  //btn_PowerAll.initButton(&tft, 60, 50, 110, 50, WHITE, RED, YELLOW, "Power Off", 1, 1);
  //btn_remotePower.initButton(&tft, 180, 50, 110, 50, WHITE, RED, YELLOW, "Control off", 1);

  btn_Habitacion.initButton(&tft, 60, 70, 110, 80, WHITE, RED, YELLOW, "Cuarto", 1);
  btn_Living.initButton(&tft, 180, 70, 110, 80, WHITE, RED, YELLOW, "Living", 1);

  btn_Barras.initButton(&tft, 60, 170, 110, 80, WHITE, RED, YELLOW, "Barras", 1);
  btn_Faros.initButton(&tft, 180, 170, 110, 80, WHITE, RED, YELLOW, "Faros", 1);

  // btn_PowerAll.drawButton(false);
  // btn_remotePower.drawButton(false);
  btn_Habitacion.drawButton(false);
  btn_Living.drawButton(false);
  btn_Barras.drawButton(false);
btn_Faros.drawButton(false);

  previosScreen = "";
  currentScreen = "main";
}

Adafruit_GFX_Button btn_power, btn_colores, btn_Brillo100, btn_Brillo70, btn_Brillo40, btn_Brillo15;
Adafruit_GFX_Button btn_SelectEfecto1, btn_SelectEfecto2, btn_SelectEfecto3, btn_SelectEfecto4, btn_SelectEfecto5, btn_SelectEfecto6;
Adafruit_GFX_Button btn_Bola, btn_UV, btn_Audio, btn_volver;

void drawLivingScreen()
{
  Serial.println("drawLivingScreen...");
 tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, ">>>>> Living  <<<<<");
  showmsgXY(20, 95, 1, &FreeSans9pt7b, "<< Brillo >>");
  showmsgXY(20, 170, 1, &FreeSans9pt7b, "<< Efecto >>");
  tft.setFont(NULL);

//void initButton(Adafruit_GFX *gfx, int x, int y, int w, int h, int outline, int fill, int textcolor, char *label, int textsize);

  btn_power.initButton(&tft,    60, 50, 110, 40, WHITE, RED, YELLOW, "Power", 1);
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

  btn_Bola.initButton(&tft,   150, 245, 50, 40, WHITE, RED, YELLOW, "Plasma", 1);
  btn_UV.initButton(&tft,    205, 245, 50, 40, WHITE, RED, YELLOW, "Ultra V.", 1);


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

void drawHabitacionScreen()
{
  Serial.println("drawHabitacionScreen...");

  tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, ">>>>> Habitacion <<<<<");
  showmsgXY(20, 95, 1, &FreeSans9pt7b, "<< Brillo >>");
  showmsgXY(20, 170, 1, &FreeSans9pt7b, "<< Efecto >>");
  
  tft.setFont(NULL);

//void initButton(Adafruit_GFX *gfx, int x, int y, int w, int h, int outline, int fill, int textcolor, char *label, int textsize);

  btn_power.initButton(&tft,    60, 50, 110, 40, WHITE, RED, YELLOW, "Power", 1);
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

void drawBarrasScreen()
{
  Serial.println("drawBarrasScreen...");

  tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, ">>>>> Barras Screen <<<<<");
  showmsgXY(20, 95, 1, &FreeSans9pt7b, "<< Brillo >>");
  showmsgXY(20, 170, 1, &FreeSans9pt7b, "<< Efecto >>");
  
  tft.setFont(NULL);

//void initButton(Adafruit_GFX *gfx, int x, int y, int w, int h, int outline, int fill, int textcolor, char *label, int textsize);

  btn_power.initButton(&tft,    60, 50, 110, 40, WHITE, RED, YELLOW, "Power", 1);
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
  
  currentScreen = "barras";
}

void drawFarosScreen()
{
  Serial.println("drawFarosScreen...");

  tft.fillScreen(BLACK);

  showmsgXY(20, 15, 1, &FreeSans9pt7b, ">>>>> Faros Screen <<<<<");
  showmsgXY(20, 95, 1, &FreeSans9pt7b, "<< Brillo >>");
  showmsgXY(20, 170, 1, &FreeSans9pt7b, "<< Efecto >>");
  
  tft.setFont(NULL);

//void initButton(Adafruit_GFX *gfx, int x, int y, int w, int h, int outline, int fill, int textcolor, char *label, int textsize);

  btn_power.initButton(&tft,    60, 50, 110, 40, WHITE, RED, YELLOW, "Power", 1);
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
  
  currentScreen = "faros";
}

void drawPuertasScreen()
{
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

void drawColoresScreen(String previosS, String desComando){
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



  btn_Red.initButton(&tft,                30, 80, 40, 40, WHITE, c_Red, BLACK, "", 1);
  btn_Naranja_fuerte.initButton(&tft,     75, 80, 40, 40, WHITE, c_NaranjaFuerte, BLACK, "", 1);
  btn_Naranja_medio.initButton(&tft,      125, 80, 40, 40, WHITE, c_NaranjaMedio, BLACK, "", 1);
  btn_Naranja_debil.initButton(&tft,      170, 80, 40, 40, WHITE, c_NaranjaDebil, BLACK, "", 1);
  btn_Amarillo.initButton(&tft,           215, 80, 40, 40, WHITE, c_Amarillo, BLACK, "", 1);

  btn_Green.initButton(&tft,              30 , 135, 40, 40, WHITE, c_Green, BLACK, "", 1);
  btn_Verde_claro.initButton(&tft,        75,  135, 40, 40, WHITE, c_VerdeClaro, BLACK, "", 1);
  btn_Celeste.initButton(&tft,            125, 135, 40, 40, WHITE, c_Celeste, BLACK, "", 1);
  btn_Navy.initButton(&tft,               170, 135, 40, 40, WHITE, c_Navy, BLACK, "", 1);
  btn_Verde_oscuro.initButton(&tft,       215, 135, 40, 40, WHITE, c_VerdeOscuro, BLACK, "", 1);

  btn_Blue.initButton(&tft,               30 , 195, 40, 40, WHITE, c_Blue, BLACK, "", 1);
  btn_Azul_claro.initButton(&tft,         75,  195, 40, 40, WHITE, c_AzulClaro, BLACK, "", 1);
  btn_Marron.initButton(&tft,             125, 195, 40, 40, WHITE, c_Marron, BLACK, "", 1);
  btn_Violeta.initButton(&tft,            170, 195, 40, 40, WHITE, c_Violeta, BLACK, "", 1);
  btn_Rosado.initButton(&tft,             215, 195, 40, 40, WHITE, c_Rosado, BLACK, "", 1);

  btn_White.initButton(&tft,              70, 250, 70, 40, WHITE, c_White, BLACK, "", 1);

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