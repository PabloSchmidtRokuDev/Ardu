
#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define PIN_POWER  31
#define trigPin 47
#define echoPin 46


#define DATA_PIN    7
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    360
#define FRAMES_PER_SECOND  120
int BRIGHTNESS = 255;

//CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;

#define TOTAL_EFECTOS    6
uint8_t gCurrentPatternNumber = 0;


enum DN
{
  GO_POWER,
  GO_B_MENOS,
  GO_B_MAS,
  GO_VOL_MAS,
  GO_VOL_MENOS,
  GO_ADELANTE,
  GO_ATRAS,
  GO_RGB,
  GO_WARM,
  GO_COOL,
  GO_MODE,
  GO_COLOR_RGB,
  DEF
}

Drive_Num = DEF;
DN Last_Drive_Num = DEF;


bool powerOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
  .setCorrection(TypicalLEDStrip)
  .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(0);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = CRGB( 0, 0, 0);
  }
  FastLED.show();
}

int modeCount = 0;
void loop() {
  do_IR_Tick();

  if (Drive_Num == GO_POWER) {
    powerOn = !powerOn;

    if (powerOn == true) {
      digitalWrite(PIN_POWER, HIGH);
      Serial.print("Enciendo leds: ");
    }
    else
    {
      digitalWrite(PIN_POWER, LOW);
      for ( int i = 0; i < NUM_LEDS; i++) { //9948
        leds[i] = CRGB( 0, 0, 0);
      }
      FastLED.show();
      Serial.print("apagado leds: ");
    }

  } else if (Drive_Num == GO_B_MENOS) {

  } else if (Drive_Num == GO_B_MAS) {

  } else if (Drive_Num == GO_VOL_MAS) {
    if (BRIGHTNESS < 256) BRIGHTNESS++;
    FastLED.setBrightness(BRIGHTNESS);
    Serial.print(" Subir brillo -> " + String(BRIGHTNESS));

  } else if (Drive_Num == GO_VOL_MENOS) {
    if (BRIGHTNESS > 0) BRIGHTNESS--;
    FastLED.setBrightness(BRIGHTNESS);
    Serial.print(" Bajar brillo -> " + String(BRIGHTNESS));

  } else if (Drive_Num == GO_ADELANTE) {
    if (gCurrentPatternNumber < TOTAL_EFECTOS) {
      gCurrentPatternNumber++;
    }
    else {
      gCurrentPatternNumber = 0;
    }

    Serial.print(" Adelante -> selectEfecto: " + String(gCurrentPatternNumber));
  } else if (Drive_Num == GO_ATRAS) {
    if (gCurrentPatternNumber == 0)
    {
      gCurrentPatternNumber = TOTAL_EFECTOS;
    }
    else
    {
      gCurrentPatternNumber--;
    }

    Serial.print(" Atras -> selectEfecto: " + String(gCurrentPatternNumber));

  } else if (Drive_Num == GO_RGB) {

  } else if (Drive_Num == GO_WARM) {

  } else if (Drive_Num == GO_MODE) {

    if (modeCount == TOTAL_EFECTOS)
    {
      Serial.print("modo 0 ");
      modeCount = 0;
    }
    else
    {
      Serial.print("suma u modo: ");
      modeCount++;
    }
    
    if (modeCount == 0)
    {
        powerOn = false;
        digitalWrite(PIN_POWER, LOW);
        for ( int i = 0; i < NUM_LEDS; i++) { //9948
          leds[i] = CRGB( 0, 0, 0);
        }
        FastLED.show();
        Serial.print("apagado leds: ");
      }else if (powerOn == false) {
        powerOn = true;
        digitalWrite(PIN_POWER, HIGH);
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
        Serial.print("Enciendo leds: ");
      }
   
  Serial.print("seleccioa modo ");
    if (modeCount == 1) {
      leds.fadeToBlackBy(40);
      gCurrentPatternNumber = 1;
      Serial.print("efecto 1 ");

    } else if (modeCount == 2) {
      leds.fadeToBlackBy(40);
      gCurrentPatternNumber = 2;
      Serial.print("efecto 2 ");

    } else if (modeCount == 3) {
      leds.fadeToBlackBy(40);
      gCurrentPatternNumber = 3;
      Serial.print("efecto 3 ");

    } else if (modeCount == 4) {
      leds.fadeToBlackBy(40);
      gCurrentPatternNumber = 4;
      Serial.print("efecto 4 ");

    } else if (modeCount == 5) {
      leds.fadeToBlackBy(40);
      gCurrentPatternNumber = 5;
      Serial.print("efecto 5 ");

    } else if (modeCount == 6) {
      leds.fadeToBlackBy(40);
      gCurrentPatternNumber = 6;
      Serial.print("efecto 6 ");

    }


  }

  Drive_Num = DEF;

  if (powerOn == true) {
    selectEfecto();
  }

}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void selectEfecto() {
  gPatterns[gCurrentPatternNumber]();

  if (gCurrentPatternNumber >= 1 && gCurrentPatternNumber <= 6) {
    FastLED.show();
    // insert a delay to keep the framerate modest
    //FastLED.delay(1000 / FRAMES_PER_SECOND);

    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) {
      gHue++;  // slowly cycle the "base color" through the rainbow
    }
    //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
  }
  else {
    FastLED.show();
  }

}


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}


