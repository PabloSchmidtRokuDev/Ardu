#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    50
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

WebSocketsClient webSocket;

const char *ssid = "ControlRemotoJoystick";
const char *password = "goodlife";

unsigned long messageInterval = 5000;
bool connected = false;

#define DEBUG_SERIAL Serial

#define SERVO_PIN1 2
#define SERVO_PIN2 0
#define PHOTO_PIN 4

#define SERVO_PIN1_DEF_POS 90
#define SERVO_PIN2_DEF_POS 90

int velocidadServo = 10;
int distance1 = 0;
int servo1PosActual = SERVO_PIN1_DEF_POS;
Servo servo1;

int distance2 = 0;
int servo2PosActual = SERVO_PIN1_DEF_POS;
Servo servo2;

int incrementos = 2;
int luzEstado = 0;

void setup() {
  Serial.begin(115200);

  pinMode(PHOTO_PIN, OUTPUT);

  servo1.attach(SERVO_PIN1);
  servo1.write(SERVO_PIN1_DEF_POS);

  servo2.attach(SERVO_PIN2);
  servo2.write(SERVO_PIN2_DEF_POS);

  DEBUG_SERIAL.begin(115200);

  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    DEBUG_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    DEBUG_SERIAL.flush();
    delay(500);
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  DEBUG_SERIAL.print("Local IP: ");
  DEBUG_SERIAL.println(WiFi.localIP());
  // server address, port and URL
  webSocket.begin("192.168.1.200", 81, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
}

String dataAvailable = "";

void loop() {
  setServoPosition();
  webSocket.loop();
}

void checkData(String dataAvailable) {
  if (dataAvailable != "" && dataAvailable.indexOf("|") != -1) {
    String cm = dataAvailable.substring(0, dataAvailable.indexOf("|"));
    //Serial.print("1 -cm: " + cm);

    dataAvailable = dataAvailable.substring(dataAvailable.indexOf("|") + 1);
    //Serial.print(" \t 2- new  dataAvailable: " + dataAvailable);

    String obj = cm.substring(0, cm.indexOf(":"));
    //Serial.print("\t obj: " + obj);
    cm = cm.substring(cm.indexOf(":") + 1);
    //Serial.println(" \t 3- cm final: " + cm);

    executeInstruccion(obj, cm);

    if (dataAvailable != "" && dataAvailable.indexOf("|") != -1) {
      checkData(dataAvailable);
    }
  }
}

bool btn1Pressed = false;

void executeInstruccion(String obj, String valor) {

  if (obj == "X" || obj == "Y") {

    //Serial.print("valor " + valor);

    String incStr = valor.substring(1);
    //Serial.print(" \t incStr " + incStr);
    incrementos = incStr.toInt();

    velocidadServo = 200;
    if (incrementos == 2) velocidadServo = 100;
    if (incrementos == 3) velocidadServo = 50;
    if (incrementos == 4) velocidadServo = 10;

    if (incrementos > 2) incrementos = 1;
    valor = valor.substring(0, 1);
    //Serial.println(" \t new valor " + valor);

    if (obj == "X" && valor == "R") {
      distance1 = incrementos;
    }

    if (obj == "X" && valor == "L") {
      distance1 = -incrementos;
    }

    if (obj == "Y" && valor == "U") {
      distance2 = incrementos;
    }

    if (obj == "Y" && valor == "D") {
      distance2 = -incrementos;
    }

    setServoPosition();

  } else if (obj == "BJ") {
    if (valor == "0" && btn1Pressed == false) btn1Pressed = true;
    if (valor == "1" && btn1Pressed == true) {
      btn1Pressed = false;
      encenderLuz();
    }
  }
}

void encenderLuz() {

  if (luzEstado == 0) {
    Serial.println("Encender Luz ");
    digitalWrite(PHOTO_PIN, HIGH);
    luzEstado = 1;
  } else {
    digitalWrite(PHOTO_PIN, LOW);
    Serial.println("Apagar Luz ");
    luzEstado = 0;
  }
}

void setServoPosition() {

  if (distance1 != 0 || distance2 != 0) {

    if (distance1 != servo1PosActual && distance1 > servo1PosActual) {
      servo1PosActual += distance1;
    }

    if (distance1 != servo1PosActual && distance1 < servo1PosActual) {
      servo1PosActual -= distance1;
    }

    if (distance2 != servo2PosActual && distance2 > servo2PosActual) {
      servo2PosActual += distance2;
    }

    if (distance2 != servo2PosActual && distance2 < servo2PosActual) {
      servo2PosActual -= distance2;
    }

    if (servo1PosActual >= 180) {
      distance1 = 0;
      servo1PosActual = 180;
    }
    if (servo1PosActual <= 0) {
      distance1 = 0;
      servo1PosActual = 0;
    }

    if (servo2PosActual >= 180) {
      distance2 = 0;
      servo2PosActual = 180;
    }

    if (servo2PosActual <= 0) {
      distance2 = 0;
      servo2PosActual = 0;
    }

    servo1.write(servo1PosActual);
    servo2.write(servo2PosActual);
    delay(velocidadServo);
  }
}

String strPayload;
String primerCaracter;
String strCommando;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
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
        webSocket.sendTXT("#ID_NAME|LUZ_01");
      }
      break;
    case WStype_TEXT:
      strPayload = (char *)payload;
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