/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/


#include <Arduino.h>
#include "FastLED.h"

#define NUM_LEDS1 93
#define NUM_LEDS2 93
//#define NUM_LEDS3 93

#define NUM_LEDS NUM_LEDS1 + NUM_LEDS2  // + NUM_LEDS3

#define LED_TYPE WS2812
#define COLOR_ORDER GRB
int FRAMES_PER_SECOND = 120;


#define PIN_LED_DATA1 0
#define PIN_LED_DATA2 2
//#define PIN_LED_DATA3 4

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
//CRGB leds3[NUM_LEDS3];

int brillo = 255;
int r, g, b;
bool powerOn = true;
String modeEfecto = "efecto";  //efecto/color

#define TOTAL_EFECTOS 11
uint8_t gCurrentPatternNumber = 9;


#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

const char *ssid = "ControlMaestro";
const char *password = "goodlife";

bool connected = false;

#define RAW_BUFFER_LENGTH 180
#define MARK_EXCESS_MICROS 20  // Adapt it to your IR receiver module. 20 is recommended for the cheap VS1838 modules.
#include <IRremote.hpp>

#include "efectos.hpp"
#include "codigosIR.hpp"

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

void setup() {
  IrReceiver.begin(4);

  // Debug console
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, PIN_LED_DATA1, COLOR_ORDER>(leds1, NUM_LEDS1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_LED_DATA2, COLOR_ORDER>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE, PIN_LED_DATA3, COLOR_ORDER>(leds3, NUM_LEDS3).setCorrection(TypicalLEDStrip);

  for (int i = 0; i < NUM_LEDS1; i++) leds1[i] = CRGB::Black;
  for (int i = 0; i < NUM_LEDS2; i++) leds2[i] = CRGB::Black;
  // for (int i = 0; i <= NUM_LEDS3; i++) leds3[i] = CRGB::Black;

  FastLED.setBrightness(brillo);
  FastLED.show();
CargarCodeIR() ;

  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  // server address, port and URL
  webSocket.begin("192.168.1.150", 81, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  tiempo1 = millis();
}

bool presshold = false;
bool botonBeforeStatus = HIGH;

void loop() {
 // webSocket.loop();
  checkIR();

  if (powerOn && gCurrentPatternNumber >= 0) {
    selectEfecto();
  } else {
    for (int i = 0; i < NUM_LEDS1; i++) leds1[i] = CRGB::Black;
    for (int i = 0; i < NUM_LEDS2; i++) leds2[i] = CRGB::Black;
    //for (int i = 0; i < NUM_LEDS3; i++) leds3[i] = CRGB::Black;
    FastLED.show();
  }
}

void checkIR() {
  if (IrReceiver.decode()) {  // Grab an IR code

    // Check if the buffer overflowed
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
      // Serial.println(F("Try to increase the \"RAW_BUFFER_LENGTH\" value of " STR(RAW_BUFFER_LENGTH) " in " __FILE__));
      // see also https://github.com/Arduino-IRremote/Arduino-IRremote#compile-options--macros-for-this-library
    } else {
      IrReceiver.stop();
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      } else {
        if (IrReceiver.decodedIRData.decodedRawData != 0) {
          Serial.print("value:");
          Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

          String Code = buscarCodigoEnLista(IrReceiver.decodedIRData.decodedRawData);
          if (Code != "") {
            executeFunction(Code);
          }
        }
      }
      IrReceiver.start();
    }
    IrReceiver.resume();  // Prepare for the next value
  }
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { sinEfecto, ColorPasifica, pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, blancoGira };

void selectEfecto() {

  gPatterns[gCurrentPatternNumber]();
  if (gCurrentPatternNumber >=0) {

    FastLED.show();
    FastLED.delay(1000 / FRAMES_PER_SECOND);

    EVERY_N_MILLISECONDS(20) {
      gHue++;  // slowly cycle the "base color" through the rainbow
    }
  }
}

void ColorPasifica() {
  EVERY_N_MILLISECONDS(20) {
    pacifica_loop();
    FastLED.show();
  }
}

void sinEfecto() {
  delay(100);
}


int efectoSelect = 2;
int ColorSelect = 1;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


void nextPattern(int efecto = -1) {

  if (efecto == -1) {
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
    if (gCurrentPatternNumber == 0) gCurrentPatternNumber = 2;
  } else {
    gCurrentPatternNumber = efecto;
  }

  efectoSelect = gCurrentPatternNumber;
}

void executeFunction(String CMD) {
  Serial.print("executeFunction.. : " + CMD);
  CMD.trim();

  String value = "";
  int index = CMD.indexOf(";");
  if (index != -1) {
    value = CMD.substring(index + 1).toInt();
  }
  Serial.println(" \t value.. : " + value);


  if (CMD.indexOf("CMD_PowerAll") != -1) {
    powerOn = !powerOn;

  } else if (CMD.indexOf("CMD_Mode") != -1) {
    if (modeEfecto == "efecto") {
      modeEfecto = "color";
      Serial.print("cambia a modo color " );

      executeFunction("CMD_efectoColor;" + String(ColorSelect));
      return;
    } else {
      modeEfecto = "efecto";
      Serial.print("cambia a modo efecto " );
       executeFunction("CMD_efectoColor;" + String(efectoSelect));
      return;

    }  //efecto/color

  } else if (CMD.indexOf("CMD_BrilloMenos") != -1) {
    brillo = brillo - 20;
    if (brillo < 0) brillo = 15;
    FastLED.setBrightness(brillo);
    FastLED.show();

  } else if (CMD.indexOf("CMD_BrilloMas") != -1) {
    brillo = brillo + 20;
    if (brillo > 255) brillo = 255;
    FastLED.setBrightness(brillo);
    FastLED.show();

  } else if (CMD.indexOf("CMD_Blanco") != -1) {
    nextPattern(0);
    for (int i = 0; i < NUM_LEDS1; i++) leds1[i] = CRGB::White;
    for (int i = 0; i < NUM_LEDS2; i++) leds2[i] = CRGB::White;
    //for (int i = 0; i < NUM_LEDS3; i++) leds3[i] = CRGB::Black;
    FastLED.show();

  } else if (CMD.indexOf("CMD_efectoColor") != -1) {


    if (value == "1" && modeEfecto == "efecto") {
      nextPattern(2);
      return;
    }
    if (value == "2" && modeEfecto == "efecto") {
      nextPattern(3);
      return;
    }
    if (value == "3" && modeEfecto == "efecto") {
      nextPattern(4);
      return;
    }
    if (value == "4" && modeEfecto == "efecto") {
      nextPattern(5);
      return;
    }
    if (value == "5" && modeEfecto == "efecto") {
      nextPattern(6);
      return;
    }
    if (value == "6" && modeEfecto == "efecto") {
      nextPattern(7);
      return;
    }
    if (value == "7" && modeEfecto == "efecto") {
      nextPattern(8);
      return;
    }

    if (value == "1" && modeEfecto == "color") {
      selectPasifica(2);
      nextPattern(1);
      ColorSelect = 1;
      return;
    }
    if (value == "2" && modeEfecto == "color") {
      selectPasifica(3);
      nextPattern(1);
      ColorSelect = 2;
      return;
    }
    if (value == "3" && modeEfecto == "color") {
      selectPasifica(4);
      nextPattern(1);
      ColorSelect = 3;
      return;
    }
    if (value == "4" && modeEfecto == "color") {
      selectPasifica(5);
      nextPattern(1);
      ColorSelect = 4;
      return;
    }
    if (value == "5" && modeEfecto == "color") {
      selectPasifica(6);
      nextPattern(1);
      ColorSelect = 5;
      return;
    }
    if (value == "6" && modeEfecto == "color") {
      selectPasifica(7);
      nextPattern(1);
      ColorSelect = 6;
      return;
    }
    if (value == "7" && modeEfecto == "color") {
      selectPasifica(8);
      nextPattern(1);
      ColorSelect = 7;
      return;
    }
    if (value == "8" && modeEfecto == "color") {
      selectPasifica(7);
      nextPattern(1);
      ColorSelect = 8;
      return;
    }
    if (value == "9" && modeEfecto == "color") {
      selectPasifica(8);
      nextPattern(1);
      ColorSelect = 9;
      return;
    }
  }
}

String strPayload;
String primerCaracter;
String strCommando;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WSc] Disconnected!");

      connected = false;
      break;
    case WStype_CONNECTED:
      {
        Serial.println("[WSc] Connected to");
        connected = true;

        // send message to server when Connected
        Serial.println("[WSc] SENT: Connected");
        webSocket.sendTXT("#ID_NAME|habitacion2");
      }
      break;
    case WStype_TEXT:
      strPayload = (char *)payload;
      strPayload.trim();

      if (strPayload.length() == 0) strPayload = " ";
      primerCaracter = strPayload.substring(0, 1);

      //Serial.println("texto: " + strPayload + " - 1er: " + primerCaracter);
      if (primerCaracter == "#") {

        strPayload = strPayload.substring(1);

        strCommando = strPayload.substring(0, strPayload.indexOf('|'));
        strPayload = strPayload.substring(strPayload.indexOf('|') + 1);

        // Serial.println("strCommando: " + strPayload);

        if (strCommando == "MSJ_RECIBIDO") {

          String msj = "#MSJ_RECIBIDO| " + strPayload;

          Serial.println("---- " + msj);
          webSocket.sendTXT(msj);
          executeFunction(strPayload);
          // }
          //#MSJ_RECIBIDO|CMD_audio|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|0.00|
        }

      } else {
        Serial.println("Simple mensaje recibido: " + strPayload);
      }
      break;
    case WStype_BIN:
      Serial.println("[WSc] get binary length: " + String(length));
      hexdump(payload, length);
      break;
    case WStype_PING:
      // pong will be send automatically
      Serial.println("[WSc] get ping");
      break;
    case WStype_PONG:
      // answer to a ping we send
      Serial.println("[WSc] get pong");
      break;
  }
}