void pride()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend( leds[pixelnumber], newcolor, 64);
  }
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(255);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
    leds[ random16(NUM_LEDS) ] += CRGB::White;
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

bool dis_corta = false;
int dis_larga = 140;

int Limite = 140 ;

int prevOUT_8 = HIGH;

int distancia2 = 0;

void do_IR_Tick()
{

  long duracion, distancia ;

  digitalWrite(trigPin, LOW);        // Nos aseguramos de que el trigger está desactivado
  delayMicroseconds(2);              // Para asegurarnos de que el trigger esta LOW
  digitalWrite(trigPin, HIGH);       // Activamos el pulso de salida
  delayMicroseconds(10);             // Esperamos 10µs. El pulso sigue active este tiempo
  digitalWrite(trigPin, LOW);        // Cortamos el pulso y a esperar el echo

  duracion = pulseIn(echoPin, HIGH) ;
  distancia = duracion / 2 / 29.1  ;
  if (distancia > Limite) distancia = Limite;
  //Serial.println(String(distancia) + " distancia") ;
  /*
    digitalWrite(trigPin2, LOW);        // Nos aseguramos de que el trigger está desactivado
    delayMicroseconds(2);              // Para asegurarnos de que el trigger esta LOW
    digitalWrite(trigPin2, HIGH);       // Activamos el pulso de salida
    delayMicroseconds(10);             // Esperamos 10µs. El pulso sigue active este tiempo
    digitalWrite(trigPin2, LOW);        // Cortamos el pulso y a esperar el echo

    duracion = pulseIn(echoPin2, HIGH) ;
    distancia2 = duracion / 2 / 29.1  ;

    if (distancia2 >=10 && distancia2 <= 130){
        calcularRGB(distancia2);
        Serial.println(String(distancia2) + " distancia2 - r: " + String(colorR) + " -g: "+String(colorG) + " - B: "+String(colorB)) ;
        Drive_Num = GO_COLOR_RGB;
    }
    else*/

  if (distancia < Limite) {

    if (distancia < 15) {
      Serial.println(String(distancia) + " distancia corta") ;
      dis_corta = true;

    } else if (distancia > 15 && distancia <= 55) {
      Serial.println(String(distancia) + " distancia larga") ;
      dis_larga = (distancia - 15) * 5;
      Serial.println(String(dis_larga) + " brillo") ;
      FastLED.setBrightness(dis_larga);

    } else if (distancia > 60 && distancia <= 100 && dis_larga != 255) {
      Serial.println(String(distancia) + " distancia max") ;
      Serial.println("brillo maximo");
      FastLED.setBrightness(255);
      dis_larga  = 255;

    }
  }
  else {

    if (dis_corta == true) {
      Serial.println(String(distancia) + " distancia free") ;
      Serial.println("nextModo");
      Drive_Num = GO_MODE;
      dis_corta = false;
    }

    dis_larga  = 255;
  }

}

/*
  void calcularRGB(int distancia2) {
  distancia2 = distancia2 - 15;
  if (distancia2 < 0) distancia2 = 0;


  if (distancia2 < 15) {
    colorR = 255;
    colorG = 0;
    colorB = distancia2 * 17;

  } else if (distancia2 <= 30) {
    distancia2 = distancia2 - 15;
    colorR = 255 - (distancia2 * 17);;
    colorG = 0;
    colorB = 255;

  } else if (distancia2 <= 45) {
    distancia2 = distancia2 - 30;
    colorR = 0;
    colorG = distancia2 * 17;
    colorB = 255;

  } else if (distancia2 <= 60) {
    distancia2 = distancia2 - 45;
    colorR = 0;
    colorG = 255;
    colorB = 255 - (distancia2 * 17);

  } else if (distancia2 <= 75) {
    distancia2 = distancia2 - 60;
    colorR = distancia2 * 17;
    colorG = 255;
    colorB = 0;

  } else if (distancia2 <= 80) {
    distancia2 = distancia2 - 75;
    colorR = 255;
    colorG = 255 - (distancia2 * 17);
    colorB = 0;

  } else if (distancia2 <= 95) {
    distancia2 = distancia2 - 80;
    colorR = 255;
    colorG = 0;
    colorB = 0;

  } else {
    colorR = 0;
    colorG = 0;
    colorB = 0;
  }
  }*/
