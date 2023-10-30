/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#include "arduinoFFT.h"
#include <Arduino.h>
#include "FastLED.h"


#define NUM_LEDS 576
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
int FRAMES_PER_SECOND = 120;
CRGB leds[NUM_LEDS];

int brillo = 255;
int r, g, b;
bool powerOn = true;
bool powerLed = true;
bool bolaPowerOn = true;
bool rele2Power = true;
bool audioActivado = false;
int efectoSonido = 1;

#define TOTAL_EFECTOS 8
uint8_t gCurrentPatternNumber = 0;

#define PIN_LED_DATA 27
#define PIN_ULTRAVIOLETA 36
#define PIN_BOLA_RAYOS 25

#define PIN_MICROFONNO 32
#define PIN_INFRARROJO 26
#define PIN_RELE_B 33

// setting PWM properties
const int freq = 2000;
const int ledChannel = 0;
const int resolution = 8;

arduinoFFT FFT = arduinoFFT();

//************************  class*******************
enum functionSelect
{
  FS_nada,
  FS_vMasT,
  FS_vMenosT,
  FS_play,
  FS_power,
  FS_colorX,
  FS_brillo,
  FS_tiempo,
  FS_audioEfecto,
  FS_efectoX
};

void setup()
{
  
  r = 137;
  g = 0;
  b = 255;



  FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  static1();

  // Debug console
  Serial.begin(115200);

  
  Serial.println("setup complete.");
}

void loop()
{
 
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

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {sinEfecto, efectoAudio, pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, blancoGira};
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
    }

    FastLED.show();

    FastLED.delay(1000 / FRAMES_PER_SECOND);

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

void setLedColorPosi(int vR, int posicion, int cFranjas, int indice, int multip, CRGB colorXX)
{
  if (posicion <= cFranjas)
  {

    if (indice < vR)
    {
      leds[multip * posicion + indice] = colorXX;
    }
    else
    {
      leds[multip * posicion + indice] = CRGB::Black;
    }
  }
}

void efectoAudio()
{
  Serial.println("efectoAudio: " + (String)efectoSonido);
  if (efectoSonido == 2)
  {
    efectoAudio2();
  }
  else if (efectoSonido == 3)
  {
    efectoAudio3();
  }
  else if (efectoSonido == 4)
  {
    efectoAudio4();
  }
  else
  {
    doubleRainbow();
  }
}

void efectoAudio2()
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
    vReal[f] = map(vReal[f], 0, 4095, 0, 144);
  }

  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 10, 255);

  if (contador == 1000 || contador == 0)
  {
    newcolorFijo = ColorFromPalette(palette, gHue + (random(0, 255) * 2), beat - gHue + (random(0, 255) * 10));
    contador = 0;
  }

  contador++;

  for (int i = 0; i < 144; i++)
  {
    CRGB newcolor = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));

    if (i < vReal[16])
      leds[i] = newcolor;
    else
      leds[i] = newcolorFijo;
    if (i < vReal[1])
      leds[144 + i] = newcolor;
    else
      leds[144 + i] = newcolorFijo;
    if (i < vReal[2])
      leds[144 * 2 + i] = newcolor;
    else
      leds[144 * 2 + i] = newcolorFijo;
    if (i < vReal[3])
      leds[144 * 3 + i] = newcolor;
    else
      leds[144 * 3 + i] = newcolorFijo;
  }
}

void efectoAudio3()
{
  for (int j = 0; j < samples; j++)
  {
    vReal[j] = analogRead(PIN_MICROFONNO);
    // Serial.println(vReal[j]);
    vImag[j] = 0;
  }

  // calculo FFT

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, samples);

  for (int f = 1; f <= samples; f++)
  {
    vReal[f] = map(vReal[f], 0, 4095, 0, 144);
  }

  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 10, 255);

  if (contador == 100 || contador == 0)
  {
    newcolorFijo = ColorFromPalette(palette, gHue + (random(0, 255) * 2), beat - gHue + (random(0, 255) * 10));
    contador = 0;
  }

  contador++;

  for (int i = 0; i < 144; i++)
  {

    CRGB newcolor = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    gHue++;

    if (i < vReal[16])
      leds[i] = newcolorFijo;
    else
      leds[i] = CRGB::Black;
    if (i < vReal[1])
      leds[144 + i] = newcolor;
    else
      leds[144 + i] = CRGB::Black;
    if (i < vReal[2])
      leds[144 * 2 + i] = newcolorFijo;
    else
      leds[144 * 2 + i] = CRGB::Black;
    if (i < vReal[3])
      leds[144 * 3 + i] = newcolor;
    else
      leds[144 * 3 + i] = CRGB::Black;
  }
}

