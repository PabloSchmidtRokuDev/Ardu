arduinoFFT FFT = arduinoFFT();
bool audioActivado = false;
int efectoSonido = 1;

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

int LedsXFranjas = 87;

String audioData = "";
void doubleRainbow() {
  audioData = "";
  for (int j = 0; j < samples; j++) {
    vReal[j] = analogRead(PIN_MIC);
    audioData += String(vReal[j]);
    audioData += "\t";
    vImag[j] = 0;
     delayMicroseconds(5);
  }

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, samples);


  for (int f = 0; f < samples; f++) {
    vReal[f] = map(vReal[f], 0, 4095, 0, LedsXFranjas);
  }

  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 10, 255);

  if (contador == 100 || contador == 0) {
    newcolorFijo = ColorFromPalette(palette, gHue + (random(0, 255) * 2), beat - gHue + (random(0, 255) * 10));
    contador = 0;
    Serial.println(audioData);
  }

  contador++;

  // if (connected)  webSocket.sendTXT("#audioData|"+audioData);

  for (int i = 0; i <= LedsXFranjas; i++) {

    CRGB newcolor = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    gHue++;
    // for (int j = 0; j <= 14; j++) {

    //   if (i < vReal[j + 1])
    //     leds[LedsXFranjas * j + i] = newcolorFijo;
    //   else
    //     leds[LedsXFranjas * j + i] = CRGB::Black;
    // }

    // if (i < vReal[15])
    //   leds[i] = newcolorFijo;
    // else
    //   leds[i] = CRGB::Black;

    // if (i < vReal[1])
    //   leds[LedsXFranjas + i] = newcolor;
    // else
    //   leds[LedsXFranjas + i] = CRGB::Black;

    // if (i < vReal[2])
    //   leds[LedsXFranjas * 2 + i] = newcolorFijo;
    // else
    //   leds[LedsXFranjas * 2 + i] = CRGB::Black;

    // if (i < vReal[3])
    //   leds[LedsXFranjas * 3 + i] = newcolor;
    // else
    //   leds[LedsXFranjas * 3 + i] = CRGB::Black;
    // if (i < vReal[4])
    //   leds[LedsXFranjas * 4 + i] = newcolorFijo;
    // else
    //   leds[LedsXFranjas * 4 + i] = CRGB::Black;
    // if (i < vReal[5])
    //   leds[LedsXFranjas * 5 + i] = newcolor;
    // else
    //   leds[LedsXFranjas * 5 + i] = CRGB::Black;
    // if (i < vReal[6])
    //   leds[LedsXFranjas * 6 + i] = newcolorFijo;
    // else
    //   leds[LedsXFranjas * 6 + i] = CRGB::Black;
    // if (i < vReal[7])
    //   leds[LedsXFranjas * 7 + i] = newcolor;
    // else
    //   leds[LedsXFranjas * 7 + i] = CRGB::Black;
    // if (i < vReal[8])
    //   leds[LedsXFranjas * 8 + i] = newcolorFijo;
    // else
    //   leds[LedsXFranjas * 8 + i] = CRGB::Black;
    // if (i < vReal[9])
    //   leds[LedsXFranjas * 9 + i] = newcolor;
    // else
    //   leds[LedsXFranjas * 9 + i] = CRGB::Black;
    // if (i < vReal[10])
    //   leds[LedsXFranjas * 10 + i] = newcolorFijo;
    // else
    //   leds[LedsXFranjas * 10 + i] = CRGB::Black;
    // if (i < vReal[11])
    //   leds[LedsXFranjas * 11 + i] = newcolor;
    // else
    //   leds[LedsXFranjas * 11 + i] = CRGB::Black;
    // if (i < vReal[12])
    //   leds[LedsXFranjas * 12 + i] = newcolorFijo;
    // else
    //   leds[LedsXFranjas * 12 + i] = CRGB::Black;
    // if (i < vReal[13])
    //   leds[LedsXFranjas * 13 + i] = newcolor;
    // else
    //   leds[LedsXFranjas * 13 + i] = CRGB::Black;
    // if (i < vReal[14])
    //   leds[LedsXFranjas * 14 + i] = newcolorFijo;
    // else
    //   leds[LedsXFranjas * 14 + i] = CRGB::Black;
    // if (i < vReal[15])
    //   leds[LedsXFranjas * 15 + i] = newcolor;
    // else
    //   leds[LedsXFranjas * 15 + i] = CRGB::Black;
  }

  FastLED.show();
 // delayMicroseconds(10);
  //FastLED.delay(15);

  //Serial.println("fin fun..");
}


void efectoAudio() {
  //Serial.println("efectoAudio: " + (String)efectoSonido);
  // if (efectoSonido == 2) {
  //   efectoAudio2();
  // } else if (efectoSonido == 3) {
  //   efectoAudio3();
  // } else if (efectoSonido == 4) {
  //   efectoAudio4();
  // } else {
  doubleRainbow();
  // }

  // efectoSonido++;
  //if (efectoSonido == 5) efectoSonido = 1;
}
