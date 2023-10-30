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


}


#define COOLING 80
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