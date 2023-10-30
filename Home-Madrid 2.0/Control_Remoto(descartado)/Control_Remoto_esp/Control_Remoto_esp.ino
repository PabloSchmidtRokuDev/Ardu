#include <Arduino.h>
#include "FastLED.h"
#include <WiFi.h>

const char* ssid = "MOVISTAR_CAE0";
const char* password = "aTchezHwfCpu8MLk6HNn";
const char* hostname = "ESP8266_1";

//const char* ssid = "MainRemote";
//const char* password = "goodlife";

IPAddress ip(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

IPAddress primaryDNS(8, 8, 8, 8);    //optional
IPAddress secondaryDNS(8, 8, 4, 4);  //optional

TaskHandle_t Task1;
TaskHandle_t Task2;

WiFiServer server(80);

String header;
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


unsigned long lastTime = millis();
unsigned long timerDelay = 3000;
// Variable to store the HTTP request



String deviceName = "";
String deviceCommand = "";

#define NUM_LEDS 600
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
int FRAMES_PER_SECOND = 120;
CRGB leds[NUM_LEDS];

int brillo = 255;
int r, g, b;
bool powerOn = true;

#define TOTAL_EFECTOS 7
uint8_t gCurrentPatternNumber = 0;

#define PIN_LED_DATA 2
#define PIN_BOTON 5

String deviceIDName = "doble_barra";

//************************  class*******************
enum functionSelect {
  FS_nada,
  FS_vMasT,
  FS_vMenosT,
  FS_play,
  FS_power,
  FS_colorX,
  FS_brillo,
  FS_tiempo,
  FS_audioEfecto,
  FS_efectoX
};


class deciveAndCommand {
private:
  String deviceName[10] = { "", "", "", "", "", "", "", "", "", "" };
  String command[10] = { "", "", "", "", "", "", "", "", "", "" };
public:


  void setData(String _deviceName, String _command);  // Aqui esta ++
  String getCommand(String _deviceName);
};

void deciveAndCommand::setData(String _deviceName, String _command) {


  for (int i = 0; i < 10; i++) {
    if (deviceName[i] == "" || deviceName[i] == _deviceName) {
      deviceName[i] = _deviceName;
      command[i] = _command;
      Serial.println("se agrego el commando en la posicion:  " + String(i) + " - " + _deviceName + " - " + _command);
      return;
    }
  }
}

String deciveAndCommand::getCommand(String _deviceName) {
  for (int i = 0; i < 10; i++) {
    if (deviceName[i] == _deviceName && command[i] != "") {
      String cc = command[i];
      command[i] = "";
      Serial.println("comando encontrado en:  " + String(i) + " - " + _deviceName + " - " + cc);

      return cc;
    }
  }
  Serial.println("comando NO encontrado:  " + _deviceName);
  return "";
}

class MatrisCodigoFuncion {
public:
  String codigoHex = "";
  int v1 = -1;
  int v2 = -1;
  int v3 = -1;
  String funcionDesc = "Nada";
  functionSelect funSelect = FS_nada;

  void setData(String hexcode, functionSelect funX, int iv1, int iv2, int iv3, String funDesc);  // Aqui esta ++
};

void MatrisCodigoFuncion::setData(String hexcode, functionSelect funX, int iv1, int iv2, int iv3, String funDesc) {
  codigoHex = hexcode;
  funSelect = funX;
  v1 = iv1;
  v2 = iv2;
  v3 = iv3;
  funcionDesc = funDesc;
}

deciveAndCommand listDeciveAndCommand;

void setup() {
  Serial.begin(115200);
  r = 10;
  g = 10;
  b = 10;

  iniciarVec();

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
}


void loop() {
}


void Task1code(void* pvParameters) {
  Serial.println("run task  1...");

  for (;;) {

    if (powerOn == true && gCurrentPatternNumber >= 0) {
      selectEfecto();
    } else {
      for (int i = 0; i <= NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
    }
  }
}

String checkData(String dataAvailable) {
  Serial.println("checkData: " + dataAvailable);
  if (dataAvailable != "") {
    return listDeciveAndCommand.getCommand(dataAvailable);
  }

  return "";
}

void Task2code(void* pvParameters) {


  int whiteLed = 0;

  Serial.println("");
  // Configures static IP address
  if (!WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    leds[whiteLed] = CRGB::White;
    FastLED.show();
    delay(50);
    leds[whiteLed] = CRGB::Black;

    whiteLed++;
    if (whiteLed == NUM_LEDS) { whiteLed = 0; }
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i % 3 == 0)
      leds[i] = CRGB::Gold;
    else
      leds[i] = CRGB::Black;
  }
  FastLED.show();

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(10);
  }

  for (;;) {

    ShowSerialData();

    WiFiClient client = server.available();  // Listen for incoming clients

    if (client) {                      // If a new client connects,
      Serial.println(" New Client.");  // print a message out in the serial port
      String currentLine = "";         // make a String to hold incoming data from the client
      currentTime = millis();
      previousTime = currentTime;

      while (client.connected()) {  // && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
        currentTime = millis();
        if (client.available()) {  // if there's bytes to read from the client,
          char c = client.read();  // read a byte, then
          Serial.write(c);         // print it out the serial monitor

          header += c;
          if (c == '\n') {  // if the byte is a newline character

            Serial.print("header: ");
            Serial.print(header);
            header.trim();
            String dataForClient = checkData(header);

            client.println(dataForClient);

            header = "";
          } else if (c == '#') {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            Serial.println("end connection");
            header = "";
            client.stop();
          }
        }
      }
    }
  }
}

