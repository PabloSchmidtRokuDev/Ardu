#include <Arduino.h>
#include "FastLED.h"

#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

#include <Servo.h>

const char* ssid = "ControlMaestro";
const char* password = "goodlife";

unsigned long messageInterval = 5000;
bool connected = false;

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;

#define SERVO_PIN1 12
#define SERVO_PIN2 14
#define SERVO_PIN3 27
#define SERVO_PIN4 26
#define SERVO_PIN5 25
#define SERVO_PIN6 33

int estado_servo[6] = { 179, 179, 179, 179, 179, 179 };
int goto_servo[6] = { 179, 179, 179, 179, 179, 179 };

#define DATA_PIN 32

int pin_R = 19;
int pin_G = 18;
int pin_B = 5;
int pin_W = 17;

int LED_R_value = 0;
int LED_G_value = 0;
int LED_B_value = 0;
int LED_W_value = 0;

int LED_R_Brillo_value = 0;
int LED_G_Brillo_value = 0;
int LED_B_Brillo_value = 0;
int LED_W_Brillo_value = 0;

int LED_R_Current_value = 0;
int LED_G_Current_value = 0;
int LED_B_Current_value = 0;
int LED_W_Current_value = 0;

int LED_R_Pulse_value = 0;
int LED_B_Pulse_value = 0;
int LED_G_Pulse_value = 0;
int LED_W_Pulse_value = 0;

#define LED_TYPE WS2811
#define COLOR_ORDER GRB

#define tira1 65 * 6

#define NUM_LEDS tira1
CRGB leds[NUM_LEDS];

int BRIGHTNESS = 5;
#define FRAMES_PER_SECOND 120

bool powerOn = true;
bool powerLed = true;

int gCurrentPatternNumber = 1;
int TOTAL_EFECTOS = 3;

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

unsigned long tiempoMotor1 = 0;
unsigned long tiempoMotor2 = 0;

unsigned long tiempoLed1 = 0;
unsigned long tiempoLed2 = 0;

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  Serial.begin(115200);

  myservo1.attach(SERVO_PIN1);
  myservo2.attach(SERVO_PIN2);
  myservo3.attach(SERVO_PIN3);
  myservo4.attach(SERVO_PIN4);
  myservo5.attach(SERVO_PIN5);
  myservo6.attach(SERVO_PIN6);

  pinMode(pin_R, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_W, OUTPUT);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, tira1).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("iniciando...." + String(NUM_LEDS));

  myservo1.write(179);
  myservo2.write(179);
  myservo3.write(179);
  myservo4.write(179);
  myservo5.write(179);
  myservo6.write(179);

  LED_R_Brillo_value = random(0, 255);
  LED_G_Brillo_value = random(0, 255);
  LED_B_Brillo_value = random(0, 255);
  LED_W_Brillo_value = random(0, 50);

  delay(3000);


  xTaskCreatePinnedToCore(
    Task1code,
    "Task1",
    10000,
    NULL,
    1,
    &Task1,
    0);

  tiempo1 = millis();

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

uint8_t gHue = 0;  // rotating "base color" used by many of the patterns
bool flag = false;
int PosServo = 179;

