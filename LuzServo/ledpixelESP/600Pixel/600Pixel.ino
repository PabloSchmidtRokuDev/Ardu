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
#define NUM_LEDS 600
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
int FRAMES_PER_SECOND = 120;
CRGB leds[NUM_LEDS];

int brillo = 255;
int r, g, b;
bool powerOn = true;
bool bolaPowerOn = true;
bool rele2Power = false;
bool audioActivado = false;

#define TOTAL_EFECTOS 7
uint8_t gCurrentPatternNumber = 0;

#define PIN_LED_DATA 2
#define PIN_INFRARROJO 4


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

IRrecv irrecv(PIN_INFRARROJO); //   IRrecv object  IR get code from IR remoter
decode_results IRresults;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "PabloDev_2_4";
char pass[] = "goodlife";

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

class MatrisCodigoFuncion{
  public:
    String codigoHex = "";
    int v1 = -1;
    int v2 = -1;
    int v3 = -1;
    String funcionDesc = "Nada";
    functionSelect funSelect = FS_nada;

    void setData(String hexcode, functionSelect funX, int iv1, int iv2, int iv3, String funDesc);                       // Aqui esta ++
   
};

void MatrisCodigoFuncion::setData( String hexcode, functionSelect funX, int iv1, int iv2, int iv3, String funDesc)  {
  codigoHex = hexcode;
  funSelect = funX;
  v1 = iv1;
  v2 = iv2;
  v3 = iv3;
  funcionDesc = funDesc;
 }

void setup()
{
  r = 137;
  g = 0;
  b = 255;

  iniciarVec();

  irrecv.enableIRIn();

  // Debug console
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  Blynk.begin(auth, ssid, pass);
}



void loop()
{
  // do_IR_Tick();
  Blynk.run();
  
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

  decode_results  results;

  if (irrecv.decode(&results)) {
    String hexStr = ircodeStr(&results);
    if (hexStr != "FFFFFFFF") {
      MatrisCodigoFuncion cf;
      cf = getData(hexStr);
      Serial.println(cf.funcionDesc + " - " + hexStr);

      if (cf.funSelect != FS_nada )
      {
        executeFunction(cf);
      }
      else
      {
        Serial.println(" - Nada ");
      }
    }
    irrecv.resume();
  }
}
//************************  codigoFuncion*******************

 MatrisCodigoFuncion codigoFuncion[40];

MatrisCodigoFuncion getData(String hexCode){
  for(int i = 0; i<40; i++){
    //Serial.print("hexCode: " + hexCode);
    //Serial.println(" == codigoHex: " + codigoFuncion[i].codigoHex + " - " + String(i));
    if (codigoFuncion[i].codigoHex == hexCode){
      return codigoFuncion[i];
    }
  }
Serial.println("default " + hexCode);
  MatrisCodigoFuncion cf;
  return cf;
}


