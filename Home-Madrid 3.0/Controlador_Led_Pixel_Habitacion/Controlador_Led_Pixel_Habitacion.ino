/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/


#include <Arduino.h>
#include "FastLED.h"

#define NUM_LEDS 650
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
int FRAMES_PER_SECOND = 120;
CRGB leds[NUM_LEDS];

int brillo = 150;
int r, g, b;
bool powerOn = false;
bool powerLed = false;

#define TOTAL_EFECTOS 11
uint8_t gCurrentPatternNumber = 0;

#define PIN_LED_DATA 2
#define PIN_INFRARROJO 4
#define PIN_BOTON 5

#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

const char *ssid = "ControlMaestro";
const char *password = "goodlife";

bool connected = false;

#include "efectos.hpp"

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

void setup() {

pinMode(PIN_BOTON, INPUT);

  r = 0;
  g = 0;
  b = 0;

  // Debug console
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    leds[whiteLed] = CRGB::Black;
  }

  FastLED.show();

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
  webSocket.loop();

  int botonStatus = digitalRead(PIN_BOTON);

  if (botonBeforeStatus == LOW && botonStatus == HIGH) {
    botonBeforeStatus = HIGH;
    Serial.println("boton precionado y soltado: ");
    nextPattern();
    onPower(true);

  } else if (botonBeforeStatus != botonStatus) {
    tiempo1 = millis();
    botonBeforeStatus = botonStatus;

  } else if (botonBeforeStatus == LOW && botonStatus == LOW) {
    tiempo2 = millis();

    if (tiempo2 > (tiempo1 + 3000) && powerOn == true) {
      botonBeforeStatus = HIGH;
      onPower(false);
      for (int i = 0; i <= NUM_LEDS; i++)
        leds[i] = CRGB::Black;
      FastLED.show();

      delay(3000);
      tiempo1 = millis();
      return;
    }
  }

  if (powerOn && powerLed && gCurrentPatternNumber >= 0) {
    selectEfecto();
  } else {
    for (int i = 0; i <= NUM_LEDS; i++)
      leds[i] = CRGB::Black;
    FastLED.show();
  }
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { sinEfecto, ColorPasifica, efectoAudio, pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, blancoGira };

void selectEfecto() {

  gPatterns[gCurrentPatternNumber]();
  if (gCurrentPatternNumber > 1) {

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

void onPower(boolean encender) {
  if (powerOn == true && encender == false) {
    powerOn = false;
    powerLed = false;
  } else {
    powerOn = true;
    powerLed = true;
  }
}

#define CMD_PowerAll "CMD_PowerAll"
#define CMD_powerFinChill "CMD_powerFinChill"
#define CMD_Abrir_Entrada "CMD_Abrir_Entrada"
#define CMD_powerLed "CMD_powerLed"
#define CMD_power_flor "CMD_power_flor"
#define CMD_efecto "CMD_efecto"
#define CMD_brillo "CMD_brillo"
#define CMD_Plasma "CMD_Plasma"
#define CMD_power_UV "CMD_power_UV"
#define CMD_audio "CMD_audio"
#define CMD_Color "CMD_Color"

int efectoSelect = 3;

void nextPattern() {
  onPower(true);

  efectoSelect++;
  if (efectoSelect == TOTAL_EFECTOS) efectoSelect = 3;

  gCurrentPatternNumber = efectoSelect;
}

void executeFunction(String CMD) {
  //Serial.print("executeFunction.. : ");

  if (CMD == CMD_efecto) {
    nextPattern();
    Serial.println(" Efecto: " + String(gCurrentPatternNumber));

  } else if (CMD == CMD_audio) {
    onPower(true);
    gCurrentPatternNumber = 2;
    Serial.print("& ");

  } else if (CMD == CMD_brillo) {
    onPower(true);
    brillo += 20;

    if (brillo > 200) {
      for (int i = 0; i <= NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      delay(500);
    }
    if (brillo > 230) brillo = 10;

    Serial.print(" FS_brillo: ");
    Serial.println(brillo);
    FastLED.setBrightness(brillo);
    FastLED.show();

  } else if (CMD == CMD_Color) {
    selectPasifica();
    onPower(true);
    gCurrentPatternNumber = 1;

  } else if (CMD == CMD_PowerAll) {
    onPower(false);

    Serial.println(" CMD_PowerAll ");

  } else if (CMD == CMD_powerFinChill) {


  } else if (CMD == CMD_powerLed) {
    powerLed = !powerLed;
    Serial.println(" powerLed ");

  } else if (CMD == CMD_Plasma) {


  } else if (CMD == CMD_power_UV) {
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

          // if (strPayload.indexOf('|') != -1) {
          //   strCommando = strPayload.substring(0, strPayload.indexOf('|'));  //guardo temporalete el comando audio
          //   strPayload = strPayload.substring(strPayload.indexOf('|') + 1);  //ahora deejo solo la info
          //   int i = 0;
          //   String aux = "";
          //   //Serial.print("Real. ");
          //   while (strPayload.indexOf('|') != -1 || i >= 15) {
          //     aux = strPayload.substring(0, strPayload.indexOf('|'));
          //     vReal[i] = aux.toInt();
          //     strPayload = strPayload.substring(strPayload.indexOf('|') + 1);
          //     //Serial.print("- " +String(i) +" -"+  vReal[i]);
          //     i++;
          //   }

          //   // Serial.println("---- " +strCommando);
          //   strCommando.trim();
          //   executeFunction(strCommando);
          // } else {
            String msj = "#MSJ_RECIBIDO| " + strPayload;

            Serial.println("---- " +msj);
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