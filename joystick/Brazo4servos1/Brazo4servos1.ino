#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <RGBWConverter.h>

#include <FastLED.h>

#define LED_PIN 13
#define LED_FOCO 14

#define NUM_LEDS 31
#define BRIGHTNESS 255
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

WebSocketsClient webSocket;

const char *ssid = "ControlRemotoJoystick";
const char *password = "goodlife";

RGBWConverter converter(240, 215, 200, true);

#define DEVICE_ID_NAME "BRAZO_01"
//#define DEVICE_ID_NAME "BRAZO_02"

unsigned long messageInterval = 5000;
bool connected = false;

#define DEBUG_SERIAL Serial

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;

#define SERVO_PIN1 0
#define SERVO_PIN2 2
#define SERVO_PIN3 5
#define SERVO_PIN4 4

int estado_servo_1 = 0;
int estado_servo_2 = 0;
int estado_servo_3 = 0;
int estado_servo_4 = 0;

int goto_servo_1 = 0;
int goto_servo_2 = 0;
int goto_servo_3 = 0;
int goto_servo_4 = 0;

// #define LED_R_PIN 12
// #define LED_G_PIN 13
// #define LED_B_PIN 14
// #define LED_W_PIN 15
// #define LED_Y_PIN 16

// int LED_R_current_value = 0;
// int LED_G_current_value = 0;
// int LED_B_current_value = 0;
// int LED_W_current_value = 0;
// int LED_Y_current_value = 0;

int LED_R_value = 0;
int LED_G_value = 0;
int LED_B_value = 0;
String LED_Nombre_value = "Black";
// int LED_W_value = 0;
// int LED_Y_value = 0;

int velocidadServo = 10;
int luzEstado = 0;