void loop() {
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { sinEfecto, Clasico, endChill };

void selectEfecto() {

  gPatterns[gCurrentPatternNumber]();
  if (gCurrentPatternNumber >= 1) {
    setLedColor();
    mover_servos();

    FastLED.show();
    FastLED.delay(1000 / FRAMES_PER_SECOND);

    EVERY_N_MILLISECONDS(20) {
      gHue++;  // slowly cycle the "base color" through the rainbow
    }
  }
}

void sinEfecto() {
  delay(100);
}

int contador = 0;
void Clasico() {
  tiempo2 = millis();

  if (tiempo2 > (tiempo1 + 5000)) {

    if (contador >= 5) {
      int posA = random(100, 180);
      int posB = random(100, 180);

      goto_servo[0] = posA;
      goto_servo[1] = posB;
      goto_servo[2] = posA;
      goto_servo[3] = posB;
      goto_servo[4] = posA;
      goto_servo[5] = posB;

      tiempoMotor1 = millis();
      contador = -1;
    }

    contador++;

    LED_R_Brillo_value = random(0, 255);
    LED_G_Brillo_value = random(0, 255);
    LED_B_Brillo_value = random(0, 255);
    LED_W_Brillo_value = random(0, 50);
    tiempoLed1 = millis();
    tiempo1 = millis();
    Serial.println(" Cambio color. Contador: " + String(contador));
  }

  blancoGira();
  // addGlitter(30);
}

void endChill() {

  myservo1.write(100);
  myservo2.write(180);
  myservo3.write(100);
  myservo4.write(180);
  myservo5.write(100);
  myservo6.write(180);

  tiempoMotor1 = millis();

  LED_R_Brillo_value = random(0, 50);
  LED_G_Brillo_value = random(0, 50);
  LED_B_Brillo_value = random(0, 50);
  LED_W_Brillo_value = 254;

  tiempoLed1 = millis();
  int cont10 = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (cont10 == 0) leds[i] = CRGB::LightBlue;
    if (cont10 == 1) leds[i] = CRGB::LightCoral;
    if (cont10 == 2) leds[i] = CRGB::LightCyan;
    if (cont10 == 3) leds[i] = CRGB::LightGoldenrodYellow;
    if (cont10 == 4) leds[i] = CRGB::LightGreen;
    if (cont10 == 5) leds[i] = CRGB::LightGrey;
    if (cont10 == 6) leds[i] = CRGB::LightPink;
    if (cont10 == 7) leds[i] = CRGB::LightSalmon;
    if (cont10 == 8) leds[i] = CRGB::LightSeaGreen;
    if (cont10 == 9) leds[i] = CRGB::LightSkyBlue;
    if (cont10 == 10) leds[i] = CRGB::LightSlateGray;
    if (cont10 == 11) leds[i] = CRGB::LightSlateGrey;
    if (cont10 == 12) leds[i] = CRGB::LightSteelBlue;
    if (cont10 == 13) {
      leds[i] = CRGB::LightYellow;
      cont10 = -1;
    }
    cont10++;
  }

  addGlitter(30);
}

void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
    leds[random16(NUM_LEDS)] += CHSV(gHue + random8(64), 200, 255);
    leds[random16(NUM_LEDS)] += CHSV(gHue + random8(64), 200, 255);
    leds[random16(NUM_LEDS)] += CHSV(gHue + random8(64), 200, 255);
    leds[random16(NUM_LEDS)] += CHSV(gHue + random8(64), 200, 255);
  }
}


int posBlanco = 0;

static uint16_t sPseudotime = 0;
static uint16_t sLastMillis = 0;
static uint16_t sHue16 = 0;

uint8_t dothue = 0;
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


  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);

  for (int i = 0; i < NUM_LEDS; i++) {  // 9948

    // flag = true;
    // if (flag) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    nblend(leds[i], newcolor, 64);
    leds[i] = newcolor;
    // } else {
    // leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    //  }
  }
}

