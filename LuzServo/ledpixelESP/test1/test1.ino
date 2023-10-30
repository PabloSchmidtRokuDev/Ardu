/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#include "arduinoFFT.h"
#include <IRremote.h>

#define BLYNK_TEMPLATE_ID "TMPLcKROAFEk"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "AI9Di0_ykDnJO-fK-D4SveGt1gDujN2o"

#include <Arduino.h>
#include "FastLED.h"
#define NUM_LEDS 576
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND 120
CRGB leds[NUM_LEDS];

int brillo = 255;
int r, g, b;
bool powerOn = true;
bool bolaPowerOn = true;
bool rele2Power = false;
bool audioActivado = false;

#define TOTAL_EFECTOS 8
uint8_t gCurrentPatternNumber = 0;

#define PIN_LED_DATA 27
#define PIN_ULTRAVIOLETA 36
#define PIN_BOLA_RAYOS 25
#define PIN_NADA 34

#define PIN_MICROFONNO 32
#define PIN_INFRARROJO 35
#define PIN_RELE_B 33

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

IRrecv IR(PIN_INFRARROJO); //   IRrecv object  IR get code from IR remoter
decode_results IRresults;

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// setting PWM properties
const int freq = 2000;
const int ledChannel = 0;
const int resolution = 8;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "PabloDev_2_4";
char pass[] = "goodlife";
BlynkTimer timer;
arduinoFFT FFT = arduinoFFT();

TaskHandle_t Task1;

void setup()
{
  r = 137;
  g = 0;
  b = 255;

  IR.enableIRIn();

  pinMode(PIN_BOLA_RAYOS, OUTPUT);
  digitalWrite(PIN_BOLA_RAYOS, HIGH);

  pinMode(PIN_RELE_B, OUTPUT);
  digitalWrite(PIN_RELE_B, LOW);


  // Debug console
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  Blynk.begin(auth, ssid, pass);

  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    90000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    1);          /* pin task to core 0 */
  delay(500);


}


void Task1code( void * pvParameters ) {
  for(;;){
   if (powerOn == true && gCurrentPatternNumber >= 0)
  {
    selectEfecto();
  }
  else
  {
    for (int i = 0; i <= NUM_LEDS; i++)
    {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  } 
  }
}


void loop()
{
  // do_IR_Tick();
  Blynk.run();
  //timer.run();

}


void myTimerEvent()
{
  // Blynk.virtualWrite(V2, millis() / 1000);
}

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  Serial.print("- v0: ");
  Serial.println(value);

  brillo = value;
  FastLED.setBrightness(brillo);
  FastLED.show();
}

BLYNK_WRITE(V1)
{
  r = param.asInt();
  Serial.print("- v1 - r: ");
  Serial.println(r);
  static1();
}

BLYNK_WRITE(V2)
{
  g = param.asInt();
  Serial.print("- v2 g: ");
  Serial.println(g);
  static1();
}

BLYNK_WRITE(V3)
{
  b = param.asInt();
  Serial.print("- v3 b:  ");
  Serial.println(b);
  static1();
}

BLYNK_WRITE(V4)
{
  int Power = param.asInt();
  Serial.print("- v4 power: ");
  Serial.println(Power);
  if (Power == 0)
    powerOn = false;
  else
    powerOn = true;
}

BLYNK_WRITE(V5)
{
  int value = param.asInt();
  Serial.print("- v5 efecto: ");
  Serial.println(value);
  gCurrentPatternNumber = value;

  Blynk.virtualWrite(V6, value);
}

BLYNK_WRITE(V7)
{
  int Power = param.asInt();

  Serial.print("- v7: ultra violeta: ");
  Serial.println(Power);

  if (Power == 0)
  {
    bolaPowerOn = false;
    digitalWrite(PIN_RELE_B, LOW);
    digitalWrite(PIN_BOLA_RAYOS, LOW);
  }
  else
  {
    bolaPowerOn = true;
    
    digitalWrite(PIN_BOLA_RAYOS, HIGH);
  }
}


BLYNK_WRITE(V8)
{
  int Power = param.asInt();

  Serial.print("- v: rele2: ");
  Serial.println(Power);

  if (Power == 0)
  {
    rele2Power = false;
    digitalWrite(PIN_RELE_B, LOW);
  }
  else
  {
    rele2Power = true;
    digitalWrite(PIN_RELE_B, HIGH);
  }
}


void static1()
{
  if (powerOn == true && gCurrentPatternNumber == 0)
  {

    FastLED.setBrightness(brillo);
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB(r, g, b);
    }
    FastLED.show();
  }
}

