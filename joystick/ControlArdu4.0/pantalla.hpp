#include <Arduino.h>

int getPosY(int linea) {
  if (linea == 1) return 10;
  if (linea == 2) return 26;
  if (linea == 3) return 42;
  if (linea == 4) return 58;
  if (linea == 5) return 74;
  if (linea == 6) return 90;
  if (linea == 7) return 106;
  if (linea == 8) return 122;
  if (linea == 9) return 138;
  if (linea == 10) return 154;
  if (linea == 11) return 170;
  if (linea == 12) return 186;
  if (linea == 13) return 202;
  if (linea == 14) return 218;

  return 10;
}

int getPosX(int col) {
  if (col == 15) return xpos + 126;
  if (col == 14) return xpos + 116;
  if (col == 13) return xpos + 116;
  if (col == 12) return xpos + 108;
  if (col == 11) return xpos + 97;
  if (col == 10) return xpos + 89;
  if (col == 9) return xpos + 80;
  if (col == 8) return xpos + 71;
  if (col == 7) return xpos + 63;
  if (col == 6) return xpos + 54;
  if (col == 5) return xpos + 44;
  if (col == 4) return xpos + 36;
  if (col == 3) return xpos + 26;
  if (col == 2) return xpos + 18;
  if (col == 1) return xpos + 7;

  return xpos;
}

void crearPantallaColorx(){

//drawRect(int16_t x, int16_t y, int16_t w, int16_t h,  uint16_t color); 
tft.fillScreen(TFT_BLACK);

for (int i= 0; i<8;i++){
  for (int j= 0; j<14;j++){
  tft.fillRect(16*i + 4, 16*j +4, 15, 15, TFT_YELLOW);
  //fillRect
  //drawRect
}
}

}

void setTexto(String txt, int linea, String alin) {
  if (alin == "right") {
    int textLen = txt.length();
    int x = getPosX(0);
    if (textLen < 14) x = getPosX(14 - textLen);

    tft.setCursor(x, getPosY(linea));
    spr.printToSprite(txt);

  } else if (alin == "center") {
    int textLen = txt.length();
    Serial.println("len: " + String(textLen));
    int x = getPosX(0);
    if (textLen < 14) {
      x = 14 - textLen;
      Serial.println("x-14: " + String(x));
      x = (x / 2);
      Serial.println("textLen / 2: " + String(x));
      x = getPosX(x);
      Serial.println("posX: " + String(x));
    }

    tft.setCursor(x, getPosY(linea));
    spr.printToSprite(txt);

  } else {
    tft.setCursor(getPosX(0), getPosY(linea));
    spr.printToSprite(txt);
  }
}

void setTexto(String txt, int linea, int tab) {

  tft.setCursor(getPosX(tab), getPosY(linea));
  spr.printToSprite(txt);
}

void drawLineSub(int linea) {
linea++;  
  tft.drawLine(0, getPosY(linea) + 1, 126, getPosY(linea) + 1, TFT_RED);
}


int opcionSelect = 0;
int subOpcionSelect = 0;

void updateMenu(int opSelect) {
  if (opSelect == 0) {
    spr.setTextColor(TFT_BLACK, TFT_GREEN);
    subOpcionSelect = 0;
  } else {
    spr.setTextColor(TFT_YELLOW, TFT_BLACK);
  }
  setTexto("Luz 01  ", 4, 1);

  if (opSelect == 1) {
    spr.setTextColor(TFT_BLACK, TFT_GREEN);
    subOpcionSelect = 1;
  } else {
    spr.setTextColor(TFT_YELLOW, TFT_BLACK);
  }

  setTexto("Soporte Camara  ", 5, 1);

  if (opSelect == 2) {
    spr.setTextColor(TFT_BLACK, TFT_GREEN);
    subOpcionSelect = 2;
  } else {
    spr.setTextColor(TFT_YELLOW, TFT_BLACK);
  }
  setTexto("Brazo Rojo 01  ", 6, 1);

  if (opSelect == 3) {
    spr.setTextColor(TFT_BLACK, TFT_GREEN);
    subOpcionSelect = 3;
  } else {
    spr.setTextColor(TFT_YELLOW, TFT_BLACK);
  }
  setTexto("Brazo Rojo 02  ", 7, 1);
}

