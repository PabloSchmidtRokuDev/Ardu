
#include <Arduino.h>
#include "FastLED.h"

#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

#define NUM_LEDS 576
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
int FRAMES_PER_SECOND = 120;
CRGB leds[NUM_LEDS];

int brillo = 150;
int r, g, b;
bool powerOn = true;
bool powerLed = true;
bool bolaPowerOn = false;
bool rele2Power = false;
bool powerUV = false;
bool audioActivado = false;
int efectoSonido = 1;

#define TOTAL_EFECTOS 11
uint8_t gCurrentPatternNumber = 0;

#define PIN_LED_DATA 27
#define PIN_RELE_LUZ_BLANCA 26
#define PIN_BOLA_RAYOS 25

#define PIN_BARRA_POWER 21
#define PIN_BARRA_SENTIDO 19

#define PIN_ULTRA_VIOLETA 33

// setting PWM properties
const int freq = 2000;
const int ledChannel = 0;
const int resolution = 8;

const char* ssid = "ControlMaestro";
const char* password = "goodlife";

unsigned long messageInterval = 5000;
bool connected = false;

TaskHandle_t Task1;
TaskHandle_t Task2;

#include "efectos.hpp"

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("iniciando....");
  r = 10;
  g = 10;
  b = 10;

  pinMode(PIN_BOLA_RAYOS, OUTPUT);
  digitalWrite(PIN_BOLA_RAYOS, LOW);

  pinMode(PIN_ULTRA_VIOLETA, OUTPUT);
  analogWrite(PIN_ULTRA_VIOLETA, 0);

  pinMode(PIN_RELE_LUZ_BLANCA, OUTPUT);
  digitalWrite(PIN_RELE_LUZ_BLANCA, LOW);

  pinMode(PIN_BARRA_POWER, OUTPUT);
  digitalWrite(PIN_BARRA_POWER, LOW);

  pinMode(PIN_BARRA_SENTIDO, OUTPUT);
  digitalWrite(PIN_BARRA_SENTIDO, LOW);

  // Serial.println("PIN_BARRA_POWER y PIN_BARRA_SENTIDO  LOW ....");

  // delay(5000);
  // Serial.println("PIN_BARRA_POWER HIGH ....");
  // digitalWrite(PIN_BARRA_POWER, HIGH);
  // delay(15000);
  // Serial.println("PIN_BARRA_SENTIDO HIGH ....");
  //  digitalWrite(PIN_BARRA_SENTIDO, HIGH);
  //  delay(15000);


  //  Serial.println("PIN_BARRA_POWER y PIN_BARRA_SENTIDO  LOW ....");

  //  digitalWrite(PIN_BARRA_POWER, LOW);
  //  digitalWrite(PIN_BARRA_SENTIDO, LOW);

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

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { sinEfecto, ColorPasifica, efectoAudio, pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, blancoGira };

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

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
    bolaPowerOn = false;
    digitalWrite(PIN_BOLA_RAYOS, LOW);
    analogWrite(PIN_ULTRA_VIOLETA, 0);
    digitalWrite(PIN_RELE_LUZ_BLANCA, LOW);

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
#define CMD_Plasma "CMD_Plasma"
#define CMD_power_UV "CMD_power_UV"
#define CMD_audio "CMD_audio"
#define CMD_Color "CMD_Color"
#define CMD_Barra "CMD_Barra"

int barraEstado = 0;
void cambiarEstadoBarra() {
  if (barraEstado == 0) {
    digitalWrite(PIN_BARRA_POWER, HIGH);
    barraEstado = 1;
  } else if (barraEstado == 1) {
    digitalWrite(PIN_BARRA_SENTIDO, HIGH);
    barraEstado = 2;
  } else if (barraEstado == 2) {
    digitalWrite(PIN_BARRA_POWER, LOW);
    digitalWrite(PIN_BARRA_SENTIDO, LOW);
    barraEstado = 0;
  }
}

int efectoSelect = 3;
void executeFunction(String CMD) {
  //Serial.print("executeFunction.. : ");

  if (CMD == CMD_efecto) {
    onPower(true);
    gCurrentPatternNumber = efectoSelect;

    efectoSelect++;
    if (efectoSelect >= TOTAL_EFECTOS) efectoSelect = 3;

    Serial.println(" Efecto: " + String(gCurrentPatternNumber));

  } else if (CMD == CMD_audio) {
    onPower(true);
    gCurrentPatternNumber = 2;
    Serial.print("_ ");

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
    onPower(true);

    Serial.println(" CMD_PowerAll ");

  } else if (CMD == CMD_powerFinChill) {
    rele2Power = !rele2Power;

    if (rele2Power == true) {
      digitalWrite(PIN_RELE_LUZ_BLANCA, HIGH);
    } else {
      digitalWrite(PIN_RELE_LUZ_BLANCA, LOW);
    }

    Serial.println(" CMD_powerFinChill ");

  } else if (CMD == CMD_powerLed) {
    powerLed = !powerLed;
    Serial.println(" powerLed ");

  } else if (CMD == CMD_Plasma) {
    bolaPowerOn = !bolaPowerOn;
    if (bolaPowerOn == true) {
      digitalWrite(PIN_BOLA_RAYOS, HIGH);
    } else {
      digitalWrite(PIN_BOLA_RAYOS, LOW);
    }
    Serial.println(" CMD_Plasma ");

  } else if (CMD == CMD_power_UV) {

    powerUV = !powerUV;
    if (powerUV == true) {
      analogWrite(PIN_ULTRA_VIOLETA, 150);
    } else {
      analogWrite(PIN_ULTRA_VIOLETA, 0);
    }

    Serial.println(" CMD_power_UV ");

  } else if (CMD == CMD_Barra) {
    cambiarEstadoBarra();
    Serial.println(" CMD_Barra ");
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
      Serial.println("[WSc] Disconnected!\n");

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

          if (strPayload.indexOf('|') != -1) {
            strCommando = strPayload.substring(0, strPayload.indexOf('|'));  //guardo temporalete el comando audio
            strPayload = strPayload.substring(strPayload.indexOf('|') + 1);  //ahora deejo solo la info
            int i = 0;
            String aux = "";
            //Serial.print("Real. ");
            while (strPayload.indexOf('|') != -1 || i > 15) {
              aux = strPayload.substring(0, strPayload.indexOf('|'));
              vReal[i] = aux.toInt();
              strPayload = strPayload.substring(strPayload.indexOf('|') + 1);
              //Serial.print("- " +String(i) +" -"+  vReal[i]);
              i++;
            }

            // Serial.println("---- " +strCommando);
            strCommando.trim();
            executeFunction(strCommando);
          } else {
            String msj = "#MSJ_RECIBIDO| " + strPayload;
            webSocket.sendTXT(msj);
            executeFunction(strPayload);
          }
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

  cont = 0;
  bool flag = true;
  int pos = 0;
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
    if (pos >= NUM_LEDS) pos = 0;

    FastLED.show();

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