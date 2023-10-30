

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

void updateMenu(int opSelect){
    if (opSelect == 0) { spr.setTextColor(TFT_BLACK, TFT_GREEN);} else {spr.setTextColor(TFT_YELLOW, TFT_BLACK);}
    setTexto("  Luz 1  ", 4, 2);

    if (opSelect == 1){ spr.setTextColor(TFT_BLACK, TFT_GREEN);} else {spr.setTextColor(TFT_YELLOW, TFT_BLACK);}
    setTexto("  Luz 2  ", 5, 2);

    if (opSelect == 2){ spr.setTextColor(TFT_BLACK, TFT_GREEN);} else {spr.setTextColor(TFT_YELLOW, TFT_BLACK);}
    setTexto("  Brazo 5 art  ", 6, 2);

}

void crearMenu() {
  tft.fillScreen(TFT_BLACK);
  setTexto("Menu", 2, "center");

  updateMenu(0);
}







/*
  tft.setCursor(xpos, ypos);
  spr.printToSprite("12345678901234");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("12345678901234")));
  tft.setCursor(xpos, ypos);
  
  spr.printToSprite("12345678901234");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("1234567890123")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("12345678901234");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("1234567890123")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("1234567890123");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("123456789012")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("123456789012");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("12345678901")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("12345678901");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("1234567890")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("1234567890");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("123456789")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("123456789");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("12345678")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("12345678");
  Serial.println("w: " + String(spr.textWidth("1234567")));
  ypos += spr.fontHeight();
tft.setCursor(xpos, ypos);

  spr.printToSprite("1234567");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("123456")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("123456");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("12345")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("12345");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("1234")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("1234");
  ypos += spr.fontHeight();
  Serial.println("w: " + String(spr.textWidth("123")));
tft.setCursor(xpos, ypos);

  spr.printToSprite("123");
  ypos += spr.fontHeight();
Serial.println("w: " + String(spr.textWidth("12")));
Serial.println("w: " + String(spr.textWidth("1")));
*/

  