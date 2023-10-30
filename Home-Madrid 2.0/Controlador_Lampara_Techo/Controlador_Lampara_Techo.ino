/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#include "arduinoFFT.h"

#include <Arduino.h>
#include "FastLED.h"

#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

#define DEBUG_SERIAL Serial

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

#define PIN_LED_DATA 5
#define PIN_RELE_B 26
#define PIN_BOLA_RAYOS 25

#define PIN_MICROFONNO 33

// setting PWM properties
const int freq = 2000;
const int ledChannel = 0;
const int resolution = 8;
arduinoFFT FFT = arduinoFFT();

const char* ssid = "ControlMaestrox";
const char* password = "goodlife";

unsigned long messageInterval = 5000;
bool connected = false;

#define DEBUG_SERIAL Serial

TaskHandle_t Task1;
TaskHandle_t Task2;

//************************  class*******************
enum functionSelect {
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

class MatrisCodigoFuncion {
public:
  String codigoHex = "";
  int v1 = -1;
  int v2 = -1;
  int v3 = -1;
  String funcionDesc = "Nada";
  functionSelect funSelect = FS_nada;

  void setData(String hexcode, functionSelect funX, int iv1, int iv2, int iv3, String funDesc);  // Aqui esta ++
};

void MatrisCodigoFuncion::setData(String hexcode, functionSelect funX, int iv1, int iv2, int iv3, String funDesc) {
  codigoHex = hexcode;
  funSelect = funX;
  v1 = iv1;
  v2 = iv2;
  v3 = iv3;
  funcionDesc = funDesc;
}

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("iniciando....");
  r = 10;
  g = 10;
  b = 10;

  iniciarVec();

  pinMode(PIN_BOLA_RAYOS, OUTPUT);
  digitalWrite(PIN_BOLA_RAYOS, LOW);

  pinMode(PIN_RELE_B, OUTPUT);
  digitalWrite(PIN_RELE_B, HIGH);
  Serial.println("cargando leds");

  FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    leds[whiteLed] = CRGB::Black;
  }

  FastLED.show();

  xTaskCreatePinnedToCore(
    Task1code,
    "Task1",
    10000,
    NULL,
    1,
    &Task1,
    0);
  delay(500);

  xTaskCreatePinnedToCore(
    Task2code,
    "Task2",
    10000,
    NULL,
    1,
    &Task2,
    1);

  delay(500);
  Serial.println("setup complete.");
}

void loop() {
}


void checkHeader(String dat) {
  dat.trim();
  dat.toUpperCase();

  if (dat != "" && dat.length() > 0 && dat.substring(0, 3) == "HEX") {
    String payload = dat.substring(3);
    if (payload != "FFFFFFFF") {
      MatrisCodigoFuncion cf;
      cf = getData(payload);
      Serial.println("x wifi: " + cf.funcionDesc + " - " + payload);

      if (cf.funSelect != FS_nada)
        executeFunction(cf);
    }
  }
}

//************************  codigoFuncion*******************

MatrisCodigoFuncion codigoFuncion[40];

MatrisCodigoFuncion getData(String hexCode) {
  for (int i = 0; i < 40; i++) {
    if (codigoFuncion[i].codigoHex == hexCode) {
      return codigoFuncion[i];
    }
  }
  Serial.println("default " + hexCode);
  MatrisCodigoFuncion cf;
  return cf;
}

