#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// work in line numbers.  Font height in ht
int16_t ht = 16, top = 3, line, lines = 15, scroll;




#define TEXT_HEIGHT 8 // Height of text to be printed and scrolled
#define BOT_FIXED_AREA 0  // Number of lines in bottom fixed area (lines counted from bottom of screen)
#define TOP_FIXED_AREA 0  // Number of lines in top fixed area (lines counted from top of screen)

uint16_t yStart = TOP_FIXED_AREA;
uint16_t yArea = 320 - TOP_FIXED_AREA - BOT_FIXED_AREA;
uint16_t yDraw = 320 - BOT_FIXED_AREA - TEXT_HEIGHT;
byte     pos[42];
uint16_t xPos = 0;


void setup()
{
    tft.reset();
    uint16_t id = tft.readID();
    setupScrollArea(TOP_FIXED_AREA, BOT_FIXED_AREA, id);
    
    tft.begin(id);
    tft.setRotation(1);     //Portrait
    tft.fillScreen(BLACK);
    
    // 
    // tft.setTextColor(WHITE, BLACK);
    // tft.setTextSize(2);     // System font is 8 pixels.  ht = 8*2=16
    // tft.setCursor(100, 0);
    // tft.print("ID = 0x");
    // tft.println(id, HEX);
    // if (id == 0x9320 || id == 0x9325 || id == 0xB509) {
    //     top = 0;                      // these controllers scroll full screen
    //     lines = tft.height() / ht;    // we are in portrait mode
    // }
    // if (id == 0x7783) {
    //     tft.println("can NOT scroll");
    //     while (1);                    // die.
    // }
    // tft.setCursor(0, 0);
    // for (line = 1; line < 21; line++) tft.println(String(line) + ": ");
}

void loop()
{
    tft.setCursor(0, (scroll + top) * ht);
    // if (++scroll >= lines) scroll = 0;
    // tft.vertScroll(top * ht, lines * ht, (scroll) * ht);
    // tft.println(String(line) + ": [" + String(scroll) + "]  ");
    // delay(100);
    // line++;

  for (int j = 0; j < 600; j += TEXT_HEIGHT) {
    for (int i = 0; i < 40; i++) {
      if (pos[i] > 20) pos[i] -= 3; // Rapid fade initially brightness values
      if (pos[i] > 0) pos[i] -= 1; // Slow fade later
      if ((random(20) == 1) && (j<400)) pos[i] = 63; // ~1 in 20 probability of a new character
      
      tft.setTextColor(pos[i] << 5, BLACK); // Set the green character brightness
      if (pos[i] == 63) tft.setTextColor(WHITE, BLACK); // Draw white character

      unsigned char uniCode = random(32, 128);
      tft.drawChar(line, yDraw, uniCode, GREEN, BLACK, 1); // Draw the character
      //(int16_t x, int16_t y, unsigned char c, uint16_t color,  uint16_t bg, uint8_t size
      line++;
    }
    yDraw = scroll_slow(TEXT_HEIGHT, 14); // Scroll, 14ms per pixel line
    line = 0;
  }

  // Now scroll smoothly forever
 // while (1) {yield(); yDraw = scroll_slow(320,5); }// Scroll 320 lines, 5ms per line
}



void setupScrollArea(uint16_t TFA, uint16_t BFA,  uint16_t id) {
     if (id == 0x9320 || id == 0x9325 || id == 0xB509) {
         top = 0;                      // these controllers scroll full screen
         lines = tft.height() / ht;    // we are in portrait mode
     }

  // tft.writecommand(ILI9341_VSCRDEF); // Vertical scroll definition
  // tft.writedata(TFA >> 8);
  // tft.writedata(TFA);
  // tft.writedata((320 - TFA - BFA) >> 8);
  // tft.writedata(320 - TFA - BFA);
  // tft.writedata(BFA >> 8);
  // tft.writedata(BFA);
}

int scroll_slow(int lines, int wait) {
   int yTemp = scroll;
  for (int i = 0; i < lines; i++) {
    yStart++;
    if (yStart == 320 - BOT_FIXED_AREA) yStart = TOP_FIXED_AREA;
    //scrollAddress(yStart);

    tft.setCursor(0, yStart);
if (++scroll >= lines) scroll = 0;
    tft.vertScroll(yStart, lines * yArea, (scroll) * yArea);
lines++;
scroll++;
    delay(wait);
  }
  return  yTemp;
}


