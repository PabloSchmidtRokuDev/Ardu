int vecLedMes[NUM_LEDS];
uint8_t gHue = 0;  // rotating "base color" used by many of the patterns

int cont10 = 0;
int cont13 = 0;

uint16_t getColor() {
  if (cont10 == 0 || cont10 == 1) {
    return CRGB::Black;
    //leds[NUM_LEDS - i] = CRGB::Black;

  } else if (cont13 == 0) {
    return CRGB::LightBlue;
    //leds[NUM_LEDS - i] = CRGB::LightBlue;

  } else if (cont13 == 1) {
    return CRGB::LightCoral;
    //leds[NUM_LEDS - i] = CRGB::LightCoral;

  } else if (cont13 == 2) {
    return CRGB::LightCyan;
    //leds[NUM_LEDS - i] = CRGB::LightCyan;

  } else if (cont13 == 3) {
    return CRGB::LightGoldenrodYellow;
    //leds[NUM_LEDS - i] = CRGB::LightGoldenrodYellow;

  } else if (cont13 == 4) {
    return CRGB::LightGreen;
    //leds[NUM_LEDS - i] = CRGB::LightGreen;

  } else if (cont13 == 5) {
    return CRGB::LightGrey;
    //leds[NUM_LEDS - i] = CRGB::LightGrey;

  } else if (cont13 == 6) {
    return CRGB::LightPink;
    //leds[NUM_LEDS - i] = CRGB::LightPink;

  } else if (cont13 == 7) {
    return CRGB::LightSalmon;
    //leds[NUM_LEDS - i] = CRGB::LightSalmon;

  } else if (cont13 == 8) {
    return CRGB::LightSeaGreen;
    //leds[NUM_LEDS - i] = CRGB::LightSeaGreen;

  } else if (cont13 == 9) {
    return CRGB::LightSkyBlue;
    //leds[NUM_LEDS - i] = CRGB::LightSkyBlue;

  } else if (cont13 == 10) {
    return CRGB::LightSlateGray;
    //leds[NUM_LEDS - i] = CRGB::LightSlateGray;

  } else if (cont13 == 11) {
    return CRGB::LightSlateGrey;
    //leds[NUM_LEDS - i] = CRGB::LightSlateGrey;

  } else if (cont13 == 12) {
    return CRGB::LightSteelBlue;
    //leds[NUM_LEDS - i] = CRGB::LightSteelBlue;

  } else if (cont13 == 13) {
    return CRGB::LightYellow;
    //leds[NUM_LEDS - i] = CRGB::LightYellow;
  }

  return CRGB::Black;
}

void endChill() {

 

  // cont10 = 0;
  // cont13 = 0;
  // Serial.println("vuelta Color led 1");
  // for (int i = 0; i < NUM_LEDS1; i++) {
  //   Serial.print("- " + String(i));
  //   leds1[i] = getColor();

  //   cont10++;
  //   if (cont10 == 31) {
  //     cont10 = 0;
  //     cont13++;

  //     if (cont13 == 14) cont13 = 0;
  //     FastLED.show();
  //     Serial.println("vuelta Color led 1");
  //   }
  // }


  // cont10 = 0;
  // cont13 = 0;
  // Serial.println("vuelta Color led 2");
  // for (int i = 0; i < NUM_LEDS2; i++) {
  //   Serial.print("- " + String(i));
  //   leds2[i] = CRGB::LightYellow;//getColor();

  //   cont10++;
  //   if (cont10 == 31) {
  //     cont10 = 0;
  //     cont13++;

  //     if (cont13 == 14) cont13 = 0;
  //     FastLED.show();
  //     Serial.println("vuelta Color led 2");
  //   }
  // }

  // cont10 = 0;
  // cont13 = 0;
  // Serial.println("led 3");
  // for (int i = 0; i < NUM_LEDS3; i++) {
  //   Serial.print("- " + String(i));
  //   leds3[i] = getColor();

  //   cont10++;
  //   if (cont10 == 31) {
  //     cont10 = 0;
  //     cont13++;

  //     if (cont13 == 14) cont13 = 0;
  //     FastLED.show();
      
  //     Serial.println("vuelta Color led 3");
  //   }
  //   //Serial.println(String(cont13));
  // }
}

// void getVectorMezclado() {

//   for (int i = 0; i < NUM_LEDS - 1; i++) { vecLedMes[i] = i; }

//   int rndn = 0;
//   int temp;

//   for (int i = 0; i < NUM_LEDS; i++) {
//     rndn = random(0, NUM_LEDS - 1);
//     temp = vecLedMes[i];
//     vecLedMes[i] = vecLedMes[rndn];
//     vecLedMes[rndn] = temp;
//   }
// }


