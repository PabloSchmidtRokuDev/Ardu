// This example is based on this tutorial
// https://www.instructables.com/id/Converting-Images-to-Flash-Memory-Iconsimages-for-/
// https://github.com/STEMpedia/eviveProjects/blob/master/imageToFlashMemoryIconsForTFT/

//Don't forget to change User_Setup.h inside TFT_eSPI library !

#include <TFT_eSPI.h>
#include "y240_3.h"
#include "y240_1.h"
#include "y240_4.h"
#include "yo_2.h"
TFT_eSPI tft = TFT_eSPI();   // Invoke library

void setup(void) {
  Serial.begin(115200);
  Serial.print("ST7789 TFT Bitmap Test");

  tft.begin();     // initialize a ST7789 chip
  tft.setSwapBytes(true); // Swap the byte order for pushImage() - corrects endianness

  tft.fillScreen(TFT_BLACK);

}
int cc = 0;
void loop() {

  if (cc == 0) {
    tft.pushImage(0, 0, 240, 240, y240_3);
  }
  else if (cc == 1) {
    tft.pushImage(0, 0, 240, 240, y240_1);
    
  } else if (cc == 2) {
    tft.pushImage(0, 0, 240, 240, y240_4);
    
  } else if (cc == 3) {
    tft.pushImage(0, 0, 240, 240, yo_2);
    cc=-1;
  }

  cc++;


  delay(4000);
}
