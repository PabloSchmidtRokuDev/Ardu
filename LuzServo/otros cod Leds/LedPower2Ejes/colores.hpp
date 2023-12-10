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

byte red = 31;
byte green = 0;
byte blue = 0;

byte state = 0;
unsigned int colour = red << 11;  // Colour order is RGB 5+6+5 bits each


const int maxColors = 500;
colorX colorXList[maxColors];  // An array of MyClass objects using default constructor

void cargarVecctorColores() {
 
  for (int i = 0; i < maxColors; i++) {

    colorXList[i].setEstado("c:" + String(colour), red, green, blue, colour, 0, i);
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

        if (red >= 253) {
          red = 0;
          state = 2;
        }
        break;

      case 2:
        blue = blue + 2;
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