BLYNK_CONNECTED()
{
  Blynk.virtualWrite(V0, brillo);
  Blynk.virtualWrite(V1, r);
  Blynk.virtualWrite(V2, g);
  Blynk.virtualWrite(V3, b);
  if (powerOn) Blynk.virtualWrite(V4, 1);  else Blynk.virtualWrite(V4, 0);

  Blynk.virtualWrite(V5, gCurrentPatternNumber);
  Blynk.virtualWrite(V6, gCurrentPatternNumber);
  Blynk.virtualWrite(V7, bolaPowerOn);

  Blynk.virtualWrite(V8, rele2Power);

  // Change Web Link Button message to "Congratulations!"
  /* Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
    Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
    Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");*/
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {sinEfecto, doubleRainbow, pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, blancoGira};
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void selectEfecto()
{

  gPatterns[gCurrentPatternNumber]();
  if (gCurrentPatternNumber > 0)
  {

    if (gCurrentPatternNumber == 1)
    {
      digitalWrite(PIN_BOLA_RAYOS, HIGH);
      digitalWrite(PIN_RELE_B, HIGH);
      bolaPowerOn = true;
    }else if(bolaPowerOn == true){
      digitalWrite(PIN_RELE_B, LOW);
      bolaPowerOn = false;
    }

    FastLED.show();

    if (gCurrentPatternNumber == 9)
    {
      FastLED.delay(1000 / FRAMES_PER_SECOND);
      // delay(2);
    }
    else
    {
      FastLED.delay(1000 / FRAMES_PER_SECOND);
      //delay(15);
    }

    // do some periodic updates
    EVERY_N_MILLISECONDS(20)
    {
      gHue++; // slowly cycle the "base color" through the rainbow
    }

    // EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
  }
  else
  {
    static1();
  }
}

void sinEfecto()
{
  static1();
}

int posBlanco = 0;

void blancoGira()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16; // gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;

    if (i >= posBlanco && i <= posBlanco + 36)
    {
      nblend(leds[getRealPosition(pixelnumber)], newcolor, 64);
    }
    else
    {
      nblend(leds[getRealPosition(pixelnumber)], CRGB::Black, 64);
    }
  }

  if (NUM_LEDS <= posBlanco)
  {
    posBlanco = 0;
  }
  else
  {
    posBlanco++;
  }
}

int getRealPosition(int ledPos)
{
  if (NUM_LEDS <= ledPos)
  {
    return ledPos - NUM_LEDS;
  }

  return ledPos;
}

const uint16_t samples = 16;
uint16_t k = 0;
double vReal[samples];
double vImag[samples];
double t = 0, ganancia = 1.0;
double uv = 0;

int contador = 0;
CRGB newcolorFijo;

int multiplicador = 1;
int cantidadFranjas = 1;


void setLedColorPosi(int vR, int posicion, int cFranjas, int indice, int multip, CRGB colorXX) {
  if (posicion <= cFranjas) {

    if (indice < vR) {
      leds[multip * posicion + indice] = colorXX;
    }
    else
    {
      leds[multip * posicion + indice] = CRGB::Black;
    }

  }
}


void doubleRainbow()
{
  for (int j = 0; j < samples; j++)
  {
    vReal[j] = analogRead(PIN_MICROFONNO);
    Serial.println(vReal[j]);
    vImag[j] = 0;
  }

  // calculo FFT

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, samples);

  for (int f = 1; f <= samples; f++)
  {
    vReal[f] = map(vReal[f], 0, 4095, 0, 36);
  }

  uv = map(analogRead(PIN_MICROFONNO), 0, 4095, 0, 255);

  ledcWrite(ledChannel, uv);

  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 10, 255);

  if (contador == 100 || contador == 0)
  {
    newcolorFijo = ColorFromPalette(palette, gHue + (random(0, 255) * 2), beat - gHue + (random(0, 255) * 10));
    contador = 0;
  }

  contador++;

  for (int i = 0; i < 36; i++)
  {

    CRGB newcolor = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    gHue++;

    if (i < vReal[16]) leds[i] = newcolor; else leds[i] = CRGB::Black;
    if (i < vReal[1]) leds[36 + i] = newcolor; else leds[36 + i] = CRGB::Black;
    if (i < vReal[2]) leds[36 * 2 + i] = newcolor; else leds[36 * 2 + i] = CRGB::Black;
    if (i < vReal[3]) leds[36 * 3 + i] = newcolor; else leds[36 * 3 + i] = CRGB::Black;
    if (i < vReal[4]) leds[36 * 4 + i] = newcolor; else leds[36 * 4 + i] = CRGB::Black;
    if (i < vReal[5]) leds[36 * 5 + i] = newcolor; else leds[36 * 5 + i] = CRGB::Black;
    if (i < vReal[6]) leds[36 * 6 + i] = newcolor; else leds[36 * 6 + i] = CRGB::Black;
    if (i < vReal[7]) leds[36 * 7 + i] = newcolor; else leds[36 * 7 + i] = CRGB::Black;
    if (i < vReal[8]) leds[36 * 8 + i] = newcolor; else leds[36 * 8 + i] = CRGB::Black;
    if (i < vReal[9]) leds[36 * 9 + i] = newcolor; else leds[36 * 9 + i] = CRGB::Black;
    if (i < vReal[10]) leds[36 * 10 + i] = newcolor; else leds[36 * 10 + i] = CRGB::Black;
    if (i < vReal[11]) leds[36 * 11 + i] = newcolor; else leds[36 * 11 + i] = CRGB::Black;
    if (i < vReal[12]) leds[36 * 12 + i] = newcolor; else leds[36 * 12 + i] = CRGB::Black;
    if (i < vReal[13]) leds[36 * 13 + i] = newcolor; else leds[36 * 13 + i] = CRGB::Black;
    if (i < vReal[14]) leds[36 * 14 + i] = newcolor; else leds[36 * 14 + i] = CRGB::Black;
    if (i < vReal[15]) leds[36 * 15 + i] = newcolor; else leds[36 * 15 + i] = CRGB::Black;

  }
}

void pride()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16; // gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
}

void rainbow()
{

  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(255);
}

void addGlitter(fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter)
  {
    leds[random16(NUM_LEDS)] += CRGB::White;
    leds[random16(NUM_LEDS)] += CRGB::White;
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++)
  { // 9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle()
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for (int i = 0; i < 8; i++)
  {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
