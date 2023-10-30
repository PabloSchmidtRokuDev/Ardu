
#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>


#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36

#include <SPI.h>
#include <TFT_eSPI.h>  // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);  // Sprite class needs to be invoked


int xpos = 0;  // Half the screen width
int ypos = 50;

String pantallaS = "";

#include "pantalla.hpp"
#include "fsnPantalla.hpp"

int JoyStick_X = 32;       // Analog Pin  X
int JoyStick_Y = 33;       // // Analog Pin  Y
int JoyStick_button = 17;  // IO Pin

int main_button1 = 0;   // IO Pin
int main_button2 = 35;  // IO Pin

int start_down = 1200;
int start_up = 2100;

#define TFT_BROWN 0x38E0

WiFiMulti wifiMulti;

int periodo = 1000;
unsigned long goToSleepIn = 0;

#define BUTTON_PIN_BITMASK 0x800000000  // 2^33 in hex

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}


long getSeconds() {
  return millis() / 1000;
}

void setup() {
 
  Serial.begin(115200);

  pinMode(JoyStick_X, INPUT);
  pinMode(JoyStick_Y, INPUT);

  pinMode(JoyStick_button, INPUT_PULLUP);
  pinMode(main_button1, INPUT_PULLUP);
  pinMode(main_button2, INPUT_PULLUP);


  tft.begin();
  tft.setRotation(0);
  spr.setColorDepth(16);  // 16 bit colour needed to show anti-aliased fonts

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set the font colour and the background colour
  tft.setTextDatum(TC_DATUM);              // Top Centre datum

  xpos = tft.width() / 2 - 60;  // Half the screen width
  ypos = 10;


  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Small font
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  spr.loadFont(AA_FONT_SMALL);              // Must load the font first into the sprite class
  spr.setTextColor(TFT_YELLOW, TFT_BLACK);  // Set the sprite font colour and the background colour
//   crearMenu();
// delay(2000);
// updateMenu(1);
// delay(2000);
// updateMenu(2);
// delay(2000);
// updateMenu(0);


//delay(100000);
//delay(100000);
//delay(100000);
  
  wifiMulti.addAP("ControlLuz01", "goodlife");
  Serial.println("Waiting for WiFi... ");

  int cont = 0;
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    tft.setCursor(xpos + cont, ypos + 10 + cont);
    spr.printToSprite(".");
    cont++;
    delay(100);
  }

  if (wifiMulti.run() != WL_CONNECTED) {
    tft.fillScreen(TFT_RED);
    ypos = 30;
    tft.setCursor(xpos, ypos);
    spr.printToSprite("wifi no conecta :(");
    ypos += spr.fontHeight() + 20;
    delay(10000);
    goToSleep();


  } else {

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    tft.fillScreen(TFT_BLACK);
    ypos = 30;
    tft.setCursor(xpos, ypos);
    spr.printToSprite("wifi conected");
    ypos += spr.fontHeight() + 20;
    //spr.unloadFont();


    tft.setCursor(xpos, ypos);
    spr.printToSprite("ip: " + String(WiFi.localIP()));
    ypos += spr.fontHeight() + 20;

    tft.setCursor(xpos, ypos);
    spr.printToSprite("iniciando....");
    ypos += spr.fontHeight() + 20;
    //spr.unloadFont();

    delay(500);

    goToSleepIn = getSeconds() + periodo;
  }
}

int x, y, xp, yp, buttonJoystick, button1, button2;
int buttonJoystickP = 1;
int button1P = 1;
int button2P = 1;

int ccc = 0;
unsigned long resta = 0;

