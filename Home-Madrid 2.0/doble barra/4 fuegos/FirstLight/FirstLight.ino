#include <FastLED.h>

#define LED_PIN 13
#define COLOR_ORDER GRB
#define CHIPSET WS2811
#define NUM_LEDS 654

#define BRIGHTNESS 60
#define FRAMES_PER_SECOND 20

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

int ccc = 0;
void loop() {
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());

  Fire2012();  // run simulation frame

  FastLED.show();  // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // if (ccc == 50){
  //   gReverseDirection = !gReverseDirection;
  //   ccc = 0;
  // }
  // ccc++;
}


// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING 80

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 180

int L1Start = 0;
int L1Fin = 168;
int L1Total = 168;

int L2Start = 337;
int L2Fin = 169;
int L2Total = 167;

int L3Start = 338;
int L3Fin = 496;
int L3Total = 158;

int L4Start = 654;
int L4Fin = 497;
int L4Total = 157;


static uint8_t heat[NUM_LEDS];

void Fire2012() {

  //Serial.println("linea: 1");
  setColorLinea(L1Start, L1Total, false);
  //Serial.println("linea: 2");
  setColorLinea(L2Start, L2Total, true);
  //Serial.println("linea: 3");
  setColorLinea(L3Start, L3Total, false);
  //Serial.println("linea: 4");
  setColorLinea(L4Start, L4Total, true);

  // // Step 1.  Cool down every cell a little
  //   for( int i = 0; i < maxReal; i++) {
  //     heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / maxReal) + 2));
  //   }

  //   // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  //   for( int k= maxReal - 1; k >= 2; k--) {
  //     heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 5] ) / 3;
  //   }

  //   // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  //   if( random8() < SPARKING ) {
  //     int y = random8(7);
  //     heat[y] = qadd8( heat[y], random8(160,255) );
  //   }

  //   // Step 4.  Map from heat cells to LED colors
  //   for( int j = 0; j < maxReal; j++) {
  //     CRGB color = HeatColor( heat[j]);


  //     leds[j] = color;
  //   }





  //   for( int i = 0; i < totalL2; i++) {
  //     heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / totalL2) + 2));
  //   }


  //   // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  //   for( int k= totalL2 - 1; k >= 2; k--) {
  //     heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 5] ) / 3;
  //   }

  //   // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  //   if( random8() < SPARKING ) {
  //     int y = random8(7);
  //     heat[y] = qadd8( heat[y], random8(160,255) );
  //   }

  //   // Step 4.  Map from heat cells to LED colors
  //   for( int j = 0; j < totalL2; j++) {
  //     CRGB color = HeatColor( heat[j]);

  //     leds[starL2 - j] = color;
  //   }
}

void setColorLinea(int start, int total, bool invert) {

  //Serial.println("step: 1");
  for (int i = 0; i < total; i++) {
    heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / total) + 2));
  }

  //Serial.println("step: 2");
  for (int k = total - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 5]) / 3;
  }

  //Serial.println("step: 3");
  if (random8() < SPARKING) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  //Serial.println("step: 4");
  for (int j = 0; j < total; j++) {
    CRGB color = HeatColor(heat[j]);
    if (invert) {
      leds[start - j] = color;
    } else {
      leds[j + start] = color;
      
    }

    // int L2Start = 337;
    // int L2Fin = 169;
    // int L2Total = 167;
  }
  //Serial.println("step: fin");
}