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

  int posX;
  int posY;

  void setSelect(bool _select);
  void setEstado(String _nombre, int _r, int _g, int _b, uint16_t _colorHex, int _posX, int _posY);
};

void colorX::setSelect(bool _select) {
  select = _select;

  Serial.print("select color: " + nombre + " posX: " + String(posX) + " posY: " + String(posY));



  Serial.println(" hex: " + String(colorHEX));
  //fillRect
  //drawRect
}


void colorX::setEstado(String _nombre, int _r, int _g, int _b, uint16_t _colorHex, int _posX, int _posY) {
  nombre = _nombre;
  r = _r;
  g = _g;
  b = _b;
  colorHEX = _colorHex;
  posX = _posX;
  posY = _posY;

  setSelect(false);
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



// colorX getColores(direccionEum direccion) {
//   colorX colorXsel = colorXList[indexSelColor];
//   colorXsel.setSelect(false);

//   Serial.print(String(direccion) + " - indexSelColor color: " + String(indexSelColor));

//   if (direccion == dir_rigth) {
//     if (colorXsel.posX == 7) {
//       indexSelColor = indexSelColor - 7;
//     } else {
//       indexSelColor++;
//     }

//   } else if (direccion == dir_left) {
//     if (colorXsel.posX == 0) {
//       indexSelColor = indexSelColor + 7;
//     } else {
//       indexSelColor--;
//     }

//   } else if (direccion == dir_up) {
//     if (colorXsel.posY == 0) {
//       indexSelColor = 104 + indexSelColor;
//     } else {
//       indexSelColor = indexSelColor - 8;
//     }

//   } else if (direccion == dir_down) {
//     if (colorXsel.posY == 13) {
//       indexSelColor = indexSelColor - 104;
//     } else {
//       indexSelColor = indexSelColor + 8;
//     }
//   }

//   colorXsel = colorXList[indexSelColor];
//   colorXsel.setSelect(true);

//   Serial.println("-  indexSelColor despues color: " + String(indexSelColor));
//   Serial.println("rgb: " + String(colorXsel.r) + ";" + String(colorXsel.g) + ";" + String(colorXsel.b));
//   Serial.println(colorXsel.r);
//   Serial.println(colorXsel.g);
//   Serial.println(colorXsel.b);


//   return colorXsel;
// }