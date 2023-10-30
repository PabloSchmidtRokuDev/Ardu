/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/


#include <Arduino.h>
#include "FastLED.h"

#define NUM_LEDS1 65*6 + 10
#define NUM_LEDS2 160
#define NUM_LEDS3 160

#define NUM_LEDS NUM_LEDS1 + NUM_LEDS2 + NUM_LEDS3

#define LED_TYPE WS2812
#define COLOR_ORDER GRB
int FRAMES_PER_SECOND = 120;


#define PIN_LED_DATA1 12
#define PIN_LED_DATA2 14
#define PIN_LED_DATA3 16

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

int brillo = 255;
int r, g, b;
bool powerOn = true;
bool powerLed = true;

bool powerUV = true;
bool powerBlanco = true;
bool powerAzul = true;

#define TOTAL_EFECTOS 11
uint8_t gCurrentPatternNumber = 4;

#define PIN_LED1 2
#define MaxLed1 70 //no conectado

#define PIN_LED2 0
#define MaxLed2 100 //uv

#define PIN_LED3 4
#define MaxLed3 150 // azul

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

  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);

  analogWrite(PIN_LED1, MaxLed1);
  analogWrite(PIN_LED2, MaxLed2);
  analogWrite(PIN_LED3, MaxLed3);
 powerUV = true;
 powerBlanco = true;
 powerAzul = true;

  r = 0;
  g = 0;
  b = 0;

  // Debug console
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, PIN_LED_DATA1, COLOR_ORDER>(leds1, NUM_LEDS1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_LED_DATA2, COLOR_ORDER>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_LED_DATA3, COLOR_ORDER>(leds3, NUM_LEDS3).setCorrection(TypicalLEDStrip);

  // for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
  //   leds[whiteLed] = CRGB::Black;
  // }

  // FastLED.show();

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

  // int botonStatus = digitalRead(PIN_BOTON);

  // if (botonBeforeStatus == LOW && botonStatus == HIGH) {
  //   botonBeforeStatus = HIGH;
  //   Serial.println("boton precionado y soltado: ");
  //   nextPattern();
  //   onPower(true);

  // } else if (botonBeforeStatus != botonStatus) {
  //   tiempo1 = millis();
  //   botonBeforeStatus = botonStatus;

  // } else if (botonBeforeStatus == LOW && botonStatus == LOW) {
  //   tiempo2 = millis();

  //   if (tiempo2 > (tiempo1 + 3000) && powerOn == true) {
  //     botonBeforeStatus = HIGH;
  //     onPower(false);
  //     for (int i = 0; i <= NUM_LEDS; i++)
  //       leds[i] = CRGB::Black;
  //     FastLED.show();

  //     delay(3000);
  //     tiempo1 = millis();
  //     return;
  //   }
// }

if (powerOn && powerLed && gCurrentPatternNumber >= 0) {
  selectEfecto();
} else {
  for (int i = 0; i < NUM_LEDS1; i++) leds1[i] = CRGB::Black;
  for (int i = 0; i < NUM_LEDS2; i++) leds2[i] = CRGB::Black;
  for (int i = 0; i < NUM_LEDS3; i++) leds3[i] = CRGB::Black;
  FastLED.show();
}
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { sinEfecto, ColorPasifica, pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, blancoGira };

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


int efectoSelect = 3;

void nextPattern() {
  onPower(true);

  efectoSelect++;
  if (efectoSelect == TOTAL_EFECTOS) efectoSelect = 3;

  gCurrentPatternNumber = efectoSelect;
}

#define CMD_PowerAll "CMD_PowerAll"
#define CMD_PowerUV "CMD_PowerUV"
#define CMD_power_Azul "CMD_power_Azul"
#define CMD_power_Blanco "CMD_power_Blanco"


void executeFunction(String CMD) {
  Serial.print("executeFunction.. : " + CMD);
  CMD.trim();

  String value = "";
  int index = CMD.indexOf(";");
  if (index != -1) {
    value = CMD.substring(index + 1).toInt();
  }
Serial.println(" \t value.. : " + value);


  if (CMD.indexOf(CMD_PowerAll) != -1) {
      analogWrite(PIN_LED1, 0);
  analogWrite(PIN_LED2, 0);
  analogWrite(PIN_LED3, 0);

    Serial.println(" CMD_PowerAll ");

  }else if (CMD.indexOf(CMD_PowerUV) != -1) {

    if (value != "") {
      if (value == "1") powerUV = true;
      else powerUV = false;
    } else {
      powerUV = !powerUV;
    }

    if (powerUV) {
        
  analogWrite(PIN_LED2, MaxLed2);
  
      Serial.println(" CMD_PowerUV ON");
    } else {
      analogWrite(PIN_LED2, 0);
      Serial.println(" CMD_PowerUV OFF");
    }

  }else if (CMD.indexOf(CMD_power_Blanco) != -1) {

    if (value != "") {
      if (value == "1") powerBlanco = true;
      else powerBlanco = false;
    } else {
      powerBlanco = !powerBlanco;
    }

    if (powerBlanco) {
      analogWrite(PIN_LED1, MaxLed1);
      Serial.println(" CMD_power_Blanco ON");
    } else {
      analogWrite(PIN_LED1, MaxLed1);
      Serial.println(" CMD_power_Blanco OFF");
    }

  }else if (CMD.indexOf(CMD_power_Azul) != -1) {

    if (value != "") {
      if (value == "1") powerUV = true;
      else powerAzul = false;
    } else {
      powerAzul = !powerAzul;
    }

    if (powerAzul) {
      analogWrite(PIN_LED3, MaxLed3);
      Serial.println(" CMD_power_Azul ON");
    } else {
      analogWrite(PIN_LED3, 0);
      Serial.println(" CMD_power_Azul OFF");
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