void iniciarVec() {
  codigoFuncion[0].setData("BF40FF00", FS_power, -1, -1, -1, "Btn - Power ");
  codigoFuncion[1].setData("BE41FF00", FS_play, -1, -1, -1, "Play");

  codigoFuncion[2].setData("A35CFF00", FS_vMasT, -1, -1, -1, "V+");
  codigoFuncion[3].setData("A25DFF00", FS_vMenosT, -1, -1, -1, "V-");

  codigoFuncion[4].setData("A758FF00", FS_colorX, 255, 0, 0, "Red");
  codigoFuncion[5].setData("AB54FF00", FS_colorX, 194, 23, 0, "OrangeRed");
  codigoFuncion[6].setData("AF50FF00", FS_colorX, 245, 111, 66, "Salmon");
  codigoFuncion[7].setData("E31CFF00", FS_colorX, 230, 153, 65, "LightSalmon");
  codigoFuncion[8].setData("E718FF00", FS_colorX, 250, 250, 0, "LightYellow");

  codigoFuncion[9].setData("A659FF00", FS_colorX, 46, 133, 46, "LimeGreen");
  codigoFuncion[10].setData("AA55FF00", FS_colorX, 9, 185, 9, "LightGreen");
  codigoFuncion[11].setData("AE51FF00", FS_colorX, 0, 201, 204, "CadetBlue");
  codigoFuncion[12].setData("E21DFF00", FS_colorX, 20, 163, 144, "Coral");
  codigoFuncion[13].setData("E619FF00", FS_colorX, 7, 115, 86, "DarkGreen");

  codigoFuncion[14].setData("BA45FF00", FS_colorX, 37, 37, 162, "Blue");
  codigoFuncion[15].setData("B649FF00", FS_colorX, 31, 95, 234, "MediumBlue");
  codigoFuncion[16].setData("B24DFF00", FS_colorX, 97, 5, 54, "Maroon");
  codigoFuncion[17].setData("E11EFF00", FS_colorX, 145, 8, 104, "Violet");
  codigoFuncion[18].setData("E51AFF00", FS_colorX, 194, 10, 142, "Pink");

  codigoFuncion[19].setData("BB44FF00", FS_colorX, 255, 255, 255, "GhostWhite");
  codigoFuncion[20].setData("B748FF00", FS_brillo, 20, -1, -1, "Luz baja");
  codigoFuncion[21].setData("B34CFF00", FS_brillo, 100, -1, -1, "Luz media");
  codigoFuncion[22].setData("E01FFF00", FS_brillo, 180, -1, -1, "Luz alta");
  codigoFuncion[23].setData("E41BFF00", FS_brillo, 255, -1, -1, "Luz total");

  codigoFuncion[24].setData("EB14FF00", FS_tiempo, 100, -1, -1, "30 min");
  codigoFuncion[25].setData("EA15FF00", FS_tiempo, 70, -1, -1, "60 min");
  codigoFuncion[26].setData("E916FF00", FS_tiempo, 40, -1, -1, "90 min");
  codigoFuncion[27].setData("E817FF00", FS_tiempo, 15, -1, -1, "120 min");

  codigoFuncion[28].setData("EF10FF00", FS_audioEfecto, 1, -1, -1, "audio Efecto 1");
  codigoFuncion[29].setData("EE11FF00", FS_audioEfecto, 2, -1, -1, "audio Efecto 2");
  codigoFuncion[30].setData("ED12FF00", FS_audioEfecto, 3, -1, -1, "audio Efecto 3");
  codigoFuncion[31].setData("EC13FF00", FS_audioEfecto, 4, -1, -1, "audio Efecto 4");

  codigoFuncion[32].setData("F30CFF00", FS_efectoX, 1, -1, -1, "Jump 3");
  codigoFuncion[33].setData("F20DFF00", FS_efectoX, 2, -1, -1, "Jump 7");
  codigoFuncion[34].setData("F10EFF00", FS_efectoX, 3, -1, -1, "Fade 3");
  codigoFuncion[35].setData("F00FFF00", FS_efectoX, 4, -1, -1, "Fade 7");
  codigoFuncion[36].setData("F708FF00", FS_efectoX, 5, -1, -1, "Quick");
  codigoFuncion[37].setData("F609FF00", FS_efectoX, 6, -1, -1, "Slow");
  codigoFuncion[38].setData("F50AFF00", FS_efectoX, 7, -1, -1, "Flash");
  codigoFuncion[39].setData("F40BFF00", FS_efectoX, 8, -1, -1, "Auto");
}



int vecLedMes[NUM_LEDS];

