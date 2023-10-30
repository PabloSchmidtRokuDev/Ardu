

#include <Arduino.h>
#include "FastLED.h"

#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;


#define DATA_PIN 12
#define DATA_PIN2 14
#define DATA_PIN3 27
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

#define tira1 650 + 17
#define tira2 42 + 65
#define NUM_LEDS tira1 + tira2
#define NUM_LEDS2 400
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];

#define FRAMES_PER_SECOND 120

bool powerOn = true;
bool powerLed = true;

int gCurrentPatternNumber = 1;
int TOTAL_EFECTOS = 3;
int BRIGHTNESS = 150;

#define f1_d 44
#define f1_h f1_d + 62

#define f2_d f1_h + 62
#define f2_h f2_d + 65

#define f3_d f2_h + 36
#define f3_h f3_d + 65

#define f4_d f3_h + 108 + 18
#define f4_h f4_d + 65

#define f5_d f4_h + 28
#define f5_h f5_d + 63

#define f6_d f5_h + 39
#define f6_h f6_d + 66

const char* ssid = "ControlMaestro";
const char* password = "goodlife";

unsigned long messageInterval = 5000;
bool connected = false;

#define pin_Led1 26
#define pin_Led2 25
#define pin_Led3 33
#define pin_Led4 32
#define pin_EncenderProy 15


#include "efectos.hpp"

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

unsigned long tiempo3 = 0;
unsigned long tiempo4 = 0;


TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {

  // Debug console
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(leds, tira2).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, tira2, tira1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN3, COLOR_ORDER>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(pin_Led1, OUTPUT);
  pinMode(pin_Led2, OUTPUT);
  pinMode(pin_Led3, OUTPUT);
  pinMode(pin_Led4, OUTPUT);
  pinMode(pin_EncenderProy, OUTPUT);

  digitalWrite(pin_Led1, 0);
  digitalWrite(pin_Led2, 0);
  digitalWrite(pin_Led3, 0);
  digitalWrite(pin_Led4, 0);
  digitalWrite(pin_EncenderProy, LOW);


  xTaskCreatePinnedToCore(
    Task1code,
    "Task1",
    10000,
    NULL,
    1,
    &Task1,
    0);
  // delay(500);

  // xTaskCreatePinnedToCore(
  //   Task2code,
  //   "Task2",
  //   10000,
  //   NULL,
  //   1,
  //   &Task2,
  //   1);
}

bool presshold = false;
bool botonBeforeStatus = HIGH;

int colorLed = 0;
int valorLed1 = 0;
int valorLed2 = 0;
int valorLed3 = 0;
int valorLed4 = 0;


void loop() {
}


typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { sinEfecto, Clasico, nebulosa };

