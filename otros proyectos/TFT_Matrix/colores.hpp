byte red = 31;
byte green = 0;
byte blue = 0;

byte state = 0;
unsigned int colour = red << 11;  // Colour order is RGB 5+6+5 bits each

int bordeTop = 10;
int bordeLeft = 10;

int anchoColor = 40;
int anchoBrillo = 40;

int sepObject = 30;
int anchoEfectos = 40;

int bordeLeftBrillo = bordeLeft + anchoColor + sepObject;
int bordeLeftEfecto = bordeLeft + anchoColor + anchoBrillo + sepObject + sepObject;
int bordeTopEfecto = bordeTop + 100 + sepObject;

class colorX {
public:
  //ConexionDispocitivo();
  //ConexionDispocitivo(String _dispocitivoID);
  bool select = false;
  uint16_t colorHEX;
  int r;
  int g;
  int b;
  String nombre;

  int brillo = -1;
  int posX;
  int posY;

  void setSelect(bool _select);
  void setEstado(String _nombre, int _r, int _g, int _b, uint16_t _colorHex, int _posX, int _posY);
};

void colorX::setSelect(bool _select) {
  select = _select;

  tft.fillRect(posX + bordeLeft +anchoColor, posY + bordeTop, 15, 15, TFT_BLACK);
  if (select == true) {
    tft.fillRect(posX + bordeLeft + anchoColor, posY + bordeTop, 15, 15, colorHEX);
    tft.drawRect(posX + bordeLeft + anchoColor, posY + bordeTop, 15, 15, TFT_WHITE);
  }
}

void colorX::setEstado(String _nombre, int _r, int _g, int _b, uint16_t _colorHex, int _posX, int _posY) {
  nombre = _nombre;
  r = map(_r, 0, 63, 0, 255);
  g = map(_g, 0, 63, 0, 255);
  b = map(_b, 0, 63, 0, 255);
  colorHEX = _colorHex;
  posX = _posX;
  posY = _posY;

  setSelect(false);
};

const int maxColors = 550;
colorX colorXList[maxColors];  // An array of MyClass objects using default constructor

void rainbow_fill() {
  // The colours and state are not initialised so the start colour changes each time the funtion is called

  for (int i = 0; i < 255; i++) {
    // Draw a vertical line 1 pixel wide in the selected colour
    tft.drawFastHLine(bordeLeft, bordeTop + i, anchoColor, colour);  // in this example tft.width() returns the pixel width of the display

    colorXList[i].setEstado("c:" + String(colour), red, green, blue, colour, bordeLeft, i);
    // This is a "state machine" that ramps up/down the colour brightnesses in sequence
    switch (state) {
      case 0:
        green++;
        if (green < 15) { blue++; }

        if (green == 64) {
          green = 63;
          state = 1;
        }
        break;

      case 1:
        red--;

        if (red < 15) { green--; }

        if (red == 255) {
          red = 0;
          state = 2;
        }
        break;

      case 2:
        blue++;
        if (blue < 15) { green++; }

        if (blue == 32) {
          blue = 31;
          state = 3;
        }
        break;

      case 3:
        green--;
        if (green < 15) { red++; }

        if (green == 255) {
          green = 0;
          state = 4;
        }
        break;

      case 4:
        red++;

        if (red == 32) {
          red = 31;
          state = 5;
        }
        break;
      case 5:
        blue--;

        if (blue < 15) { green++; }

        if (blue == 255) {
          blue = 0;
          state = 0;
        }
        break;
    }

    colour = red << 11 | green << 5 | blue;
  }
}

int indexSelColor = 0;
int brillo = 30;
int seletOp = 1;  //0: cambio color. 1: cambio brillo 2:efecto

void setBrilloPos() {

  tft.fillRect(bordeLeftBrillo + 1, bordeTop + 1, anchoBrillo - 2, 101 - brillo, TFT_BLACK);
  tft.fillRect(bordeLeftBrillo + 1, bordeTop + 99 - brillo + 1, anchoBrillo - 2, brillo + 1, TFT_WHITE);
  // tft.drawRect(205, brillo, 15, 4, TFT_WHITE);
}

enum direccionEum {
  dir_rigth,
  dir_left,
  dir_up,
  dir_down
};


bool selectObj(direccionEum direccion) {
  bool flag = false;
  if (direccion == dir_rigth && seletOp < 2) {
    seletOp++;
    flag =true;
  } else if (direccion == dir_left && seletOp > 0) {
    seletOp--;
    flag =true;
  }

  if (seletOp == 0 && flag) {
    tft.drawRect(bordeLeft, bordeTop, anchoColor, 255, TFT_GREEN);
    tft.drawRect(bordeLeftBrillo, bordeTop, anchoBrillo, 100, TFT_WHITE);
    tft.drawRect(bordeLeftEfecto, bordeTop, anchoEfectos, 100, TFT_WHITE);
delay(1000);
    return false;

  } else if (seletOp == 1 && flag) {
    tft.drawRect(bordeLeft, bordeTop, anchoColor, 255, TFT_WHITE);
    tft.drawRect(bordeLeftBrillo, bordeTop, anchoBrillo, 100, TFT_GREEN);
    tft.drawRect(bordeLeftEfecto, bordeTop, anchoEfectos, 100, TFT_WHITE);
delay(1000);
    return false;

  } else if (seletOp == 2 && flag) {
    tft.drawRect(bordeLeft, bordeTop, anchoColor, 255, TFT_WHITE);
    tft.drawRect(bordeLeftBrillo, bordeTop, anchoBrillo, 100, TFT_WHITE);
    tft.drawRect(bordeLeftEfecto, bordeTop, anchoEfectos, 100, TFT_GREEN);
delay(1000);
    return false;
  }

  return true;
}

void crearPantallaColores() {
  tft.fillScreen(TFT_BLACK);

  rainbow_fill();
  selectObj(dir_left);

  for (int i = 0; i < 112; i++) {
    colorXList[i].setSelect(false);
  }

  colorXList[0].setSelect(true);
  setBrilloPos();
  indexSelColor = 0;
  
}


colorX getColoreX(direccionEum direccion) {

  colorX colorXsel = colorXList[indexSelColor];

  if (direccion == dir_up) {
    if (colorXsel.posY > 0) {
      colorXsel.setSelect(false);
      indexSelColor--;
    }

  } else if (direccion == dir_down) {
    if (colorXsel.posY < 254) {
      colorXsel.setSelect(false);
      indexSelColor++;
    }
  }

  colorXsel = colorXList[indexSelColor];
  colorXsel.setSelect(true);
  return colorXsel;
}

colorX getBrillo(direccionEum direccion) {

  if (direccion == dir_up) {
    brillo++;
    if (brillo > 100) brillo = 100;

  } else if (direccion == dir_down) {
    brillo--;
    if (brillo < 0) brillo = 0;
  }

  setBrilloPos();
  colorX colorXselB;
  colorXselB.brillo = brillo;
  return colorXselB;
}

colorX getEfecto(direccionEum direccion) {

    colorX colorXselB;
    colorXselB.brillo = brillo;
    return colorXselB;
}

colorX getColores(direccionEum direccion) {
  if (selectObj(direccion)) {
    if (seletOp == 0) {
      return getColoreX(direccion);

    } else if (seletOp == 1) {
      return getBrillo(direccion);

    } else if (seletOp == 2) {
      return getEfecto(direccion);
    }
  }
}