bool setLedColor() {

  tiempoLed2 = millis();

  if (tiempoLed2 > (tiempoLed1 + 50)) {

    // if (brillo_goto != 0){
    //   tiempo2 = millis();
    //   if (tiempo2 > (tiempo1 + 100)) {  //Si ha pasado 1 segundo ejecuta el IF
    //     tiempo1 = millis();
    //     brillo += brillo_goto;
    //     if (brillo > 100) brillo = 100;
    //     if (brillo < 5) brillo = 5;

    //     LED_R_Brillo_value = LED_R_value * brillo / 100;
    //     LED_G_Brillo_value = LED_G_value * brillo / 100;
    //     LED_B_Brillo_value = LED_B_value * brillo / 100;
    //     LED_W_Brillo_value = LED_W_value * brillo / 100;

    //     Serial.println(" - valor: " + String(brillo) + " - \t LED_R_Brillo_value: " + String(LED_R_Brillo_value)  + "  \t LED_G_Brillo_value: " + String(LED_G_Brillo_value) + "  \t LED_B_Brillo_value" +String(LED_B_Brillo_value) );
    //   }
    // }

    bool flag = true;

    if (LED_R_Current_value != LED_R_Brillo_value) {
      if (LED_R_Current_value < LED_R_Brillo_value) LED_R_Current_value++;
      else LED_R_Current_value--;
      flag = false;
    }

    if (LED_G_Current_value != LED_G_Brillo_value) {
      if (LED_G_Current_value < LED_G_Brillo_value) LED_G_Current_value++;
      else LED_G_Current_value--;
      flag = false;
    }

    if (LED_B_Current_value != LED_B_Brillo_value) {
      if (LED_B_Current_value < LED_B_Brillo_value) LED_B_Current_value++;
      else LED_B_Current_value--;
      flag = false;
    }


    if (LED_W_Current_value != LED_W_Brillo_value) {
      if (LED_W_Current_value < LED_W_Brillo_value) LED_W_Current_value++;
      else LED_W_Current_value--;
      flag = false;
    }


    if (flag == false) {

      LED_R_Pulse_value = map(LED_R_Current_value, 0, 255, 0, 20);
      LED_B_Pulse_value = map(LED_B_Current_value, 0, 255, 0, 254);
      LED_G_Pulse_value = map(LED_G_Current_value, 0, 255, 0, 254);
      LED_W_Pulse_value = map(LED_W_Current_value, 0, 255, 0, 100);

      analogWrite(pin_R, LED_R_Pulse_value);
      analogWrite(pin_B, LED_B_Pulse_value);
      analogWrite(pin_G, LED_G_Pulse_value);
      analogWrite(pin_W, LED_W_Pulse_value);
    }

    tiempoLed1 = millis();
  }

  return flag;
}

void mover_servos() {

  tiempoMotor2 = millis();

  if (tiempoMotor2 > (tiempoMotor1 + 10)) {

    flag = false;
    for (int i = 0; i < 6; i++) {
      if (estado_servo[i] != goto_servo[i]) {
        if (estado_servo[i] <= goto_servo[i]) {
          estado_servo[i]++;
        } else if (estado_servo[i] >= goto_servo[i]) {
          estado_servo[i]--;
        }
        flag = true;
      }
    }

    if (flag) {
      myservo1.write(estado_servo[0]);
      myservo2.write(estado_servo[1]);
      myservo3.write(estado_servo[2]);
      myservo4.write(estado_servo[3]);
      myservo5.write(estado_servo[4]);
      myservo6.write(estado_servo[5]);
    }
    tiempoMotor1 = millis();
  }
}


void onPower(boolean encender) {
  if (powerOn == true && encender == false) {
    powerOn = false;
    powerLed = false;

    analogWrite(pin_R, 0);
    analogWrite(pin_G, 0);
    analogWrite(pin_B, 0);
    analogWrite(pin_W, 0);
  } else {
    powerOn = true;
    powerLed = true;
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

  gCurrentPatternNumber = efectoSelect;
}

void executeFunction(String CMD) {
  //Serial.print("executeFunction.. : ");

  if (CMD == CMD_efecto || CMD == FLOR_efecto) {
    nextPattern();
    Serial.println(" Efecto: " + String(gCurrentPatternNumber));

  } else if (CMD == CMD_powerFinChill) {
    onPower(true);
    efectoSelect = 2;
    gCurrentPatternNumber = efectoSelect;
    Serial.println(" CMD_powerFinChill: " + String(gCurrentPatternNumber));

  } else if (CMD == CMD_brillo || CMD == FLOR_brillo) {
    onPower(true);
    BRIGHTNESS += 5;

    if (BRIGHTNESS > 50) BRIGHTNESS = 5;

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