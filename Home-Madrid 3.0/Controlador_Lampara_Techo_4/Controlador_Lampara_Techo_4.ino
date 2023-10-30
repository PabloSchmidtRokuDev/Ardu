#include "arduinoFFT.h"

#include <Arduino.h>
#include "FastLED.h"

#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

#define NUM_LEDS1 65 * 6 + 15
#define NUM_LEDS2 336
#define NUM_LEDS3 570

#define NUM_LEDS NUM_LEDS1 + NUM_LEDS2 + NUM_LEDS3

#define LED_TYPE WS2812
#define COLOR_ORDER GRB
int FRAMES_PER_SECOND = 120;
CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

int brillo = 255;
bool powerOn = true;
bool powerLed = true;
bool bolaPowerOn = true;
bool rele2Power = false;
bool powerUV = true;


#define TOTAL_EFECTOS 11
uint8_t gCurrentPatternNumber = 0;

const char* ssid = "ControlMaestro";
const char* password = "goodlife";

unsigned long messageInterval = 5000;
bool connected = false;

TaskHandle_t Task1;
TaskHandle_t Task2;

#define PIN_LED_DATA 14
#define PIN_LED_DATA2 12
#define PIN_LED_DATA3 13


#define PIN_UV 19
#define MaxUV 150

int LED_Pins[4] = { 2, 4, 15, 5 };
int LED_Brillo_value[4] = { 0, 0, 0, 0 };
int LED_Current_value[4] = { 0, 0, 0, 0 };
int LED_Pulse_value[4] = { 0, 0, 0, 0 };

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

unsigned long tiempoMotor1 = 0;
unsigned long tiempoMotor2 = 0;

unsigned long tiempoLed1 = 0;
unsigned long tiempoLed2 = 0;

#include "efectos.hpp"
#include "fsnVarios.hpp"

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("iniciando....");

  for (int i = 0; i < 4; i++) { pinMode(LED_Pins[i], OUTPUT); }

  pinMode(PIN_UV, OUTPUT);
  analogWrite(PIN_UV, MaxUV);

  Serial.println("cargando leds");

  FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds1, NUM_LEDS1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_LED_DATA2, COLOR_ORDER>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_LED_DATA3, COLOR_ORDER>(leds3, NUM_LEDS3).setCorrection(TypicalLEDStrip);


  // for (int i = 0; i <= NUM_LEDS1; i++) leds1[i] = CRGB::Black;
  // for (int i = 0; i <= NUM_LEDS2; i++) leds2[i] = CRGB::Black;
  // for (int i = 0; i <= NUM_LEDS3; i++) leds3[i] = CRGB::Black;

  FastLED.setBrightness(brillo);
  FastLED.show();

  ColorFijo();
  selectPasifica(6);
  gCurrentPatternNumber = 5;

  tiempo1 = millis();
  tiempoLed1 = millis();
  tiempoMotor1 = millis();

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

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { sinEfecto, sinEfecto, ColorPasifica, pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, blancoGira };

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void selectEfecto() {

  //Serial.println("selectEfecto " + String(gCurrentPatternNumber) );
  gPatterns[gCurrentPatternNumber]();
  if (gCurrentPatternNumber >= 1) {

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
    //  FastLED.show();
  }
}

void sinEfecto() {
  delay(1);
}

void onPower(boolean encender) {

  if (powerOn == true && encender == false) {
    powerOn = false;
    bolaPowerOn = false;
    // digitalWrite(PIN_BOLA_RAYOS, LOW);
    analogWrite(PIN_UV, 0);
    // digitalWrite(PIN_RELE_LUZ_BLANCA, LOW);

  } else {
    powerOn = true;
    //bolaPowerOn = false;
    powerLed = true;
    //rele2Power = false;
  }
}

#define CMD_PowerAll "CMD_PowerAll"
#define CMD_powerFinChill "CMD_powerFinChill"
#define CMD_Abrir_Entrada "CMD_Abrir_Entrada"
#define CMD_powerLed "CMD_powerLed"
#define CMD_power_flor "CMD_power_flor"
#define CMD_efecto "CMD_efecto"
#define CMD_brillo "CMD_brillo"
#define ARA_ServoPosition "ARA_ServoPosition"
#define CMD_power_UV "CMD_power_UV"
#define ARA_audio "ARA_audio"
#define CMD_Color "CMD_Color"
#define CMD_Barra "CMD_Barra"