void loop() {

  if (ccc == 1000) {
    ccc = 0;
    resta = goToSleepIn - getSeconds();
    if (resta <= 0) {
      goToSleep();

    } else if (resta > getSeconds() + (periodo * 2)) {
      //Serial.println("go to sleep " + String(goToSleepIn) + "  -- " + String(resta) + "  -- " + String(getSeconds() + (periodo * 2)));
      goToSleepIn = getSeconds() + periodo;
    } else {

      //Serial.println("go to sleep " + String(goToSleepIn) + "  -- " + String(resta));

      tft.setCursor(xpos, ypos);
      spr.printToSprite("sleep in: " + String(resta) + "      ");

      //spr.unloadFont();
    }
  }

  ccc++;

Serial.print("Lecctura sensor: "); 
Serial.println(touchRead(2)); 

  String Data = readSensores();

  if (Data != "") {

    // Serial.println("x:\t" + String(x) + "\t \t y:\t" + String(y));
    Serial.println("DATA: " + Data);
    SendData(Data);
  }
}

String readSensores() {
  x = analogRead(JoyStick_X);  //  X
  y = analogRead(JoyStick_Y);  //  Y

  buttonJoystick = digitalRead(JoyStick_button);
  button1 = digitalRead(main_button1);
  button2 = digitalRead(main_button2);

  //Serial.println("x: " + String(x) + "\t y: " + String(y));
  //Serial.println("-- 36: " + String(analogRead(36)) + "\t 39: " + String(analogRead(39)) + " -- B: " + String(digitalRead(2)));
  //delay(500);


  String Data = "";
  if (x < start_down && xp == 0) {
    //x = map(x, 0, start_up, 3, 10);
    x = 2;  // 10 - x + 3;
    xp = 2;
    Data += "X:L" + String(x) + "|";

  } else if (x > start_up && xp == 0) {
    x = 2;  //map(x, start_up, 4095, 3, 10);
    xp = 2;
    Data += "X:R" + String(x) + "|";

  } else if (xp != 0 && x > start_down && x < start_up) {
    xp = 0;
    Data += "X:R0|";
  }

  if (y < start_down && yp == 0) {
    //y = map(y, 0, start_up, 3, 10);
    y = 2;  //10 - y + 3;
    yp = 2;
    Data += "Y:D" + String(y) + "|";

  } else if (y > start_up && yp == 0) {
    y = 2;  //map(y, start_up, 4095, 3, 10);
    yp = 2;
    Data += "Y:U" + String(y) + "|";

  } else if (yp != 0 && y > start_down && y < start_up) {
    yp = 0;
    Data += "Y:U0|";
  }

  if (buttonJoystickP != buttonJoystick) {
    Data += "BJ:" + String(buttonJoystick) + "|";
    buttonJoystickP = buttonJoystick;
  }

  if (button1P != button1) {
    //Data += "B1:" + String(button1) + "|";
    button1P = button1;
    goToSleep();
  }
  
  if (button2P != button2) {
    Data += "B2:" + String(button2) + "|";
    button2P = button2;
  }

  return Data;
}

void goToSleep() {
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  print_wakeup_reason();

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1); //1 = High, 0 = Low

  Serial.println("Going to sleep now");
  delay(1000);
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}


const uint16_t port = 80;
const char* host = "192.168.1.200";

void SendData(String DataXYB) {

  goToSleepIn = getSeconds() + periodo;

  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }

  int maxloops = 0;

  while (DataXYB != "") {

    if (client.connected()) {
      client.println(DataXYB);
    }


    maxloops = 0;

    Serial.println("");
    //wait for the server's reply to become available
    while (!client.available() && maxloops < 1000) {
      maxloops++;
      delay(1);  //delay 1 msec
      Serial.print(".");
    }

    if (client.available() > 0) {
      //read back one line from the server
      String line = client.readStringUntil('\r');
      Serial.println("server response:  " + line);
    } else {
      Serial.println("client.available() timed out ");
      client.stop();
      return;
    }

    DataXYB = readSensores();
    Serial.println("read again: " + DataXYB);
  }

  client.println('#');
  Serial.println("send stop  ");
  client.stop();

  goToSleepIn = getSeconds() + periodo;
}