void selectEfecto() {

  gPatterns[gCurrentPatternNumber]();
  if (gCurrentPatternNumber >= 1) {

    FastLED.show();
    FastLED.delay(1000 / FRAMES_PER_SECOND);

    EVERY_N_MILLISECONDS(20) {
      gHue++;  // slowly cycle the "base color" through the rainbow
       gHue2++; 
        gHue3++; 
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

void nebulosa() {

  digitalWrite(pin_EncenderProy, HIGH);

  tiempo2 = millis();

  if (tiempo2 > (tiempo1 + 10000)) {
    colorLed++;
    if (colorLed == 4) colorLed = 0;
    tiempo1 = millis();
  }

  tiempo4 = millis();
  if (tiempo4 > (tiempo3 + 10)) {

    if (colorLed == 0) {
      if (valorLed4 > 0) valorLed4--;
      if (valorLed1 < 255) valorLed1 = valorLed1 + 2;

    } else if (colorLed == 1) {
      if (valorLed1 > 0) valorLed1--;
      if (valorLed2 < 255) valorLed2 = valorLed2 + 2;
    }
    if (colorLed == 2) {
      if (valorLed2 > 0) valorLed2--;
      if (valorLed3 < 255) valorLed3 = valorLed3 + 2;
    }
    if (colorLed == 3) {
      if (valorLed3 > 0) valorLed3--;
      if (valorLed4 < 255) valorLed4 = valorLed4 + 2;
    }
    tiempo3 = millis();
  }

  analogWrite(pin_Led1, valorLed1);
  analogWrite(pin_Led2, valorLed2);
  analogWrite(pin_Led3, valorLed3);
  analogWrite(pin_Led4, valorLed4);
}

void onPower(boolean encender) {
  if (powerOn == true && encender == false) {
    powerOn = false;
    powerLed = false;
    digitalWrite(pin_EncenderProy, LOW);

   valorLed1 = 0; 
   valorLed2 = 0; 
   valorLed3 = 0; 
   valorLed4 = 0;

    analogWrite(pin_Led1, 0);
    analogWrite(pin_Led2, 0);
    analogWrite(pin_Led3, 0);
    analogWrite(pin_Led4, 0);
  } else {
    powerOn = true;
    powerLed = true;
    digitalWrite(pin_EncenderProy, HIGH);
  }
}

void Task1code(void* pvParameters) {
  Serial.println("run task...");


  for (;;) {

    if (powerOn && powerLed && gCurrentPatternNumber >= 0) {
      selectEfecto();
    } else {
      for (int i = 0; i <= NUM_LEDS; i++)
        leds[i] = CRGB::Black;
      FastLED.show();
    }
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

#define FLOR_power_flor "FLOR_power_flor"
#define FLOR_efecto "FLOR_efecto"
#define FLOR_brillo "FLOR_brillo"

int efectoSelect = 3;

void nextPattern() {
  onPower(true);

  efectoSelect++;
  if (efectoSelect >= TOTAL_EFECTOS) efectoSelect = 0;

  for (int i = 0; i <= NUM_LEDS; i++)
    leds[i] = CRGB::Black;
  FastLED.show();

    digitalWrite(pin_EncenderProy, LOW);

   valorLed1 = 0; 
   valorLed2 = 0; 
   valorLed3 = 0; 
   valorLed4 = 0;

    analogWrite(pin_Led1, 0);
    analogWrite(pin_Led2, 0);
    analogWrite(pin_Led3, 0);
    analogWrite(pin_Led4, 0);

  tiempo1 = millis();
  tiempo3 = millis();

  gCurrentPatternNumber = efectoSelect;
}

void executeFunction(String CMD) {
  //Serial.print("executeFunction.. : ");

  if (CMD == CMD_efecto || CMD == FLOR_efecto) {
    nextPattern();
    Serial.println(" Efecto: " + String(gCurrentPatternNumber));

  } else if (CMD == CMD_audio) {
    // onPower(true);
    // gCurrentPatternNumber = 2;
    // Serial.print("& ");

  } else if (CMD == CMD_brillo || CMD == FLOR_brillo) {
    onPower(true);
    BRIGHTNESS += 20;

    if (BRIGHTNESS > 200) {
      for (int i = 0; i <= NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      delay(500);
    }
    if (BRIGHTNESS > 230) BRIGHTNESS = 10;

    Serial.print(" FS_brillo: ");
    Serial.println(BRIGHTNESS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();

  } else if (CMD == CMD_PowerAll) {
    onPower(false);

    Serial.println(" CMD_PowerAll ");


  } else if (CMD == FLOR_power_flor || CMD == CMD_powerLed) {
    powerLed = !powerLed;
    Serial.println(" powerLed ");

  } else if (CMD == CMD_Plasma) {


  } else if (CMD == CMD_power_UV) {
  }
}


void hexdump(const void* mem, uint32_t len, uint8_t cols = 16) {
  const uint8_t* src = (const uint8_t*)mem;
  Serial.println("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)");  //, (ptrdiff_t)src, len, len);
  for (uint32_t i = 0; i < len; i++) {
    if (i % cols == 0) {
      Serial.println("\n[0x%08X] 0x%08X: ");  //, (ptrdiff_t)src, i);
    }
    src++;
  }
}

String strPayload;
String primerCaracter;
String strCommando;

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
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
        webSocket.sendTXT("#ID_NAME|Flor");
      }
      break;
    case WStype_TEXT:
      strPayload = (char*)payload;
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


void Task2code(void* pvParameters) {


  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.println("[SETUP] BOOT WAIT %d...\n" + String(t));
    delay(500);
  }

  Serial.println("Conectando...");
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
  for (;;) {
    webSocket.loop();
  }
}