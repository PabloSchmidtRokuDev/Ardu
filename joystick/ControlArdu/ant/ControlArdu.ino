int JoyStick_X = 32;  // Analog Pin  X

int JoyStick_Y = 33;  // // Analog Pin  Y

int JoyStick_button = 17;  // IO Pin

#include <WiFi.h>
#include <HTTPClient.h>

//St//ring url = "192.168.1.200/temperature";
    const uint16_t port = 80;
    const char * host = "192.168.1.200";
    
#include "config.h"  // Sustituir con datos de vuestra red
//#include "Client.hpp"
//#include "ESP32_Utils.hpp"
//#include "ESP32_Utils_Client.hpp"

#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

void setup() {

  pinMode(JoyStick_X, INPUT);

  pinMode(JoyStick_Y, INPUT);

  pinMode(JoyStick_button, INPUT_PULLUP);

  Serial.begin(115200);

  //ConnectWiFi_AP();
  //ConnectWiFi_STA();

  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}

int x, y, button;
int xS, yS, buttonS;

WiFiClient client;

void loop() {

  x = analogRead(JoyStick_X);  //  X

  y = analogRead(JoyStick_Y);  //  Y

  button = digitalRead(JoyStick_button);  //

  x = map(x, 0, 4095, 0, 100);

  y = map(y, 0, 4095, 0, 100);

  Serial.print("x: " + String(x));

  Serial.println(" - y: " + String(y));

  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
    delay(5000);
    return;
  }

  client.println("test!!!! XXXXX ");
int maxloops = 0;

  //wait for the server's reply to become available
  while (!client.available() && maxloops < 1000) {
    maxloops++;
    delay(1);  //delay 1 msec
  }
  if (client.available() > 0) {
    //read back one line from the server
    String line = client.readStringUntil('\r');
    Serial.println(line);
  } else {
    Serial.println("client.available() timed out ");
  }

  Serial.println("Closing connection.");
  client.stop();

  delay(1000);



  /* if (x != xS) {
    SendData("X:" + String(x));
    xS = x;
  }
  // else {
  //  Serial.print("x: " + String(x));
  //  Serial.println(" - xs: " + String(xS));
  //}

  if (y != yS) {
    SendData("Y:" + String(y));
    yS = y;
  }

  if (button != buttonS) {
    SendData("B:" + String(button));
    buttonS = button;
  }*/
}

/*
const uint16_t port = 80;
const char* host = "192.168.1.140"; 
WiFiClient client;

void SendData(String DataXYB) {


  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
    delay(5000);

    SendData(DataXYB);
    return;
  }


  Serial.println("Send: " + DataXYB);
  client.println(DataXYB);
  int maxloops = 0;

  //wait for the server's reply to become available
  while (!client.available() && maxloops < 1000) {
    maxloops++;
    delay(1);  //delay 1 msec
  }

  if (client.available() > 0) {
    //read back one line from the server
    String line = client.readStringUntil('\r');

    Serial.println(line);
  //Serial.println('\n');
  }

  //else {
  //  Serial.println("client.available() timed out ");
  //}
}*/