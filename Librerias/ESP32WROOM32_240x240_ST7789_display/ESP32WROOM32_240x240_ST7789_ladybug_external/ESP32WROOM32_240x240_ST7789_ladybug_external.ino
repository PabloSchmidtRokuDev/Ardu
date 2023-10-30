// EXTERNAL C-ARRAY
// ESP32WROOM32_240x240_ST7789_ladybug_external
// microprocessor is a ESP32-WROOM-32
// ST7789 based 240*240 SPI TFT display
// 
// wiring:
// Vcc = 3.3V - this display is 5V compliant
// display pin SCL ----> WROOM pin D18
// display pin SDA ----> WROOM pin D23
// display pin RES ----> WROOM pin D4
// display pin DC  ----> WROOM pin D2
// display pin BLK ----> not connected


// example is based on this tutorial
// https://www.instructables.com/id/Converting-Images-to-Flash-Memory-Iconsimages-for-/
// https://github.com/STEMpedia/eviveProjects/blob/master/imageToFlashMemoryIconsForTFT/

// Floris Wouterlood
// Nov 6, 2021
// public domain

// don't forget to change User_Setup.h inside TFT_eSPI library - that is: point to the user specific, tweaked Setup_24_ST7789.h !

   #include <TFT_eSPI.h>                                            
   TFT_eSPI tft = TFT_eSPI();                
   
  #include "ladybug.h"


void setup(void) {
  
  Serial.begin (9600);
  Serial.print ("ST7789 TFT Bitmap Test");

  tft.begin ();                                 // initialize a ST7789 chip
  tft.setSwapBytes (true);                      // swap the byte order for pushImage() - corrects endianness

tft.setRotation(3);
  tft.fillScreen (TFT_BLACK);
  tft.pushImage (0,0,240,240,ladybug);
}

void loop() {
}