void getVectorMezclado() {

  for (int i = 0; i < NUM_LEDS - 1; i++) { vecLedMes[i] = i; }

  int rndn = 0;
  int temp;

  for (int i = 0; i < NUM_LEDS; i++) {
    rndn = random(0, NUM_LEDS - 1);
    temp = vecLedMes[i];
    vecLedMes[i] = vecLedMes[rndn];
    vecLedMes[rndn] = temp;
  }
}

void static1() {
  if (powerOn == true && gCurrentPatternNumber == 0) {

    FastLED.setBrightness(brillo);

    getVectorMezclado();
    int cont = 0;

    for (int i = 0; i < NUM_LEDS; i++) {
      //Serial.print("i: "+String(i)  +  " - vm:  "+ String(vecLedMes[i]));
      leds[vecLedMes[i]] = CRGB(r, g, b);
      //Serial.println(" -- Aplicado");
      cont++;

      if (cont == 30) {
        FastLED.show();
        //delay(10);
        FastLED.delay(1000 / FRAMES_PER_SECOND);
        cont = 0;
      }
    }
    FastLED.show();
  }
}

void static1ColorName(String ColorName) {
  if (powerOn == true && gCurrentPatternNumber == 0) {

    FastLED.setBrightness(brillo);

    CRGB sColor = CRGB::Black;

    if (ColorName == "Red") { sColor = CRGB::Red; }
    if (ColorName == "OrangeRed") { sColor = CRGB::OrangeRed; }
    if (ColorName == "Salmon") { sColor = CRGB::Salmon; }
    if (ColorName == "LightSalmon") { sColor = CRGB::LightSalmon; }
    if (ColorName == "LightYellow") { sColor = CRGB::LightYellow; }
    if (ColorName == "LimeGreen") { sColor = CRGB::LimeGreen; }
    if (ColorName == "LightGreen") { sColor = CRGB::LightGreen; }
    if (ColorName == "CadetBlue") { sColor = CRGB::CadetBlue; }
    if (ColorName == "Coral") { sColor = CRGB::Coral; }
    if (ColorName == "DarkGreen") { sColor = CRGB::DarkGreen; }
    if (ColorName == "Blue") { sColor = CRGB::Blue; }
    if (ColorName == "MediumBlue") { sColor = CRGB::MediumBlue; }
    if (ColorName == "Maroon") { sColor = CRGB::Maroon; }
    if (ColorName == "Violet") { sColor = CRGB::Violet; }
    if (ColorName == "Pink") { sColor = CRGB::Pink; }
    if (ColorName == "GhostWhite") { sColor = CRGB::GhostWhite; }

    FastLED.delay(1000 / FRAMES_PER_SECOND);
    getVectorMezclado();
    int cont = 0;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[vecLedMes[i]] = sColor;
      cont++;

      if (cont == 30) {
        FastLED.show();
        // delay(10);
        FastLED.delay(1000 / FRAMES_PER_SECOND);
        cont = 0;
      }
    }

    FastLED.show();
  }
}


typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { sinEfecto, efectoAudio, pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, blancoGira };
uint8_t gHue = 0;  // rotating "base color" used by many of the patterns

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void selectEfecto() {

  gPatterns[gCurrentPatternNumber]();
  if (gCurrentPatternNumber > 0) {

    if (gCurrentPatternNumber == 1) {
      digitalWrite(PIN_BOLA_RAYOS, HIGH);
      digitalWrite(PIN_RELE_B, HIGH);
      bolaPowerOn = true;
    } else{
    FastLED.show();

    FastLED.delay(1000 / FRAMES_PER_SECOND);
}
    // do some periodic updates
    EVERY_N_MILLISECONDS(20) {
      gHue++;  // slowly cycle the "base color" through the rainbow
    }
    


  } else {
    delay(100);
  }
}

void sinEfecto() {
  delay(100);
}

int posBlanco = 0;

static uint16_t sPseudotime = 0;
static uint16_t sLastMillis = 0;
static uint16_t sHue16 = 0;