void executeFunction(String CMD) {
  Serial.print("executeFunction.. : " + String(CMD));

  if (CMD == CMD_efecto) {
    onPower(true);

    nextPattern();

    if (gCurrentPatternNumber == 0) gCurrentPatternNumber = 3;

    Serial.println(" Efecto: " + String(gCurrentPatternNumber));

  } else if (CMD == CMD_brillo) {
    onPower(true);
    brillo += 50;

    if (brillo > 251) brillo = 10;

    Serial.print(" FS_brillo: ");
    Serial.println(brillo);
    FastLED.setBrightness(brillo);
    FastLED.show();

  } else if (CMD == CMD_Color) {
    //selectPasifica();
    ColorFijo();
    setLedColor();
    onPower(true);

    gCurrentPatternNumber = 1;

  } else if (CMD == CMD_PowerAll) {
    onPower(true);

    Serial.println(" CMD_PowerAll ");

  } else if (CMD == CMD_powerFinChill) {
    rele2Power = !rele2Power;

    onPower(true);

    gCurrentPatternNumber = 0;

    //FastLED.setBrightness(100);
    //endChill();

    Serial.println(" CMD_powerFinChill ");

  } else if (CMD == CMD_powerLed) {
    powerLed = !powerLed;
    Serial.println(" powerLed ");

  } else if (CMD == ARA_ServoPosition) {
    bolaPowerOn = !bolaPowerOn;
    //setServosPos();

    // colorX ccl = colorXList[random(0, 52)];
    // LED_Brillo_value[0] = ccl.r;
    // LED_Brillo_value[1] = ccl.g;
    // LED_Brillo_value[2] = ccl.b;
    // LED_Brillo_value[3] = random(10, 50);

    // tiempoLed1 = millis();
    // if (bolaPowerOn == true) {
    //   digitalWrite(PIN_BOLA_RAYOS, HIGH);
    // } else {
    //   digitalWrite(PIN_BOLA_RAYOS, LOW);
    // }
    Serial.println(" ARA_ServoPosition ");

  } else if (CMD == CMD_power_UV) {

    powerUV = !powerUV;
    if (powerUV == true) {
      analogWrite(PIN_UV, MaxUV);
      Serial.println(" CMD_power_UV ON");
    } else {
      analogWrite(PIN_UV, 0);
      Serial.println(" CMD_power_UV OFF");
    }

  } else if (CMD == CMD_Barra) {
    Serial.println(" CMD_Barra ");
  }
}

void Task1code(void* pvParameters) {
  Serial.println("run task...");

  for (;;) {
    selectEfecto();
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
int cont = 0;

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      //Serial.println("[WSc] Disconnected!\n");

      connected = false;
      break;
    case WStype_CONNECTED:
      {
        Serial.println("[WSc] Connected to url:");
        connected = true;

        webSocket.sendTXT("#ID_NAME|living");
      }
      break;
    case WStype_TEXT:
      strPayload = (char*)payload;
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
          //   while (strPayload.indexOf('|') != -1 || i > 15) {
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

      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      Serial.println("[WSc] get binary length: %u\n");  //, length);
      hexdump(payload, length);
      break;
    case WStype_PING:
      // pong will be send automatically
      Serial.println("[WSc] get ping\n");
      break;
    case WStype_PONG:
      // answer to a ping we send
      Serial.println("[WSc] get pong\n");
      break;
  }
}

bool ShowSerialData() {
  int bufferCount;
  if (Serial.available()) {

    String serialHeader = "";

    delay(50);
    bufferCount = 0;
    while (Serial.available()) {  // reading data into char array
      char c = Serial.read();     // writing data into array
      //Serial.print(c);
      if (c == '\n') break;

      if (bufferCount == 100) break;
      bufferCount++;

      serialHeader += c;
    }

    Serial.println("llegsa: " + serialHeader);
    serialHeader.trim();

    if (serialHeader.length() >= 3) {

      executeFunction(serialHeader);

      return true;
    }
  }

  return false;
}

void Task2code(void* pvParameters) {

  Serial.println();

  Serial.println("Conectando...");
  WiFi.begin(ssid, password);

  cont = 0;
  bool flag = true;
  int pos = 0;
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

  tiempoLed1 = millis();
  for (;;) {

    webSocket.loop();
    ShowSerialData();
  }
}