void iniciarVec() {
  codigoFuncion[0].setData("FF02FD", FS_power,  -1, -1, -1, "Btn - Power ");
  codigoFuncion[1].setData("FF827D", FS_play, -1, -1, -1, "Play");
  
  codigoFuncion[2].setData("FF3AC5", FS_vMasT, -1, -1, -1, "V+");
  codigoFuncion[3].setData("FFBA45", FS_vMenosT, -1, -1, -1, "V-");
  
  codigoFuncion[4].setData("FF1AE5", FS_colorX, 255, 0, 0, "Red");
  codigoFuncion[5].setData("FF2AD5", FS_colorX, 194, 23, 0, "Naranja fuerte");
  codigoFuncion[6].setData("FF0AF5", FS_colorX, 245, 111, 66, "Naranja medio");
  codigoFuncion[7].setData("FF38C7", FS_colorX, 230, 153, 65, "Naranja débil");
  codigoFuncion[8].setData("FF18E7", FS_colorX, 250, 250, 0, "Amarillo");
  codigoFuncion[9].setData("FF9A65", FS_colorX, 46, 133, 46, "Green");
  codigoFuncion[10].setData("FFAA55", FS_colorX, 9, 185, 9, "Verde claro");
  codigoFuncion[11].setData("FF8A75", FS_colorX, 0, 201, 204, "Celeste");
  codigoFuncion[12].setData("FFB847", FS_colorX, 20, 163, 144, "Navy");
  codigoFuncion[13].setData("FF9867", FS_colorX, 7, 115, 86, "Verde oscuro");
  codigoFuncion[14].setData("FFA25D", FS_colorX, 37, 37, 162, "Blue");
  codigoFuncion[15].setData("FF926D", FS_colorX, 31, 95, 234, "Azul claro");
  codigoFuncion[16].setData("FFB24D", FS_colorX, 97, 5, 54, "Marron");
  codigoFuncion[17].setData("FF7887", FS_colorX, 145, 8, 104, "Violeta");
  codigoFuncion[18].setData("FF58A7", FS_colorX, 194, 10, 142, "Rosado");
  codigoFuncion[19].setData("FF22DD", FS_colorX, 255, 255, 255, "White");
  codigoFuncion[20].setData("FF12ED", FS_brillo, 30, -1, -1, "Luz baja");
  codigoFuncion[21].setData("FF32CD", FS_brillo, 100, -1, -1, "Luz media");
  codigoFuncion[22].setData("FFF807", FS_brillo, 180, -1, -1, "Luz alta");
  codigoFuncion[23].setData("FFD827", FS_brillo, 255, -1, -1, "Luz total");
  codigoFuncion[24].setData("FF28D7", FS_tiempo, 30, -1, -1, "30 min");
  codigoFuncion[25].setData("FFA857", FS_tiempo, 60, -1, -1, "60 min");
  codigoFuncion[26].setData("FF6897", FS_tiempo, 90, -1, -1, "90 min");
  codigoFuncion[27].setData("FFE817", FS_tiempo, 120, -1, -1, "120 min");
  codigoFuncion[28].setData("FF08F7", FS_audioEfecto, 1, -1, -1, "audio Efecto 1");
  codigoFuncion[29].setData("FF8877", FS_audioEfecto, 2, -1, -1, "audio Efecto 2");
  codigoFuncion[30].setData("FF48B7", FS_audioEfecto, 3, -1, -1, "audio Efecto 3");
  codigoFuncion[31].setData("FFE817", FS_audioEfecto, 4, -1, -1, "audio Efecto 4");
  codigoFuncion[32].setData("FF30CF", FS_efectoX,  1, -1, -1, "Jump 3");
  codigoFuncion[33].setData("FFB04F", FS_efectoX, 2, -1, -1, "Jump 7");
  codigoFuncion[34].setData("FF708F", FS_efectoX, 3, -1, -1, "Fade 3");
  codigoFuncion[35].setData("FFF00F", FS_efectoX, 4, -1, -1, "Fade 7");
  codigoFuncion[36].setData("FF10EF", FS_efectoX, 5, -1, -1, "Quick");
  codigoFuncion[37].setData("FF906F", FS_efectoX, 6, -1, -1, "Slow");
  codigoFuncion[38].setData("FF50AF", FS_efectoX, 7, -1, -1, "Flash");
  codigoFuncion[39].setData("FFD02F", FS_efectoX, 8, -1, -1, "Auto");

}

String  ircodeStr (decode_results *results)
{

  String value = String(results->value, HEX);
  value.toUpperCase();
  return value;
}


void onPower(boolean encender) {

  if (powerOn == true && encender == false)
  {
    powerOn = false;

  }
  else
  {
    powerOn = true;

  }

}

void executeFunction(MatrisCodigoFuncion cf) {
  if (cf.funSelect == FS_vMasT) {
    FRAMES_PER_SECOND = FRAMES_PER_SECOND + 10;
    Serial.print("FS_vMasT : ");
    Serial.println(FRAMES_PER_SECOND);

  } else if (cf.funSelect == FS_vMenosT) {
    FRAMES_PER_SECOND =  FRAMES_PER_SECOND - 10;
    Serial.print("FS_vMenosT : ");
    Serial.println(FRAMES_PER_SECOND);
    
  } else if (cf.funSelect == FS_play) {

  } else if (cf.funSelect == FS_power) {
    onPower(false);

    Serial.print(" FS_power: ");
    Serial.println(powerOn);

  } else if (cf.funSelect == FS_colorX) {
    onPower(true);
    gCurrentPatternNumber = 0;
    r = cf.v1;
    g = cf.v2;
    b = cf.v3;
    static1();

    Serial.print(" FS_colorX : ");
    Serial.println(String(r) + " - " + String(g) + " - " + String(b));

  } else if (cf.funSelect == FS_brillo) {
    onPower(true);
    brillo = cf.v1;

    Serial.print(" FS_brillo: ");
    Serial.println(brillo);

  } else if (cf.funSelect == FS_tiempo) {

  } else if (cf.funSelect == FS_audioEfecto) {
   

  } else if (cf.funSelect == FS_efectoX) {
    onPower(true);
    gCurrentPatternNumber = cf.v1 +1;

    Serial.print("FS_efectoX : ");
    Serial.println(cf.v1);

  }

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
    powerOn = false;

  }
  else
  {
    powerOn = true;

  }
}


BLYNK_WRITE(V8)
{
  int Power = param.asInt();

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


  // Change Web Link Button message to "Congratulations!"
  /* Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
    Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
    Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");*/
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {sinEfecto, pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, blancoGira};
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

    FastLED.show();

    if (gCurrentPatternNumber == 9)
    {
      FastLED.delay(3);
      // delay(2);
    }
    else
    {
      FastLED.delay(1000 / FRAMES_PER_SECOND);
      delay(15);
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
