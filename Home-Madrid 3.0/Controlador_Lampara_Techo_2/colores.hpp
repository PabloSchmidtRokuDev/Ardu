class colorX {
public:

  int r;
  int g;
  int b;
  String nombre;


  void setEstado(String _nombre, int _r, int _g, int _b, uint16_t _colorHex, int _posX, int _posY);
};



void colorX::setEstado(String _nombre, int _r, int _g, int _b, uint16_t _colorHex, int _posX, int _posY) {
  nombre = _nombre;
  r = _r;
  g = _g;
  b = _b;

};


const int maxColors = 52;
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
}