static uint16_t sPseudotime = 0;
static uint16_t sLastMillis = 0;
static uint16_t sHue16 = 0;
static uint16_t sHue16_2 = 0;
static uint16_t sHue16_3 = 0;

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

  // for (uint16_t i = 0; i < NUM_LEDS; i++) {
  //   hue16 += hueinc16;
  //   uint8_t hue8 = hue16 / 256;

  //   brightnesstheta16 += brightnessthetainc16;
  //   uint16_t b16 = sin16(brightnesstheta16) + 32768;

  //   uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
  //   uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
  //   bri8 += (255 - brightdepth);

  //   CRGB newcolor = CHSV(hue8, sat8, bri8);

  //   nblend(leds[i], newcolor, 64);
  // }

  for (uint16_t i = 0; i < NUM_LEDS1; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    nblend(leds1[i], newcolor, 64);
  }

  for (uint16_t i = 0; i < NUM_LEDS2; i++) {
    hue16 += hueinc16 * 2;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    nblend(leds2[i], newcolor, 64);
  }



  for (uint16_t i = 0; i < NUM_LEDS3; i++) {
    hue16 += hueinc16 * 3;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    nblend(leds3[i], newcolor, 64);
  }
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
  uint16_t pixelnumber = 0;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    if (i < NUM_LEDS1) {
      pixelnumber = i;
      pixelnumber = (NUM_LEDS1 - 1) - pixelnumber;
      nblend(leds1[pixelnumber], newcolor, 64);

    } else if (i < NUM_LEDS2 + NUM_LEDS1) {
      pixelnumber = i - NUM_LEDS1;
      pixelnumber = (NUM_LEDS2 - 1) - pixelnumber;
      nblend(leds2[pixelnumber], newcolor, 64);
    } else {
      pixelnumber = i - NUM_LEDS1 - NUM_LEDS2;
      pixelnumber = (NUM_LEDS3 - 1) - pixelnumber;
      nblend(leds3[pixelnumber], newcolor, 64);
    }
  }
}

void rainbow() {

  // FastLED's built-in rainbow generator
  // fill_rainbow_circular(leds1, NUM_LEDS1, gHue, 7);
  fill_rainbow_circular(leds1, NUM_LEDS1, gHue, true);
  fill_rainbow(leds2, NUM_LEDS2, gHue, false);
  fill_rainbow_circular(leds3, NUM_LEDS3, gHue, true);
}