void setup() {
  Serial.begin(115200);

  pinMode(LED_FOCO, OUTPUT);
    digitalWrite(LED_FOCO, HIGH);
    delay(500);
    digitalWrite(LED_FOCO, LOW);
    
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  setLedColor();
  // pinMode(LED_R_PIN, OUTPUT);
  // pinMode(LED_G_PIN, OUTPUT);
  // pinMode(LED_B_PIN, OUTPUT);
  // pinMode(LED_W_PIN, OUTPUT);
  // pinMode(LED_Y_PIN, OUTPUT);

  servo_1.attach(SERVO_PIN1, 500, 2500);
  servo_2.attach(SERVO_PIN2, 500, 2500);
  servo_3.attach(SERVO_PIN3, 500, 2500);
  servo_4.attach(SERVO_PIN4, 500, 2500);

  estado_servo_1 = servo_1.read();
  estado_servo_2 = servo_2.read();
  estado_servo_3 = servo_3.read();
  estado_servo_4 = servo_4.read();

  Serial.println("go to 90...");
  mover_servo_1(90);
  mover_servo_2(90);
  mover_servo_3(90);
  mover_servo_4(90);

  Serial.println("servo 1: " + String(estado_servo_1) + " go to: " + String(goto_servo_1));
  Serial.println("servo 2: " + String(estado_servo_2) + " go to: " + String(goto_servo_2));
  Serial.println("servo 3: " + String(estado_servo_3) + " go to: " + String(goto_servo_3));
  Serial.println("servo 4: " + String(estado_servo_4) + " go to: " + String(goto_servo_4));

  mover_servos_a_la_pocicionX();
  Serial.println("calibre completo ");


  DEBUG_SERIAL.begin(115200);
  WiFi.disconnect(true);

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
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  FastLED.show();
  mover_servos();
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

bool TB2Pressed = false;

void executeInstruccion(String obj, String valor) {

  if (obj.substring(0, 1) == "S") {

    String incStr = valor.substring(1);
    //Serial.print(" \t incStr " + incStr);
    int incrementos = incStr.toInt();

    String nvalor = valor.substring(0, 1);

    if (incrementos != 0 && nvalor == "A") {
      incrementos = 1;
    } else if (incrementos != 0 && nvalor == "B") {
      incrementos = -1;
    }

    //  Serial.print(" Obj: " + obj + " - " + obj.substring(0, 1));
    //  Serial.println(" - valor: " + valor + " - v: " + nvalor + "  - in: " + String(incrementos));

    if (obj == "S1") {
      goto_servo_1 = incrementos;
      goto_servo_2 = 0;
      goto_servo_3 = 0;
      goto_servo_4 = 0;
    }
    if (obj == "S2") {
      goto_servo_2 = incrementos;
      goto_servo_1 = 0;
      goto_servo_3 = 0;
      goto_servo_4 = 0;
    }
    if (obj == "S3") {
      goto_servo_3 = incrementos;
      goto_servo_2 = 0;
      goto_servo_1 = 0;
      goto_servo_4 = 0;
    }
    if (obj == "S4") {
      goto_servo_4 = incrementos;
      goto_servo_2 = 0;
      goto_servo_3 = 0;
      goto_servo_1 = 0;
    }
    //velocidadServo = 50;

  } else if (obj == "COLOR") {
    String sR = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String sG = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String sB = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String nombre = valor;

    bool flag = true;
    if (sR.toInt() > 255) flag = false;
    if (sR.toInt() < 0) flag = false;
    if (sG.toInt() > 255) flag = false;
    if (sG.toInt() < 0) flag = false;
    if (sB.toInt() > 255) flag = false;
    if (sB.toInt() < 0) flag = false;

    if (flag) {
      LED_R_value = sR.toInt();
      LED_G_value = sG.toInt();
      LED_B_value = sB.toInt();

      LED_Nombre_value = nombre;
    }
    setLedColor();
  } else if (obj == "TB2") {
    if (valor == "0" && TB2Pressed == false) TB2Pressed = true;
    if (valor == "1" && TB2Pressed == true) {
      TB2Pressed = false;
      encenderLuz();
    }
  }
  //mover_servos();
}

void encenderLuz() {

  if (luzEstado == 0) {
    Serial.println("Encender Luz ");
    digitalWrite(LED_FOCO, HIGH);
    luzEstado = 1;
  } else {
    digitalWrite(LED_FOCO, LOW);
    Serial.println("Apagar Luz ");
    luzEstado = 0;
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
        if (DEVICE_ID_NAME == "BRAZO_01") {
          webSocket.sendTXT("#ID_NAME|BRAZO_SERVO_01");
        } else {
          webSocket.sendTXT("#ID_NAME|BRAZO_SERVO_02");
        }
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

CRGB getColorByName(String nombre) {
  nombre.trim();
  CRGB selColor = CRGB::Black;
  Serial.println(" >>>>> nombre ccolor: " + nombre);
  Serial.println(selColor);

  if (nombre == "AliceBlue") selColor = CRGB::AliceBlue;
  if (nombre == "Amethyst") selColor = CRGB::Amethyst;
  if (nombre == "AntiqueWhite") selColor = CRGB::AntiqueWhite;
  if (nombre == "Aqua") selColor = CRGB::Aqua;
  if (nombre == "Aquamarine") selColor = CRGB::Aquamarine;
  if (nombre == "Azure") selColor = CRGB::Azure;
  if (nombre == "Beige") selColor = CRGB::Beige;
  if (nombre == "Bisque") selColor = CRGB::Bisque;
  if (nombre == "Black") selColor = CRGB::Black;
  if (nombre == "BlanchedAlmond") selColor = CRGB::BlanchedAlmond;
  if (nombre == "Blue") selColor = CRGB::Blue;
  if (nombre == "BlueViolet") selColor = CRGB::BlueViolet;
  if (nombre == "Brown") selColor = CRGB::Brown;
  if (nombre == "BurlyWood") selColor = CRGB::BurlyWood;
  if (nombre == "CadetBlue") selColor = CRGB::CadetBlue;
  if (nombre == "Chartreuse") selColor = CRGB::Chartreuse;
  if (nombre == "Chocolate") selColor = CRGB::Chocolate;
  if (nombre == "Coral") selColor = CRGB::Coral;
  if (nombre == "CornflowerBlue") selColor = CRGB::CornflowerBlue;
  if (nombre == "Cornsilk") selColor = CRGB::Cornsilk;
  if (nombre == "Crimson") selColor = CRGB::Crimson;
  if (nombre == "Cyan") selColor = CRGB::Cyan;
  if (nombre == "DarkBlue") selColor = CRGB::DarkBlue;
  if (nombre == "DarkCyan") selColor = CRGB::DarkCyan;
  if (nombre == "DarkGoldenrod") selColor = CRGB::DarkGoldenrod;
  if (nombre == "DarkGray") selColor = CRGB::DarkGray;
  if (nombre == "DarkGreen") selColor = CRGB::DarkGreen;
  if (nombre == "DarkKhaki") selColor = CRGB::DarkKhaki;
  if (nombre == "DarkMagenta") selColor = CRGB::DarkMagenta;
  if (nombre == "DarkOliveGreen") selColor = CRGB::DarkOliveGreen;
  if (nombre == "DarkOrange") selColor = CRGB::DarkOrange;
  if (nombre == "DarkOrchid") selColor = CRGB::DarkOrchid;
  if (nombre == "DarkRed") selColor = CRGB::DarkRed;
  if (nombre == "DarkSalmon") selColor = CRGB::DarkSalmon;
  if (nombre == "DarkSeaGreen") selColor = CRGB::DarkSeaGreen;
  if (nombre == "DarkSlateBlue") selColor = CRGB::DarkSlateBlue;
  if (nombre == "DarkSlateGray") selColor = CRGB::DarkSlateGray;
  if (nombre == "DarkTurquoise") selColor = CRGB::DarkTurquoise;
  if (nombre == "DarkViolet") selColor = CRGB::DarkViolet;
  if (nombre == "DeepPink") selColor = CRGB::DeepPink;
  if (nombre == "DeepSkyBlue") selColor = CRGB::DeepSkyBlue;
  if (nombre == "DimGray") selColor = CRGB::DimGray;
  if (nombre == "DodgerBlue") selColor = CRGB::DodgerBlue;
  if (nombre == "FireBrick") selColor = CRGB::FireBrick;
  if (nombre == "FloralWhite") selColor = CRGB::FloralWhite;
  if (nombre == "ForestGreen") selColor = CRGB::ForestGreen;
  if (nombre == "Fuchsia") selColor = CRGB::Fuchsia;
  if (nombre == "Gainsboro") selColor = CRGB::Gainsboro;
  if (nombre == "GhostWhite") selColor = CRGB::GhostWhite;
  if (nombre == "Gold") selColor = CRGB::Gold;
  if (nombre == "Goldenrod") selColor = CRGB::Goldenrod;
  if (nombre == "Gray") selColor = CRGB::Gray;
  if (nombre == "Green") selColor = CRGB::Green;
  if (nombre == "GreenYellow") selColor = CRGB::GreenYellow;
  if (nombre == "Honeydew") selColor = CRGB::Honeydew;
  if (nombre == "HotPink") selColor = CRGB::HotPink;
  if (nombre == "IndianRed") selColor = CRGB::IndianRed;
  if (nombre == "Indigo") selColor = CRGB::Indigo;
  if (nombre == "Ivory") selColor = CRGB::Ivory;
  if (nombre == "Khaki") selColor = CRGB::Khaki;
  if (nombre == "Lavender") selColor = CRGB::Lavender;
  if (nombre == "LavenderBlush") selColor = CRGB::LavenderBlush;
  if (nombre == "LawnGreen") selColor = CRGB::LawnGreen;
  if (nombre == "LemonChiffon") selColor = CRGB::LemonChiffon;
  if (nombre == "LightBlue") selColor = CRGB::LightBlue;
  if (nombre == "LightCoral") selColor = CRGB::LightCoral;
  if (nombre == "LightCyan") selColor = CRGB::LightCyan;
  if (nombre == "LightGoldenrodYellow") selColor = CRGB::LightGoldenrodYellow;
  if (nombre == "LightGreen") selColor = CRGB::LightGreen;
  if (nombre == "LightGrey") selColor = CRGB::LightGrey;
  if (nombre == "LightPink") selColor = CRGB::LightPink;
  if (nombre == "LightSalmon") selColor = CRGB::LightSalmon;
  if (nombre == "LightSeaGreen") selColor = CRGB::LightSeaGreen;
  if (nombre == "LightSkyBlue") selColor = CRGB::LightSkyBlue;
  if (nombre == "LightSlateGray") selColor = CRGB::LightSlateGray;
  if (nombre == "LightSteelBlue") selColor = CRGB::LightSteelBlue;
  if (nombre == "LightYellow") selColor = CRGB::LightYellow;
  if (nombre == "Lime") selColor = CRGB::Lime;
  if (nombre == "LimeGreen") selColor = CRGB::LimeGreen;
  if (nombre == "Linen") selColor = CRGB::Linen;
  if (nombre == "Magenta") selColor = CRGB::Magenta;
  if (nombre == "Maroon") selColor = CRGB::Maroon;
  if (nombre == "MediumAquamarine") selColor = CRGB::MediumAquamarine;
  if (nombre == "MediumBlue") selColor = CRGB::MediumBlue;
  if (nombre == "MediumOrchid") selColor = CRGB::MediumOrchid;
  if (nombre == "MediumPurple") selColor = CRGB::MediumPurple;
  if (nombre == "MediumSeaGreen") selColor = CRGB::MediumSeaGreen;
  if (nombre == "MediumSlateBlue") selColor = CRGB::MediumSlateBlue;
  if (nombre == "MediumSpringGreen") selColor = CRGB::MediumSpringGreen;
  if (nombre == "MediumTurquoise") selColor = CRGB::MediumTurquoise;
  if (nombre == "MediumVioletRed") selColor = CRGB::MediumVioletRed;
  if (nombre == "MidnightBlue") selColor = CRGB::MidnightBlue;
  if (nombre == "MintCream") selColor = CRGB::MintCream;
  if (nombre == "MistyRose") selColor = CRGB::MistyRose;
  if (nombre == "Moccasin") selColor = CRGB::Moccasin;
  if (nombre == "NavajoWhite") selColor = CRGB::NavajoWhite;
  if (nombre == "Navy") selColor = CRGB::Navy;
  if (nombre == "OldLace") selColor = CRGB::OldLace;
  if (nombre == "Olive") selColor = CRGB::Olive;
  if (nombre == "OliveDrab") selColor = CRGB::OliveDrab;
  if (nombre == "Orange") selColor = CRGB::Orange;
  if (nombre == "OrangeRed") selColor = CRGB::OrangeRed;
  if (nombre == "Orchid") selColor = CRGB::Orchid;
  if (nombre == "PaleGoldenrod") selColor = CRGB::PaleGoldenrod;
  if (nombre == "PaleGreen") selColor = CRGB::PaleGreen;
  if (nombre == "PaleTurquoise") selColor = CRGB::PaleTurquoise;
  if (nombre == "PaleVioletRed") selColor = CRGB::PaleVioletRed;
  if (nombre == "PapayaWhip") selColor = CRGB::PapayaWhip;
  if (nombre == "PeachPuff") selColor = CRGB::PeachPuff;
  if (nombre == "Peru") selColor = CRGB::Peru;
  if (nombre == "Pink") selColor = CRGB::Pink;
  if (nombre == "Plaid") selColor = CRGB::Plaid;
  if (nombre == "Plum") selColor = CRGB::Plum;
  if (nombre == "PowderBlue") selColor = CRGB::PowderBlue;
  if (nombre == "Purple") selColor = CRGB::Purple;
  if (nombre == "Red") selColor = CRGB::Red;
  if (nombre == "RosyBrown") selColor = CRGB::RosyBrown;
  if (nombre == "RoyalBlue") selColor = CRGB::RoyalBlue;
  if (nombre == "SaddleBrown") selColor = CRGB::SaddleBrown;
  if (nombre == "Salmon") selColor = CRGB::Salmon;
  if (nombre == "SandyBrown") selColor = CRGB::SandyBrown;
  if (nombre == "SeaGreen") selColor = CRGB::SeaGreen;
  if (nombre == "Seashell") selColor = CRGB::Seashell;
  if (nombre == "Sienna") selColor = CRGB::Sienna;
  if (nombre == "Silver") selColor = CRGB::Silver;
  if (nombre == "SkyBlue") selColor = CRGB::SkyBlue;
  if (nombre == "SlateBlue") selColor = CRGB::SlateBlue;
  if (nombre == "SlateGray") selColor = CRGB::SlateGray;
  if (nombre == "Snow") selColor = CRGB::Snow;
  if (nombre == "SpringGreen") selColor = CRGB::SpringGreen;
  if (nombre == "SteelBlue") selColor = CRGB::SteelBlue;
  if (nombre == "Tan") selColor = CRGB::Tan;
  if (nombre == "Teal") selColor = CRGB::Teal;
  if (nombre == "Thistle") selColor = CRGB::Thistle;
  if (nombre == "Tomato") selColor = CRGB::Tomato;
  if (nombre == "Turquoise") selColor = CRGB::Turquoise;
  if (nombre == "Violet") selColor = CRGB::Violet;
  if (nombre == "Wheat") selColor = CRGB::Wheat;
  if (nombre == "White") selColor = CRGB::White;
  if (nombre == "WhiteSmoke") selColor = CRGB::WhiteSmoke;
  if (nombre == "Yellow") selColor = CRGB::Yellow;
  if (nombre == "YellowGreen") selColor = CRGB::YellowGreen;

  Serial.println(selColor);

  return selColor;
}
void setLedColor() {

  Serial.println(" >>>>> update Color r: " + LED_Nombre_value);  //String(LED_R_value) + " - g: " + String(LED_G_value) + "  - b: " + String(LED_B_value));

  for (int i = 0; i < NUM_LEDS; i++) {
    Serial.print(".");
    leds[i] = getColorByName(LED_Nombre_value);  //CRGB(LED_R_value, LED_G_value, LED_B_value);

    FastLED.show();
  }
  Serial.println(".");


  // auto c = converter.RGBToRGBW(LED_R_value, LED_G_value, LED_B_value);

  // LED_R_value = c.r;
  // LED_G_value = c.g;
  // LED_B_value = c.b;
  // LED_W_value = c.w;

  // bool flag = true;

  // while (flag) {
  //   flag = false;

  //   if (LED_R_current_value != LED_R_value) {
  //     if (LED_R_current_value < LED_R_value) LED_R_current_value++;
  //     if (LED_R_current_value > LED_R_value) LED_R_current_value--;
  //     analogWrite(LED_R_PIN, LED_R_current_value);
  //     flag = true;
  //   }

  //   if (LED_G_current_value != LED_G_value) {
  //     if (LED_G_current_value < LED_G_value) LED_G_current_value++;
  //     if (LED_G_current_value > LED_G_value) LED_G_current_value--;
  //     analogWrite(LED_G_PIN, LED_G_current_value);
  //     flag = true;
  //   }

  //   if (LED_B_current_value != LED_B_value) {
  //     if (LED_B_current_value < LED_B_value) LED_B_current_value++;
  //     if (LED_B_current_value > LED_B_value) LED_B_current_value--;
  //     analogWrite(LED_B_PIN, LED_B_current_value);
  //     flag = true;
  //   }

  //   if (LED_W_current_value != LED_W_value) {
  //     if (LED_W_current_value < LED_W_value) LED_W_current_value++;
  //     if (LED_W_current_value > LED_W_value) LED_W_current_value--;
  //     analogWrite(LED_W_PIN, LED_W_current_value);
  //     flag = true;
  //   }

  //   if (LED_Y_current_value != LED_Y_value) {
  //     if (LED_Y_current_value < LED_Y_value) LED_Y_current_value++;
  //     if (LED_Y_current_value > LED_Y_value) LED_Y_current_value--;
  //     analogWrite(LED_Y_PIN, LED_Y_current_value);
  //     flag = true;
  //   }

  //   delay(1);
  // }
}
bool flag = false;

void mover_servos() {

  flag = false;
  if (estado_servo_1 != estado_servo_1 + goto_servo_1) {
    estado_servo_1 += goto_servo_1;
    if (estado_servo_1 < 0) {
      estado_servo_1 = 0;
      goto_servo_1 = 0;
    }
    if (estado_servo_1 > 180) {
      estado_servo_1 = 180;
      goto_servo_1 = 0;
    }
    servo_1.write(estado_servo_1);
    Serial.println(" servo_1 go to: " + String(estado_servo_1));
    flag = true;
    delay(velocidadServo);
  }

  if (estado_servo_2 != estado_servo_2 + goto_servo_2) {
    estado_servo_2 += goto_servo_2;

    if (estado_servo_2 < 0) {
      estado_servo_2 = 0;
      goto_servo_2 = 0;
    }
    if (estado_servo_2 > 180) {
      estado_servo_2 = 180;
      goto_servo_2 = 0;
    }

    servo_2.write(estado_servo_2);
    Serial.println(" servo_2 go to: " + String(estado_servo_2));
    flag = true;
    delay(velocidadServo);
  }

  if (estado_servo_3 != estado_servo_3 + goto_servo_3) {
    estado_servo_3 += goto_servo_3;

    if (estado_servo_3 < 0) {
      estado_servo_3 = 0;
      goto_servo_3 = 0;
    }
    if (estado_servo_3 > 180) {
      estado_servo_3 = 180;
      goto_servo_3 = 0;
    }

    servo_3.write(estado_servo_3);
    Serial.println(" servo_3 go to: " + String(estado_servo_3));
    flag = true;
    delay(velocidadServo);
  }

  if (estado_servo_4 != estado_servo_4 + goto_servo_4) {
    estado_servo_4 += goto_servo_4;

    if (estado_servo_4 < 0) {
      estado_servo_4 = 0;
      goto_servo_4 = 0;
    }
    if (estado_servo_4 > 180) {
      estado_servo_4 = 180;
      goto_servo_4 = 0;
    }

    servo_4.write(estado_servo_4);
    Serial.println(" servo_4 go to: " + String(estado_servo_4));
    flag = true;
    delay(velocidadServo);
  }

  // if (flag) {
  //   if (velocidadServo <= 5) {
  //     velocidadServo = velocidadServo - 5;
  //   }
  // }
}


void mover_servos_a_la_pocicionX() {


  bool servosEnPosicion = estado_servo_1 == goto_servo_1;
  servosEnPosicion = servosEnPosicion && estado_servo_2 == goto_servo_2;
  servosEnPosicion = servosEnPosicion && estado_servo_3 == goto_servo_3;
  servosEnPosicion = servosEnPosicion && estado_servo_4 == goto_servo_4;

  int vuelas = 0;

  while (servosEnPosicion == false) {

    if (estado_servo_1 != goto_servo_1) {
      if (estado_servo_1 < goto_servo_1) {
        estado_servo_1++;
      } else {
        estado_servo_1--;
      }
      servo_1.write(estado_servo_1);
    }

    if (estado_servo_2 != goto_servo_2) {
      if (estado_servo_2 < goto_servo_2) {
        estado_servo_2++;
      } else {
        estado_servo_2--;
      }
      servo_2.write(estado_servo_2);
    }

    if (estado_servo_3 != goto_servo_3) {
      if (estado_servo_3 < goto_servo_3) {
        estado_servo_3++;
      } else {
        estado_servo_3--;
      }
      servo_3.write(estado_servo_3);
    }

    if (estado_servo_4 != goto_servo_4) {
      if (estado_servo_4 < goto_servo_4) {
        estado_servo_4++;
      } else {
        estado_servo_4--;
      }
      servo_4.write(estado_servo_4);
    }

    delay(50);

    servosEnPosicion = estado_servo_1 == goto_servo_1;
    servosEnPosicion = servosEnPosicion && estado_servo_2 == goto_servo_2;
    servosEnPosicion = servosEnPosicion && estado_servo_3 == goto_servo_3;
    servosEnPosicion = servosEnPosicion && estado_servo_4 == goto_servo_4;

    vuelas++;
  }

  goto_servo_1 = 0;
  goto_servo_2 = 0;
  goto_servo_3 = 0;
  goto_servo_4 = 0;
}

void mover_servo_2(int pos) {
  goto_servo_2 = pos;
}

void mover_servo_3(int pos) {
  goto_servo_3 = pos;
}

void mover_servo_4(int pos) {
  goto_servo_4 = pos;
}

void mover_servo_1(int pos) {
  goto_servo_1 = pos;
}