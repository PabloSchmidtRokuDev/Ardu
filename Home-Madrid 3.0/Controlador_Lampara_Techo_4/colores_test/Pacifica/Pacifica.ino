//
//  "Pacifica"
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
//

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#define DATA_PIN 5
#define NUM_LEDS 600
#define MAX_POWER_MILLIAMPS 6000
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

//////////////////////////////////////////////////////////////////////////

CRGB leds[NUM_LEDS];

void setup() {
  delay(3000);  // 3 second delay for boot recovery, and a moment of silence
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_MILLIAMPS);
}

void loop() {
  EVERY_N_MILLISECONDS(20) {
    pacifica_loop();
    FastLED.show();
  }
}

//////////////////////////////////////////////////////////////////////////
//
// The code for this animation is more complicated than other examples, and
// while it is "ready to run", and documented in general, it is probably not
// the best starting point for learning.  Nevertheless, it does illustrate some
// useful techniques.
//
//////////////////////////////////////////////////////////////////////////
//
// In this animation, there are four "layers" of waves of light.
//
// Each layer moves independently, and each is scaled separately.
//
// All four wave layers are added together on top of each other, and then
// another filter is applied that adds "whitecaps" of brightness where the
// waves line up with each other more.  Finally, another pass is taken
// over the led array to 'deepen' (dim) the blues and greens.
//
// The speed and scale and motion each layer varies slowly within independent
// hand-chosen ranges, which is why the code has a lot of low-speed 'beatsin8' functions
// with a lot of oddly specific numeric ranges.
//
// These three custom blue-green color palettes were inspired by the colors found in
// the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7
//
/*
CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };
*/

CRGBPalette16 pp0 = { 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF };
CRGBPalette16 pp1 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x202020, 0x404040, 0x606060, 0x808080, 0x000000, 0x202020, 0x404040, 0x606060, 0x808080, 0x808080, 0xA0A0A0 };   //, 0xA0A0A0, 0xC0C0C0, 0xE0E0E0, 0xFFFFFF};
CRGBPalette16 pp2 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x330019, 0x660033, 0x99004C, 0xCC0066, 0xFF007F, 0x330019, 0x660033, 0x99004C, 0xCC0066, 0xFF007F, 0xFF007F, 0xFF3399 };   //, 0xFF3399, 0xFF66B2, 0xFF99CC, 0xFFCCE5};
CRGBPalette16 pp3 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x330033, 0x660066, 0x990099, 0xCC00CC, 0xFF00FF, 0x330033, 0x660066, 0x990099, 0xCC00CC, 0xFF00FF, 0xFF00FF, 0xFF33FF };   //, 0xFF33FF, 0xFF66FF, 0xFF99FF, 0xFFCCFF};
CRGBPalette16 pp4 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x190033, 0x330066, 0x4C0099, 0x6600CC, 0x7F00FF, 0x190033, 0x330066, 0x4C0099, 0x6600CC, 0x7F00FF, 0x7F00FF, 0x9933FF };   //, 0x9933FF, 0xB266FF, 0xCC99FF, 0xE5CCFF};
CRGBPalette16 pp5 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x000033, 0x000066, 0x000099, 0x0000CC, 0x0000FF, 0x000033, 0x000066, 0x000099, 0x0000CC, 0x0000FF, 0x0000FF, 0x3399FF };   //, 0x3333FF, 0x6666FF, 0x9999FF, 0xCCCCFF};
CRGBPalette16 pp6 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x001933, 0x003366, 0x004C99, 0x0066CC, 0x0080FF, 0x001933, 0x003366, 0x004C99, 0x0066CC, 0x0080FF, 0x0080FF, 0x3399FF };   //, 0x3399FF, 0x66B2FF, 0x99CCFF, 0xCCE5FF};
CRGBPalette16 pp7 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x003333, 0x006666, 0x009999, 0x00CCCC, 0x00FFFF, 0x003333, 0x006666, 0x009999, 0x00CCCC, 0x00FFFF, 0x00FFFF, 0x33FFFF };   //, 0x33FFFF, 0x66FFFF, 0x99FFFF, 0xCCFFFF};
CRGBPalette16 pp8 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x003319, 0x006633, 0x00994C, 0x00CC66, 0x00FF80, 0x003319, 0x006633, 0x00994C, 0x00CC66, 0x00FF80, 0x00FF80, 0x33FF99 };   //, 0x33FF99, 0x66FFB2, 0x99FFCC, 0xCCFFE5};
CRGBPalette16 pp9 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x003300, 0x006600, 0x009900, 0x00CC00, 0x00FF00, 0x003300, 0x006600, 0x009900, 0x00CC00, 0x00FF00, 0x00FF00, 0x33FF33 };   //, 0x33FF33, 0x66FF66, 0x99FF99, 0xCCFFCC};
CRGBPalette16 pp10 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x193300, 0x336600, 0x4C9900, 0x66CC00, 0x80FF00, 0x193300, 0x336600, 0x4C9900, 0x66CC00, 0x80FF00, 0x80FF00, 0x99ff33 };  //,, 0x99FF33, 0xB2FF66, 0xCCFF99, 0xE5FFCC};
CRGBPalette16 pp11 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x333300, 0x666600, 0x999900, 0xCCCC00, 0xFFFF00, 0x333300, 0x666600, 0x999900, 0xCCCC00, 0xFFFF00, 0xFFFF00, 0xFFFF33 };  //,, 0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC};
CRGBPalette16 pp12 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x331900, 0x663300, 0x994C00, 0xCC6600, 0xFF8000, 0x331900, 0x663300, 0x994C00, 0xCC6600, 0xFF8000, 0xFF8000, 0xFF9933 };  //,, 0xFF9933, 0xFFB266, 0xFFCC99, 0xFFE5CC};
CRGBPalette16 pp13 = { 0x000000, 0x000000, 0x000000, 0x000000, 0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000, 0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000, 0xFF0000, 0xFF3333 };  //,, 0xFF3333, 0xFF6666, 0xFF9999, 0xFFCCCC};

