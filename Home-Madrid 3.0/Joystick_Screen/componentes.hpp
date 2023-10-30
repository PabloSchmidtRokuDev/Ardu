
class botonC {
public:

  bool select = false;

  String text;
  int posX;
  int posY;
  int alto;
  int ancho;

  void draw(TFT_eSprite *dspr);
  void setEstado(String _text, int _posX, int _posY, int _ancho, int _alto);
  void setSelected(int _posX, int _posY);
  bool getSelected();
private:

  TFT_eSprite spt;
};

void botonC::draw(bool _select) {
  select = _select;

  tft.fillRect(posX + bordeLeft +anchoColor, posY + bordeTop, 15, 15, TFT_BLACK);
  if (select == true) {
    tft.fillRect(posX + bordeLeft + anchoColor, posY + bordeTop, 15, 15, colorHEX);
    tft.drawRect(posX + bordeLeft + anchoColor, posY + bordeTop, 15, 15, TFT_WHITE);
  }
}

void botonC::setEstado(String _nombre, int _r, int _g, int _b, uint16_t _colorHex, int _posX, int _posY) {
  nombre = _nombre;
  r = map(_r, 0, 63, 0, 255);
  g = map(_g, 0, 63, 0, 255);
  b = map(_b, 0, 63, 0, 255);
  colorHEX = _colorHex;
  posX = _posX;
  posY = _posY;

  setSelect(false);
};

  void  botonC::setSelected(int _posX, int _posY){

  }
  
  bool  botonC::getSelected(){
    
  }