void blancoGira() {


  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  //uint8_t msmultiplier = beatsin88(147, 23, 60);
  uint8_t msmultiplier = beatsin88(50, 100, 150);

  uint16_t hue16 = sHue16;  // gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    nblend(leds[i], newcolor, 64);
    // uint16_t pixelnumber = i;

    //if (i >= posBlanco && i <= posBlanco + 500) {
    //  nblend(leds[getRealPosition(pixelnumber)], newcolor, 64);
    // } else {
    // nblend(leds[getRealPosition(pixelnumber)], CRGB::Black, 64);
    //}
  }

  if (NUM_LEDS <= posBlanco) {
    posBlanco = 0;
  } else {
    posBlanco++;
  }
}

int getRealPosition(int ledPos) {
  if (NUM_LEDS <= ledPos) {
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
    } else {
      leds[multip * posicion + indice] = CRGB::Black;
    }
  }
}

void efectoAudio() {
  Serial.println("efectoAudio: " + (String)efectoSonido);
  if (efectoSonido == 2) {
    efectoAudio2();
  } else if (efectoSonido == 3) {
    efectoAudio3();
  } else if (efectoSonido == 4) {
    efectoAudio4();
  } else {
    doubleRainbow();
  }
}

void efectoAudio2() {
  for (int j = 0; j < samples; j++) {
    vReal[j] = analogRead(PIN_MICROFONNO);
    Serial.println(vReal[j]);
    vImag[j] = 0;
  }

  // calculo FFT

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, samples);

  for (int f = 0; f < samples; f++) {
    vReal[f] = map(vReal[f], 0, 4095, 0, 144);
  }

  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 10, 255);

  if (contador == 1000 || contador == 0) {
    newcolorFijo = ColorFromPalette(palette, gHue + (random(0, 255) * 2), beat - gHue + (random(0, 255) * 10));
    contador = 0;
  }

  contador++;

  for (int i = 0; i < 144; i++) {
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

void efectoAudio3() {
  for (int j = 0; j < samples; j++) {
    vReal[j] = analogRead(PIN_MICROFONNO);
    // Serial.println(vReal[j]);
    vImag[j] = 0;
  }

  // calculo FFT

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, samples);

  for (int f = 0; f < samples; f++) {
    vReal[f] = map(vReal[f], 0, 4095, 0, 144);
  }

  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 10, 255);

  if (contador == 100 || contador == 0) {
    newcolorFijo = ColorFromPalette(palette, gHue + (random(0, 255) * 2), beat - gHue + (random(0, 255) * 10));
    contador = 0;
  }

  contador++;

  for (int i = 0; i < 144; i++) {

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

void efectoAudio4() {
  for (int j = 0; j < samples; j++) {
    vReal[j] = analogRead(PIN_MICROFONNO);
    // Serial.println(vReal[j]);
    vImag[j] = 0;
  }

  // calculo FFT

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, samples);

  for (int f = 0; f < samples; f++) {
    vReal[f] = map(vReal[f], 0, 4095, 0, 72);
  }

  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 10, 255);

  if (contador == 100 || contador == 0) {
    newcolorFijo = ColorFromPalette(palette, gHue + (random(0, 255) * 2), beat - gHue + (random(0, 255) * 10));
    contador = 0;
  }

  contador++;

  for (int i = 0; i < 72; i++) {

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

String audioData= "";
void doubleRainbow() {
  for (int j = 0; j < samples; j++) {
    vReal[j] = analogRead(PIN_MICROFONNO);
     Serial.print("e4 - ");
     Serial.println(vReal[j]);
    vImag[j] = 0;
     delayMicroseconds(1);
  }

  // calculo FFT
  Serial.println("calculo");

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, samples);

  Serial.println("reduzco ");

audioData = "";
  for (int f = 0; f < samples; f++) {
    vReal[f] = map(vReal[f], 0, 5000, 0, 36);
    audioData+= String(vReal[f])+"!";
    // Serial.print(vReal[f]);
    // Serial.print("-");
  }
  Serial.println("calculo colores ");

  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 10, 255);

  if (contador == 100 || contador == 0) {
    newcolorFijo = ColorFromPalette(palette, gHue + (random(0, 255) * 2), beat - gHue + (random(0, 255) * 10));
    contador = 0;
  }

  contador++;

  if (connected)  webSocket.sendTXT("#audioData|"+audioData);

  for (int i = 0; i < 36; i++) {

    CRGB newcolor = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    gHue++;

    if (i < vReal[15])
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

      FastLED.show(); 
//delayMicroseconds(10);
    FastLED.delay(15);

  //Serial.println("fin fun..");
}