CRGB bgColorRed = CRGB(255, 5, 5);
CRGB bgColorgreen = CRGB(38, 255, 33);
CRGB bgColorblue = CRGB(0, 0, 255);
CRGB bgColorRedLight = CRGB(243, 125, 34);
CRGB bgColorBlueLight = CRGB(34, 194, 243);
CRGB bgColorGreenLight = CRGB(34, 243, 41);


CRGB bgColor = CRGB(0, 0, 0);

CRGBPalette16 pacifica_palette_1 = pp10;
CRGBPalette16 pacifica_palette_2 = pp12;
CRGBPalette16 pacifica_palette_3 = pp9;

CRGBPalette16 pacifica_palette_4 = pp0;

void pacifica_loop() {
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4, sCIStart5;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011, 10, 13));
  sCIStart2 -= (deltams21 * beatsin88(777, 8, 11));
  sCIStart3 -= (deltams1 * beatsin88(501, 5, 7));
  sCIStart4 -= (deltams2 * beatsin88(257, 4, 6));

  // Clear out the LED array to a dim background blue-green
  fill_solid(leds, NUM_LEDS, bgColor);

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer(pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0 - beat16(301));
  //pacifica_one_layer(pacifica_palette_2, sCIStart2, beatsin16(4, 6 * 256, 9 * 256), beatsin8(17, 40, 80), beat16(401));
  pacifica_one_layer(pacifica_palette_3, sCIStart3, 6 * 256, beatsin8(9, 10, 38), 0 - beat16(503));
//pacifica_one_layer(pacifica_palette_4, sCIStart4, 5 * 256, beatsin8(8, 10, 28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
 // pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  //pacifica_deepen_colors();
}

// Add one layer of waves into the led array
void pacifica_one_layer(CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff) {
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16(waveangle) + 32768;
    uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16(ci) + 32768;
    uint8_t sindex8 = scale16(sindex16, 240);
    CRGB c = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps() {
  uint8_t basethreshold = beatsin8(9, 55, 65);
  uint8_t wave = beat8(7);

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      leds[i] += CRGB(overage, overage2, qadd8(overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8(leds[i].blue, 145);
    leds[i].green = scale8(leds[i].green, 200);
    leds[i] |= CRGB(2, 5, 7);
  }
}