void crearMenu() {
  tft.fillScreen(TFT_BLACK);
  spr.setTextColor(TFT_YELLOW, TFT_BLACK);
  setTexto("Menu", 2, "center");
  drawLineSub(2);
  opcionSelect = 0;
  updateMenu(0);
}

void updatePantallaLuz01(String obj, String value) {

  if (obj == "cnn_lbl") setTexto("Conectando..       ", 4, "left");
  if (obj == "cnn_puntos") setTexto(value, 5, "left");

  if (obj == "error") {

    tft.fillScreen(TFT_RED);
    setTexto("Luz 01", 2, "center");
    setTexto("Error!!!", 6, "center");
    setTexto(value, 7, "center");
    setTexto("Cerrar", 15, "left");
    subOpcionSelect = 1;
  }

  if (obj == "cnn_exito") {
    setTexto("Conectado  :)         ", 4, "left");
    setTexto("                              ", 5, "left");

    subOpcionSelect = 1;
  }

  if (obj == "ejeX") {
    setTexto("  EjeX: " + value, 6, "left");
  }

  if (obj == "ejeY") {
    setTexto("  EjeY: " + value, 7, "left");
  }

  if (obj == "sendData") {
    setTexto("SD: '" + value + "'                  ", 9, "left");
  }

  if (obj == "sleepIn") {
    setTexto("sleep in: " + value, 12, "rigth");
  }
}

void crearPantallaLuz01() {
  tft.fillScreen(TFT_BLACK);
  spr.setTextColor(TFT_YELLOW, TFT_BLACK);
  setTexto("Luz 01", 2, "center");
drawLineSub(2);
  setTexto("Cerrar", 14, "left");
  subOpcionSelect = 0;
}



void updatePantallaSoporte(String obj, String value) {

  if (obj == "cnn_lbl") setTexto("Conectando..       ", 3, "left");
  if (obj == "cnn_puntos") setTexto(value, 4, "left");

  if (obj == "error") {

    tft.fillScreen(TFT_RED);
    setTexto("Luz 01", 2, "center");
    setTexto("Error!!!", 6, "center");
    setTexto(value, 7, "center");
    setTexto("Cerrar", 15, "left");
    subOpcionSelect = 1;
  }

  if (obj == "cnn_exito") {
    setTexto("Conectado  :)         ", 3, "left");
    setTexto("                              ", 4, "left");

    updatePantallaSoporte("Ejes1y2", "");
  }

  if (obj == "ejeX") {
    setTexto("  EjeX: " + value, 4, "left");
  }

  if (obj == "ejeY") {
    setTexto("  EjeY: " + value, 5, "left");
  }

  if (obj == "Ejes1y2") {
    spr.setTextColor(TFT_BLACK, TFT_GREEN);
    setTexto("> Ejes 1 y 2 <", 6, 2);
    subOpcionSelect = 1;
    spr.setTextColor(TFT_YELLOW, TFT_BLACK);
    setTexto("> Ejes 3 y 4 <", 7, 2);
    setTexto("> Ejes 5 y 6 <", 8, 2);
  }

  if (obj == "Ejes3y4") {
    spr.setTextColor(TFT_BLACK, TFT_GREEN);

    setTexto("> Ejes 3 y 4 <", 7, 2);
    subOpcionSelect = 2;
    spr.setTextColor(TFT_YELLOW, TFT_BLACK);
    setTexto("> Ejes 1 y 2 <", 6, 2);
    setTexto("> Ejes 5 y 6 <", 8, 2);
  }

  if (obj == "Ejes5y6") {
    spr.setTextColor(TFT_BLACK, TFT_GREEN);
    setTexto("> Ejes 5 y 6 <", 8, 2);
    subOpcionSelect = 3;
    spr.setTextColor(TFT_YELLOW, TFT_BLACK);
    setTexto("> Ejes 3 y 4 <", 7, 2);
    setTexto("> Ejes 1 y 2 <", 6, 2);
  }


  if (obj == "sendData") {
    setTexto("SD: '" + value + "'                  ", 12, "left");
  }

  if (obj == "sleepIn") {
    setTexto("sleep in: " + value, 13, "rigth");
  }
}