void efectoAudio4()
{
  for (int j = 0; j < samples; j++)
  {
    vReal[j] = analogRead(PIN_MICROFONNO);
    // Serial.println(vReal[j]);
    vImag[j] = 0;
  }

  // calculo FFT

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, samples);

  for (int f = 1; f <= samples; f++)
  {
    vReal[f] = map(vReal[f], 0, 4095, 0, 72);
  }

  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 10, 255);

  if (contador == 100 || contador == 0)
  {
    newcolorFijo = ColorFromPalette(palette, gHue + (random(0, 255) * 2), beat - gHue + (random(0, 255) * 10));
    contador = 0;
  }

  contador++;

  for (int i = 0; i < 72; i++)
  {

    CRGB newcolor = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    gHue++;

    if (i < vReal[16])
      leds[i] = newcolorFijo;
    else
      leds[i] = CRGB::Black;
    if (i < vReal[1])
      leds[72 + 72 - i] = newcolor;
    else
      leds[72 + 72 - i] = CRGB::Black;
    if (i < vReal[2])
      leds[72 * 2 + i] = newcolorFijo;
    else
      leds[72 * 2 + i] = CRGB::Black;
    if (i < vReal[3])
      leds[72 * 3 + 72 - i] = newcolor;
    else
      leds[72 * 3 + 72 - i] = CRGB::Black;
    if (i < vReal[4])
      leds[72 * 4 + i] = newcolorFijo;
    else
      leds[72 * 4 + i] = CRGB::Black;
    if (i < vReal[5])
      leds[72 * 5 + 72 - i] = newcolor;
    else
      leds[72 * 5 + 72 - i] = CRGB::Black;
    if (i < vReal[6])
      leds[72 * 6 + i] = newcolorFijo;
    else
      leds[72 * 6 + i] = CRGB::Black;
    if (i < vReal[7])
      leds[72 * 7 + 72 - i] = newcolor;
    else
      leds[72 * 7 + 72 - i] = CRGB::Black;
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
    // Serial.print(vReal[f]);
    // Serial.print("-");
  }
  Serial.println("");

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

    if (i < vReal[16])
      leds[i] = newcolorFijo;
    else
      leds[i] = CRGB::Black;
    if (i < vReal[1])
      leds[36 + i] = newcolor;
    else
      leds[36 + i] = CRGB::Black;
    if (i < vReal[2])
      leds[36 * 2 + i] = newcolorFijo;
    else
      leds[36 * 2 + i] = CRGB::Black;
    if (i < vReal[3])
      leds[36 * 3 + i] = newcolor;
    else
      leds[36 * 3 + i] = CRGB::Black;
    if (i < vReal[4])
      leds[36 * 4 + i] = newcolorFijo;
    else
      leds[36 * 4 + i] = CRGB::Black;
    if (i < vReal[5])
      leds[36 * 5 + i] = newcolor;
    else
      leds[36 * 5 + i] = CRGB::Black;
    if (i < vReal[6])
      leds[36 * 6 + i] = newcolorFijo;
    else
      leds[36 * 6 + i] = CRGB::Black;
    if (i < vReal[7])
      leds[36 * 7 + i] = newcolor;
    else
      leds[36 * 7 + i] = CRGB::Black;
    if (i < vReal[8])
      leds[36 * 8 + i] = newcolorFijo;
    else
      leds[36 * 8 + i] = CRGB::Black;
    if (i < vReal[9])
      leds[36 * 9 + i] = newcolor;
    else
      leds[36 * 9 + i] = CRGB::Black;
    if (i < vReal[10])
      leds[36 * 10 + i] = newcolorFijo;
    else
      leds[36 * 10 + i] = CRGB::Black;
    if (i < vReal[11])
      leds[36 * 11 + i] = newcolor;
    else
      leds[36 * 11 + i] = CRGB::Black;
    if (i < vReal[12])
      leds[36 * 12 + i] = newcolorFijo;
    else
      leds[36 * 12 + i] = CRGB::Black;
    if (i < vReal[13])
      leds[36 * 13 + i] = newcolor;
    else
      leds[36 * 13 + i] = CRGB::Black;
    if (i < vReal[14])
      leds[36 * 14 + i] = newcolorFijo;
    else
      leds[36 * 14 + i] = CRGB::Black;
    if (i < vReal[15])
      leds[36 * 15 + i] = newcolor;
    else
      leds[36 * 15 + i] = CRGB::Black;
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

void onPower(boolean encender)
{

  if (powerOn == true && encender == false)
  {
    powerOn = false;
    bolaPowerOn = false;
    digitalWrite(PIN_BOLA_RAYOS, LOW);
    digitalWrite(PIN_RELE_B, LOW);
  }
  else
  {
    powerOn = true;

    bolaPowerOn = true;
    powerLed = true;
    rele2Power = true;
    digitalWrite(PIN_BOLA_RAYOS, HIGH);
    digitalWrite(PIN_RELE_B, HIGH);
  }
}