void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;  // gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
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

void rainbow() {

  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(255);
}

void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
    leds[random16(NUM_LEDS)] += CRGB::White;
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(gHue, 255, 192);
}

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) {  // 9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void onPower(boolean encender) {

  if (powerOn == true && encender == false) {
    powerOn = false;
    bolaPowerOn = false;
    digitalWrite(PIN_BOLA_RAYOS, LOW);
    digitalWrite(PIN_RELE_B, LOW);
  } else {
    powerOn = true;

    bolaPowerOn = true;
    powerLed = true;
    rele2Power = true;
    //digitalWrite(PIN_BOLA_RAYOS, HIGH);
    //digitalWrite(PIN_RELE_B, HIGH);
  }
}

void executeFunction(MatrisCodigoFuncion cf) {
  Serial.print("executeFunction.. : ");

  if (cf.funSelect == FS_vMasT) {
    Serial.println("FS_vMasT : power Led");
    powerLed = !powerLed;

  } else if (cf.funSelect == FS_vMenosT) {
    Serial.println("FS_vMenosT : power UV");
    rele2Power = !rele2Power;

    if (rele2Power == true) {
      digitalWrite(PIN_RELE_B, HIGH);
    } else {
      digitalWrite(PIN_RELE_B, LOW);
    }

  } else if (cf.funSelect == FS_play) {
    bolaPowerOn = !bolaPowerOn;
    if (bolaPowerOn == true) {
      digitalWrite(PIN_BOLA_RAYOS, HIGH);
    } else {
      digitalWrite(PIN_BOLA_RAYOS, LOW);
    }

    Serial.println("Se cambia estado Bola plasma");

  } else if (cf.funSelect == FS_power) {
    onPower(false);

    Serial.print(" FS_power: ");
    Serial.println(powerOn);

  } else if (cf.funSelect == FS_colorX) {
    onPower(true);
    gCurrentPatternNumber = 0;


    static1ColorName(cf.funcionDesc);
    Serial.print(" FS_colorX : ");
    Serial.println(String(r) + " - " + String(g) + " - " + String(b));

  } else if (cf.funSelect == FS_brillo) {
    onPower(true);
    brillo = cf.v1;

    Serial.print(" FS_brillo: ");
    Serial.println(brillo);
    FastLED.setBrightness(brillo);
    FastLED.show();

  } else if (cf.funSelect == FS_tiempo) {
  } else if (cf.funSelect == FS_audioEfecto) {

    onPower(true);
    efectoSonido = cf.v1;
    digitalWrite(PIN_RELE_B, HIGH);
    gCurrentPatternNumber = 1;
    Serial.print("FS_audioEfecto : ");
    Serial.println(cf.v1);

  } else if (cf.funSelect == FS_efectoX) {
    onPower(true);
    gCurrentPatternNumber = cf.v1 + 1;

    Serial.print("FS_efectoX : ");
    Serial.println(cf.v1);
  }
}