void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds1[random16(NUM_LEDS1)] += CRGB::White;
    leds2[random16(NUM_LEDS2)] += CRGB::White;
    leds3[random16(NUM_LEDS3)] += CRGB::White;
  }
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(255);
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds1, NUM_LEDS1, 10);
  fadeToBlackBy(leds2, NUM_LEDS2, 10);
  fadeToBlackBy(leds3, NUM_LEDS3, 10);

  int pos = random16(NUM_LEDS1);
  leds1[pos] += CHSV(gHue + random8(64), 200, 255);

  pos = random16(NUM_LEDS2);
  leds2[pos] += CHSV(gHue + random8(64), 200, 255);

  pos = random16(NUM_LEDS3);
  leds3[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds1, NUM_LEDS1, 20);
  fadeToBlackBy(leds2, NUM_LEDS2, 20);
  fadeToBlackBy(leds3, NUM_LEDS3, 20);

  int pos = beatsin16(13, 0, NUM_LEDS1 - 1);
  leds1[pos] += CHSV(gHue, 255, 192);

  pos = beatsin16(13, 0, NUM_LEDS2 - 1);
  leds2[pos] += CHSV(gHue, 255, 192);

  pos = beatsin16(13, 0, NUM_LEDS3 - 1);
  leds3[pos] += CHSV(gHue, 255, 192);


  // int pos = beatsin16(13, 0, NUM_LEDS - 1);
  // leds[pos] += CHSV(gHue, 255, 192);
}

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS1; i++) {  // 9948
    leds1[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }

  for (int i = 0; i < NUM_LEDS2; i++) {  // 9948
    leds2[i] = ColorFromPalette(palette, gHue + (i * 3), beat - gHue + (i * 10));
  }

  for (int i = 0; i < NUM_LEDS3; i++) {  // 9948
    leds3[i] = ColorFromPalette(palette, gHue + (i * 4), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds1, NUM_LEDS1, 20);
  fadeToBlackBy(leds2, NUM_LEDS2, 20);
  fadeToBlackBy(leds3, NUM_LEDS3, 20);
  uint8_t dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds1[beatsin16(i + 7, 0, NUM_LEDS1 - 1)] |= CHSV(dothue, 200, 255);
    leds2[beatsin16(i + 7, 0, NUM_LEDS2 - 1)] |= CHSV(dothue, 200, 255);
    leds3[beatsin16(i + 7, 0, NUM_LEDS3 - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


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

CRGB bgColor = CRGB(0, 0, 0);

CRGBPalette16 pacifica_palette_1 = pp9;
CRGBPalette16 pacifica_palette_2 = pp9;
CRGBPalette16 pacifica_palette_3 = pp8;

void selectPasifica(int ColorNum) {

  if (ColorNum == 0) {
    pacifica_palette_1 = pp13;
    pacifica_palette_3 = pp12;

  } else if (ColorNum == 1) {
    pacifica_palette_1 = pp12;
    pacifica_palette_3 = pp11;

  } else if (ColorNum == 2) {
    pacifica_palette_1 = pp11;
    pacifica_palette_3 = pp9;

  } else if (ColorNum == 3) {
    pacifica_palette_1 = pp9;
    pacifica_palette_3 = pp8;

  } else if (ColorNum == 4) {
    pacifica_palette_1 = pp8;
    pacifica_palette_3 = pp7;

  } else if (ColorNum == 5) {
    pacifica_palette_1 = pp7;
    pacifica_palette_3 = pp6;

  } else if (ColorNum == 6) {
    pacifica_palette_1 = pp6;
    pacifica_palette_3 = pp5;

  } else if (ColorNum == 7) {
    pacifica_palette_1 = pp5;
    pacifica_palette_3 = pp4;

  } else if (ColorNum == 8) {
    pacifica_palette_1 = pp4;
    pacifica_palette_3 = pp3;

  } else {
    pacifica_palette_1 = pp3;
    pacifica_palette_3 = pp2;
  }

  pacifica_palette_2 = pacifica_palette_1;
}


// Add one layer of waves into the led array
void pacifica_one_layer(CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff) {
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  bool flag = false;
  for (uint16_t i = 0; i < NUM_LEDS1; i++) {
    waveangle += 250;
    uint16_t s16 = sin16(waveangle) + 32768;
    uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16(ci) + 32768;
    uint8_t sindex8 = scale16(sindex16, 240);
    CRGB c = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
    leds1[i] += c;
  }

  for (uint16_t i = 0; i < NUM_LEDS2; i++) {
    waveangle += 250;
    uint16_t s16 = sin16(waveangle) + 32768;
    uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16(ci) + 32768;
    uint8_t sindex8 = scale16(sindex16, 240);
    CRGB c = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
    leds2[i] += c;
  }

  for (uint16_t i = 0; i < NUM_LEDS3; i++) {
    waveangle += 250;
    uint16_t s16 = sin16(waveangle) + 32768;
    uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16(ci) + 32768;
    uint8_t sindex8 = scale16(sindex16, 240);
    CRGB c = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
    leds3[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps() {
  uint8_t basethreshold = beatsin8(9, 55, 65);
  uint8_t wave = beat8(7);

  bool flag = false;

  for (uint16_t i = 0; i < NUM_LEDS1; i++) {
    uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
    wave += 2;
    uint8_t l = leds1[i].getAverageLight();
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      leds1[i] += CRGB(overage, overage2, qadd8(overage2, overage2));
    }
  }

  for (uint16_t i = 0; i < NUM_LEDS2; i++) {
    uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
    wave += 10;
    uint8_t l = leds2[i].getAverageLight();
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      leds2[i] += CRGB(overage, overage2, qadd8(overage2, overage2));
    }
  }

  for (uint16_t i = 0; i < NUM_LEDS3; i++) {
    uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
    wave += 20;
    uint8_t l = leds3[i].getAverageLight();
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      leds3[i] += CRGB(overage, overage2, qadd8(overage2, overage2));
    }
  }
}

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

  bool flag = false;
  for (uint16_t i = 0; i < NUM_LEDS1; i++) leds1[i] = bgColor;
  for (uint16_t i = 0; i < NUM_LEDS2; i++) leds2[i] = bgColor;
  for (uint16_t i = 0; i < NUM_LEDS3; i++) leds3[i] = bgColor;

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer(pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0 - beat16(301));
  pacifica_one_layer(pacifica_palette_2, sCIStart2, beatsin16(4, 6 * 256, 9 * 256), beatsin8(17, 40, 80), beat16(401));
  pacifica_one_layer(pacifica_palette_3, sCIStart3, 6 * 256, beatsin8(9, 10, 38), 0 - beat16(503));
  pacifica_one_layer(pacifica_palette_3, sCIStart4, 5 * 256, beatsin8(8, 10, 28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  //pacifica_deepen_colors();
}