void executeFunction(MatrisCodigoFuncion cf)
{
  Serial.print("executeFunction.. : ");

  if (cf.funSelect == FS_vMasT)
  {
    // FRAMES_PER_SECOND = FRAMES_PER_SECOND + 10;
    Serial.println("FS_vMasT : ");
    // Serial.println(FRAMES_PER_SECOND);

    powerLed = !powerLed;
  }
  else if (cf.funSelect == FS_vMenosT)
  {
    // FRAMES_PER_SECOND =  FRAMES_PER_SECOND - 10;
    Serial.println("FS_vMenosT : ");
    // Serial.println(FRAMES_PER_SECOND);

    rele2Power = !rele2Power;
    if (rele2Power == true)
    {
      digitalWrite(PIN_RELE_B, HIGH);
    }
    else
    {
      digitalWrite(PIN_RELE_B, LOW);
    }
  }
  else if (cf.funSelect == FS_play)
  {
    bolaPowerOn = !bolaPowerOn;
    if (bolaPowerOn == true)
    {
      digitalWrite(PIN_BOLA_RAYOS, HIGH);
    }
    else
    {
      digitalWrite(PIN_BOLA_RAYOS, LOW);
    }

    Serial.println("Se cambia estado Bola plasma");
  }
  else if (cf.funSelect == FS_power)
  {
    onPower(false);

    Serial.print(" FS_power: ");
    Serial.println(powerOn);
  }
  else if (cf.funSelect == FS_colorX)
  {
    onPower(true);
    gCurrentPatternNumber = 0;
    r = cf.v1;
    g = cf.v2;
    b = cf.v3;
    static1();
    Serial.print(" FS_colorX : ");
    Serial.println(String(r) + " - " + String(g) + " - " + String(b));
  }
  else if (cf.funSelect == FS_brillo)
  {
    onPower(true);
    brillo = cf.v1;

    Serial.print(" FS_brillo: ");
    Serial.println(brillo);
  }
  else if (cf.funSelect == FS_tiempo)
  {
  }
  else if (cf.funSelect == FS_audioEfecto)
  {
    onPower(true);
    efectoSonido = cf.v1;
    digitalWrite(PIN_RELE_B, HIGH);
    gCurrentPatternNumber = 1;
    Serial.print("FS_audioEfecto : ");
    Serial.println(cf.v1);
  }
  else if (cf.funSelect == FS_efectoX)
  {
    onPower(true);
    gCurrentPatternNumber = cf.v1 + 1;

    Serial.print("FS_efectoX : ");
    Serial.println(cf.v1);
  }
}

void Task1code(void *pvParameters)
{
  Serial.println("run task...");

  for (;;)
  {

    if (powerOn == true && powerLed == true && gCurrentPatternNumber >= 0)
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

void Task2code(void *pvParameters)
{
  Serial.println("run task 2...");

  wifiMulti.addAP(ssid, pass);
  USE_SERIAL.print("iniciando wifi...");

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  for (;;)
  {

    if ((wifiMulti.run() == WL_CONNECTED))
    {

      HTTPClient http;

      USE_SERIAL.print("[HTTP] begin...\n");
      // configure traged server and url
      http.begin("http://devpablo.com/Home/EstadosLampara.php?lampId=0"); // HTTP

      // USE_SERIAL.print("[HTTP] GET...\n");
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0)
      {
        // USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
          String payload = http.getString();
          payload.trim();
          if (payload != "" && payload.length() > 0)
          {
            payload.toUpperCase();
            ;
            if (payload != "FFFFFFFF")
            {
              MatrisCodigoFuncion cf;
              cf = getData(payload);
              USE_SERIAL.print("x web: " + cf.funcionDesc + " - " + payload);

              if (cf.funSelect != FS_nada)
              {
                executeFunction(cf);
              }
            }
          }
        }
        else
        {
          USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
      }

      delay(3000);
    }

    WiFiClient client = server.available(); // Listen for incoming clients

    if (client)
    {                                // If a new client connects,
      Serial.println("New Client."); // print a message out in the serial port
      String currentLine = "";       // make a String to hold incoming data from the client
      currentTime = millis();
      previousTime = currentTime;
      while (client.connected() && currentTime - previousTime <= timeoutTime)
      { // loop while the client's connected
        currentTime = millis();
        if (client.available())
        {                         // if there's bytes to read from the client,
          char c = client.read(); // read a byte, then
          Serial.write(c);        // print it out the serial monitor

          header += c;
          if (c == '\n')
          { // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0)
            {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              header.trim();
              header.toUpperCase();

              if (header != "" && header.length() > 0 && header.substring(0, 2) == "HEX")
              {
                payload = header.substring(3, header.length() - 1);
                if (payload != "FFFFFFFF")
                {
                  MatrisCodigoFuncion cf;
                  cf = getData(payload);
                  USE_SERIAL.print("x web: " + cf.funcionDesc + " - " + payload);

                  if (cf.funSelect != FS_nada)
                    executeFunction(cf);
                }
              }

              break;
            }
            else
            { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          }
          else if (c != '\r')
          {                   // if you got anything else but a carriage return character,
            currentLine += c; // add it to the end of the currentLine
          }
        }
      }
      // Clear the header variable
      header = "";
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
    }
  
  }
}