void Task1code(void* pvParameters) {
  Serial.println("run task...");

  for (;;) {

    if (powerOn == true && powerLed == true && gCurrentPatternNumber >= 0) {
      selectEfecto();
    } else {
      for (int i = 0; i <= NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
    }
  }
}

void checkData(String dataAvailable) {
  Serial.println("checkData: " + dataAvailable.substring(0, 3));
  if (dataAvailable != "" && dataAvailable.length() > 0 && dataAvailable.substring(0, 3) == "HEX") {
    String payload = dataAvailable.substring(3);
    Serial.println("checkData - payload: " + payload);
    if (payload != "FFFFFFFF") {
      MatrisCodigoFuncion cf;
      cf = getData(payload);
      Serial.println("x web: " + cf.funcionDesc + " - " + payload);
      if (cf.funSelect != FS_nada)
        executeFunction(cf);
    }
  }
}

void hexdump(const void* mem, uint32_t len, uint8_t cols = 16) {
  const uint8_t* src = (const uint8_t*)mem;
  DEBUG_SERIAL.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for (uint32_t i = 0; i < len; i++) {
    if (i % cols == 0) {
      DEBUG_SERIAL.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    DEBUG_SERIAL.printf("%02X ", *src);
    src++;
  }
  DEBUG_SERIAL.printf("\n");
}

String strPayload;
String primerCaracter;
String strCommando;
int cont = 0;
void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      DEBUG_SERIAL.printf("[WSc] Disconnected!\n");
     
      connected = false;
      break;
    case WStype_CONNECTED:
      {
        DEBUG_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
        connected = true;

        // send message to server when Connected
        DEBUG_SERIAL.println("[WSc] SENT: Connected");
        webSocket.sendTXT("#ID_NAME|living");
      }
      break;
    case WStype_TEXT:
      strPayload = (char*)payload;
      primerCaracter = strPayload.substring(0, 1);

      DEBUG_SERIAL.println("texto: " + strPayload + " - 1er: " + primerCaracter);
      if (primerCaracter == "#") {

        strPayload = strPayload.substring(1);

        strCommando = strPayload.substring(0, strPayload.indexOf('|'));
        strPayload = strPayload.substring(strPayload.indexOf('|') + 1);

        DEBUG_SERIAL.println("strCommando: " + strCommando + " - msj: " + strPayload);

        if (strCommando == "MSJ_RECIBIDO") {
          String msj = "#MSJ_RECIBIDO| " + strPayload;
          webSocket.sendTXT(msj);
          checkData(strPayload);
        }

      } else {
        DEBUG_SERIAL.println("Simple mensaje recibido: " + strPayload);
      }

      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      DEBUG_SERIAL.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);
      break;
    case WStype_PING:
      // pong will be send automatically
      DEBUG_SERIAL.printf("[WSc] get ping\n");
      break;
    case WStype_PONG:
      // answer to a ping we send
      DEBUG_SERIAL.printf("[WSc] get pong\n");
      break;
  }
}

void Task2code(void* pvParameters) {

  // Serial.print("Connecting to ");
  // Serial.println(ssid);

  // WiFi.softAPConfig(ip, gateway, subnet);

  // while (!WiFi.softAP(ssid, password)) {
  //   Serial.println(".");
  //   delay(100);
  // }

  // Serial.println("");
  // Serial.print("Iniciado AP:\t");
  // Serial.println(ssid);
  // Serial.print("IP address:\t");
  // Serial.println(WiFi.softAPIP());
  // server.begin();

  DEBUG_SERIAL.begin(115200);

  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    DEBUG_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    DEBUG_SERIAL.flush();
    delay(500);
  }

DEBUG_SERIAL.println("Conectando...");
  WiFi.begin(ssid, password);

  cont = 0;
  bool flag = true;
int pos  = 0;
  while (WiFi.status() != WL_CONNECTED) {
    

        if (cont == 3) {
          leds[pos] = CRGB::Gold;
          cont = 0;
        } else {
          leds[pos] = CRGB::Black;
          cont++;
        }
        FastLED.show();

pos++;
    if (pos >= NUM_LEDS) pos=0;

    FastLED.show();

    delay(500);
    Serial.print(".");
  }

  DEBUG_SERIAL.print("Local IP: ");
  DEBUG_SERIAL.println(WiFi.localIP());
  // server address, port and URL
  webSocket.begin("192.168.1.150", 81, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  for (;;) {
    webSocket.loop();
  }
}