bool ShowSerialData() {
  deviceName = "";
  deviceCommand = "";
  if (Serial.available()) {

    String serialHeader = "";

    delay(50);
    while (Serial.available()) {  // reading data into char array
      char c = Serial.read();     // writing data into array
      if (c == '\n') break;
      serialHeader += c;
    }

    String linea = serialHeader;  // (char*)buffer;

    Serial.println("llegsa: " + linea);
    linea.trim();

    if (linea.length() >= 3) {
      deviceName = linea.substring(0, 20);
      deviceCommand = linea.substring(20);
      deviceName.trim();
      deviceCommand.trim();
    }

    Serial.println(deviceName + "-" + deviceCommand);


    if (deviceName != "" && deviceCommand != "") {
      if (deviceName == deviceIDName) {
        checkDataCommand(deviceCommand);
        deviceName = "";
        deviceCommand = "";
        return false;
      } else {
        listDeciveAndCommand.setData(deviceName, deviceCommand);
        return true;
      }

    } else {
      deviceName = "";
      deviceCommand = "";
      return false;
    }
  }

  return false;
}


//************************  codigoFuncion*******************

MatrisCodigoFuncion codigoFuncion[40];

void static1() {
  if (powerOn == true && gCurrentPatternNumber == 0) {

    FastLED.setBrightness(brillo);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(r, g, b);
    }
    FastLED.show();
  }
}

typedef void (*SimplePatternList[])();
uint8_t gHue = 0;  // rotating "base color" used by many of the patterns

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void sinEfecto() {
  static1();
}

int posBlanco = 0;

int getRealPosition(int ledPos) {
  if (NUM_LEDS <= ledPos) {
    return ledPos - NUM_LEDS;
  }

  return ledPos;
}

void blancoGira() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;  // gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;

    if (i >= posBlanco && i <= posBlanco + 36) {
      nblend(leds[getRealPosition(pixelnumber)], newcolor, 64);
    } else {
      nblend(leds[getRealPosition(pixelnumber)], CRGB::Black, 64);
    }
  }

  if (NUM_LEDS <= posBlanco) {
    posBlanco = 0;
  } else {
    posBlanco++;
  }
}

