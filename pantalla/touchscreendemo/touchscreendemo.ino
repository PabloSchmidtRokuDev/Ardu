// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// This demo code returns raw readings, public domain

#include <TFT_eSPI.h>  // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

#include "TouchScreen.h"

const int XP=17;  // can be a digital pin
const int XM=27 ; //A3 must be an analog pin, use "An" notation!

const int YP=14;  //A18 must be an analog pin, use "An" notation!
const int YM=16;  // can be a digital pin
/*
const int XP=6;
const int XM=A2;
const int YP=A1;
const int YM=7; //320x480 ID=0x9486
*/
#define MINPRESSURE -30
#define MAXPRESSURE 1000

const int TS_LEFT = 625;
const int TS_RT = 131;
const int TS_TOP = 903;
const int TS_BOT =  82;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int pixel_x, pixel_y;  //Touch_getXY() updates global vars
bool Touch_getXY(void) {
  TSPoint p = ts.getPoint();

     /*   Serial.print("\t  p.z = "); Serial.print(p.z);
        Serial.print("\t p.x = "); Serial.print(p.x);
        Serial.print("\t  p.y = "); Serial.println(p.y);
    
*/
  pinMode(YP, OUTPUT);  //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);  //because TFT control pins
  digitalWrite(XM, HIGH);
 // bool pressed = (p.z == -1);  //> MINPRESSURE && p.z < MAXPRESSURE);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
 
  if (pressed) {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());  //.kbv makes sense to me
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}



#define TEXT_HEIGHT 8     // Height of text to be printed and scrolled
#define BOT_FIXED_AREA 0  // Number of lines in bottom fixed area (lines counted from bottom of screen)
#define TOP_FIXED_AREA 0  // Number of lines in top fixed area (lines counted from top of screen)

uint16_t yStart = TOP_FIXED_AREA;
uint16_t yArea = 320 - TOP_FIXED_AREA - BOT_FIXED_AREA;
uint16_t yDraw = 320 - BOT_FIXED_AREA - TEXT_HEIGHT;
byte pos[42];
uint16_t xPos = 0;

//TaskHandle_t Task1;
//TaskHandle_t Task2;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(A0));
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(ILI9341_BLACK);
  setupScrollArea(TOP_FIXED_AREA, BOT_FIXED_AREA);

pinMode(34, INPUT);
pinMode(39, INPUT);

pinMode(25, INPUT);
pinMode(23, INPUT);
  // xTaskCreatePinnedToCore(
  //   Task1code,
  //   "Task1",
  //   10000,
  //   NULL,
  //   1,
  //   &Task1,
  //   0);
  // delay(500);

  // xTaskCreatePinnedToCore(
  //   Task2code,
  //   "Task2",
  //   10000,
  //   NULL,
  //   1,
  //   &Task1,
  //   0);
  // delay(500);
}

void loop(void) {

Serial.print("b1 23= ");
      Serial.print(digitalRead(23));

      Serial.print("\tb2 25= ");
      Serial.print(digitalRead(25));

      Serial.print("\tX = ");
      Serial.print(analogRead(34));
      Serial.print("\tY = ");
      Serial.println(analogRead(39));
      delay(1000);

    // bool down = Touch_getXY();
    // if (down) {
    //   Serial.print("X = ");
    //   Serial.print(pixel_x);
    //   Serial.print("\tY = ");
    //   Serial.println(pixel_y);
    //  // delay(1000);
    // }
}

void setupScrollArea(uint16_t TFA, uint16_t BFA) {
  tft.writecommand(ILI9341_VSCRDEF);  // Vertical scroll definition
  tft.writedata(TFA >> 8);
  tft.writedata(TFA);
  tft.writedata((320 - TFA - BFA) >> 8);
  tft.writedata(320 - TFA - BFA);
  tft.writedata(BFA >> 8);
  tft.writedata(BFA);
}

int scroll_slow(int lines, int wait) {
  int yTemp = yStart;
  for (int i = 0; i < lines; i++) {
    yStart++;
    if (yStart == 320 - BOT_FIXED_AREA) yStart = TOP_FIXED_AREA;
    scrollAddress(yStart);
    delay(wait);
  }
  return yTemp;
}

void scrollAddress(uint16_t VSP) {
  tft.writecommand(ILI9341_VSCRSADD);  // Vertical scrolling start address
  tft.writedata(VSP >> 8);
  tft.writedata(VSP);
}

/************************************/
void Task1code(void* pvParameters) {
  Serial.println("run task  1...");

  for (;;) {


    
  }
}

/************************************/
void Task2code(void* pvParameters) {
  Serial.println("run task  2...");

  for (;;) {
    for (int j = 0; j < 600; j += TEXT_HEIGHT) {
      for (int i = 0; i < 40; i++) {
        if (pos[i] > 20) pos[i] -= 3;                                      // Rapid fade initially brightness values
        if (pos[i] > 0) pos[i] -= 1;                                       // Slow fade later
        if ((random(20) == 1) && (j < 400)) pos[i] = 63;                   // ~1 in 20 probability of a new character
        tft.setTextColor(pos[i] << 5, ILI9341_BLACK);                      // Set the green character brightness
        if (pos[i] == 63) tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  // Draw white character
        xPos += tft.drawChar(random(32, 128), xPos, yDraw, 1);             // Draw the character
      }
      yDraw = scroll_slow(TEXT_HEIGHT, 14);  // Scroll, 14ms per pixel line
      xPos = 0;
    }

    //tft.setRotation(2);
    //tft.setTextColor(63 << 5, ILI9341_BLACK);
    //tft.drawCentreString("MATRIX",120,60,4);
    //tft.setRotation(0);

    // Now scroll smoothly forever
    while (1) {
      yield();
      yDraw = scroll_slow(320, 5);
    }  // Scroll 320 lines, 5ms per line
  }
}