void crearPantallaSoporte() {
  tft.fillScreen(TFT_BLACK);
  spr.setTextColor(TFT_YELLOW, TFT_BLACK);
  setTexto("Soporte Camara", 1, "center");
drawLineSub(1);
  setTexto("Cerrar", 14, "left");
  subOpcionSelect = 0;
}

void updatePantallaBrazo01(String obj, String value) {

  spr.setTextColor(TFT_YELLOW, TFT_BLACK);

  if (obj == "cnn_lbl") {
    setTexto("Conectando..       ", 3, "left");

  } else if (obj == "cnn_puntos") {
    setTexto(value, 4, "left");

  } else if (obj == "error") {
    tft.fillScreen(TFT_RED);
    setTexto("Luz 01", 2, "center");
    setTexto("Error!!!", 6, "center");
    setTexto(value, 7, "center");
    setTexto("Cerrar", 15, "left");
    subOpcionSelect = 1;

  } else if (obj == "cnn_exito") {
    setTexto("Conectado  :)         ", 3, "left");
    setTexto("                              ", 4, "left");

    updatePantallaBrazo01("Ejes1y2", "");
  } else if (obj == "ejeX") {
    setTexto("  EjeX: " + value, 4, "left");

  } else if (obj == "ejeY") {
    setTexto("  EjeY: " + value, 5, "left");

  } else if (obj == "sendData") {
    setTexto("SD: '" + value + "'                  ", 13, "left");
  } else {

    setTexto("> Ejes 1 y 2 <", 6, 2);
    setTexto("> Ejes 3 y 4 <", 7, 2);
    setTexto("> C R -> ", 8, 2);
    setTexto("> C G <", 9, 2);
    setTexto("> C B <", 10, 2);
    setTexto("> C W <", 11, 2);
    setTexto("> C Y <", 12, 2);

    if (obj == "Ejes1y2") {
      spr.setTextColor(TFT_BLACK, TFT_GREEN);
      subOpcionSelect = 1;
      setTexto("> Ejes 1 y 2 <", 6, 2);
    }

    if (obj == "Ejes3y4") {
      spr.setTextColor(TFT_BLACK, TFT_GREEN);
      subOpcionSelect = 2;
      setTexto("> Ejes 3 y 4 <", 7, 2);
    }

    if (obj == "CR") {
      spr.setTextColor(TFT_BLACK, TFT_GREEN);
      subOpcionSelect = 3;
      setTexto("> C R -> ", 8, 2);
    }

    if (obj == "CG") {
      spr.setTextColor(TFT_BLACK, TFT_GREEN);
      subOpcionSelect = 4;
      setTexto("> C G <", 9, 2);
    }

    if (obj == "CB") {
      spr.setTextColor(TFT_BLACK, TFT_GREEN);
      subOpcionSelect = 5;
      setTexto("> C B <", 10, 2);
    }

    if (obj == "CW") {
      spr.setTextColor(TFT_BLACK, TFT_GREEN);
      subOpcionSelect = 6;
      setTexto("> C W <", 11, 2);
    }

    if (obj == "CY") {
      spr.setTextColor(TFT_BLACK, TFT_GREEN);
      subOpcionSelect = 7;
      setTexto("> C Y <", 12, 2);
    }
  }
}

void crearPantallaBrazo01(String titulo) {
  tft.fillScreen(TFT_BLACK);
  spr.setTextColor(TFT_YELLOW, TFT_BLACK);
  setTexto(titulo, 1, "center");
drawLineSub(1);
  setTexto("Cerrar", 14, "left");
  subOpcionSelect = 0;
}