void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;  // gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
    leds[random16(NUM_LEDS)] += CRGB::White;
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(255);
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(gHue, 255, 192);
}

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) {  // 9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

MatrisCodigoFuncion getData(String hexCode) {
  for (int i = 0; i < 40; i++) {
    if (codigoFuncion[i].codigoHex == hexCode) {
      return codigoFuncion[i];
    }
  }
  Serial.println("default " + hexCode);
  MatrisCodigoFuncion cf;
  return cf;
}

void iniciarVec() {
  codigoFuncion[0].setData("BF40FF00", FS_power, -1, -1, -1, "Btn - Power ");
  codigoFuncion[1].setData("BE41FF00", FS_play, -1, -1, -1, "Play");

  codigoFuncion[2].setData("A35CFF00", FS_vMasT, -1, -1, -1, "V+");
  codigoFuncion[3].setData("A25DFF00", FS_vMenosT, -1, -1, -1, "V-");

  codigoFuncion[4].setData("A758FF00", FS_colorX, 255, 0, 0, "Red");
  codigoFuncion[5].setData("AB54FF00", FS_colorX, 194, 23, 0, "Naranja fuerte");
  codigoFuncion[6].setData("AF50FF00", FS_colorX, 245, 111, 66, "Naranja medio");
  codigoFuncion[7].setData("E31CFF00", FS_colorX, 230, 153, 65, "Naranja débil");
  codigoFuncion[8].setData("E718FF00", FS_colorX, 250, 250, 0, "Amarillo");

  codigoFuncion[9].setData("A659FF00", FS_colorX, 46, 133, 46, "Green");
  codigoFuncion[10].setData("AA55FF00", FS_colorX, 9, 185, 9, "Verde claro");
  codigoFuncion[11].setData("AE51FF00", FS_colorX, 0, 201, 204, "Celeste");
  codigoFuncion[12].setData("E21DFF00", FS_colorX, 20, 163, 144, "Navy");
  codigoFuncion[13].setData("E619FF00", FS_colorX, 7, 115, 86, "Verde oscuro");

  codigoFuncion[14].setData("BA45FF00", FS_colorX, 37, 37, 162, "Blue");
  codigoFuncion[15].setData("B649FF00", FS_colorX, 31, 95, 234, "Azul claro");
  codigoFuncion[16].setData("B24DFF00", FS_colorX, 97, 5, 54, "Marron");
  codigoFuncion[17].setData("E11EFF00", FS_colorX, 145, 8, 104, "Violeta");
  codigoFuncion[18].setData("E51AFF00", FS_colorX, 194, 10, 142, "Rosado");

  codigoFuncion[19].setData("BB44FF00", FS_colorX, 255, 255, 255, "White");
  codigoFuncion[20].setData("B748FF00", FS_brillo, 30, -1, -1, "Luz baja");
  codigoFuncion[21].setData("B34CFF00", FS_brillo, 100, -1, -1, "Luz media");
  codigoFuncion[22].setData("E01FFF00", FS_brillo, 180, -1, -1, "Luz alta");
  codigoFuncion[23].setData("E41BFF00", FS_brillo, 255, -1, -1, "Luz total");

  codigoFuncion[24].setData("EB14FF00", FS_tiempo, 30, -1, -1, "30 min");
  codigoFuncion[25].setData("EA15FF00", FS_tiempo, 60, -1, -1, "60 min");
  codigoFuncion[26].setData("E916FF00", FS_tiempo, 90, -1, -1, "90 min");
  codigoFuncion[27].setData("E817FF00", FS_tiempo, 120, -1, -1, "120 min");

  codigoFuncion[28].setData("EF10FF00", FS_audioEfecto, 1, -1, -1, "audio Efecto 1");
  codigoFuncion[29].setData("EE11FF00", FS_audioEfecto, 2, -1, -1, "audio Efecto 2");
  codigoFuncion[30].setData("ED12FF00", FS_audioEfecto, 3, -1, -1, "audio Efecto 3");
  codigoFuncion[31].setData("EC13FF00", FS_audioEfecto, 4, -1, -1, "audio Efecto 4");

  codigoFuncion[32].setData("F30CFF00", FS_efectoX, 1, -1, -1, "Jump 3");
  codigoFuncion[33].setData("F20DFF00", FS_efectoX, 2, -1, -1, "Jump 7");
  codigoFuncion[34].setData("F10EFF00", FS_efectoX, 3, -1, -1, "Fade 3");
  codigoFuncion[35].setData("F00FFF00", FS_efectoX, 4, -1, -1, "Fade 7");
  codigoFuncion[36].setData("F708FF00", FS_efectoX, 5, -1, -1, "Quick");
  codigoFuncion[37].setData("F609FF00", FS_efectoX, 6, -1, -1, "Slow");
  codigoFuncion[38].setData("F50AFF00", FS_efectoX, 7, -1, -1, "Flash");
  codigoFuncion[39].setData("F40BFF00", FS_efectoX, 8, -1, -1, "Auto");
}

void onPower(boolean encender) {

  if (powerOn == true && encender == false) {
    Serial.println("Power off ");
    powerOn = false;
  } else {
    Serial.println("Power on ");
    powerOn = true;
  }
}

void executeFunction(MatrisCodigoFuncion cf) {
  if (cf.funSelect == FS_vMasT) {
    //FRAMES_PER_SECOND = FRAMES_PER_SECOND + 10;
    if (brillo < 255) brillo += 5;

    Serial.print("FS_vMasT : ");
    Serial.print(" FS_brillo: ");
    Serial.println(brillo);
  } else if (cf.funSelect == FS_vMenosT) {
    if (brillo > 5) brillo -= 5;

    Serial.print("FS_vMenosT : ");
    Serial.print(" FS_brillo: ");
    Serial.println(brillo);

  } else if (cf.funSelect == FS_play) {
  } else if (cf.funSelect == FS_power) {
    onPower(false);

    Serial.print(" FS_power: ");
    Serial.println(powerOn);

  } else if (cf.funSelect == FS_colorX) {
    onPower(true);
    gCurrentPatternNumber = 0;
    r = cf.v1;
    g = cf.v2;
    b = cf.v3;
    static1();

    Serial.print(" FS_colorX : ");
    Serial.println(String(r) + " - " + String(g) + " - " + String(b));

  } else if (cf.funSelect == FS_brillo) {
    onPower(true);
    brillo = cf.v1;

    Serial.print(" FS_brillo: ");
    Serial.println(brillo);

  } else if (cf.funSelect == FS_tiempo) {
  } else if (cf.funSelect == FS_audioEfecto) {
  } else if (cf.funSelect == FS_efectoX) {
    onPower(true);
    gCurrentPatternNumber = cf.v1 + 1;

    Serial.print("FS_efectoX : ");
    Serial.println(cf.v1);
  }
}

bool checkDataCommand(String dataAvailable) {
  Serial.println("checkDataCommand: " + dataAvailable.substring(0, 3));

  if (dataAvailable != "" && dataAvailable.length() > 0 && dataAvailable.substring(0, 3) == "HEX") {
    String payload = dataAvailable.substring(3);
    Serial.println("checkDataCommand - payload: " + payload);
    if (payload != "FFFFFFFF") {
      MatrisCodigoFuncion cf;
      cf = getData(payload);
      Serial.println("x web: " + cf.funcionDesc + " - " + payload);
      if (cf.funSelect != FS_nada) {
        executeFunction(cf);
        return true;
      }
    }
  }
  return false;
}

SimplePatternList gPatterns = { sinEfecto, pride, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, blancoGira };

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void selectEfecto() {

  gPatterns[gCurrentPatternNumber]();
  if (gCurrentPatternNumber > 0) {

    FastLED.show();

    if (gCurrentPatternNumber == 9) {
      FastLED.delay(3);
      // delay(2);
    } else {
      FastLED.delay(1000 / FRAMES_PER_SECOND);
      delay(15);
    }

    // do some periodic updates
    EVERY_N_MILLISECONDS(20) {
      gHue++;  // slowly cycle the "base color" through the rainbow
    }

    // EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
  } else {
    static1();
  }
}