/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#include "arduinoFFT.h"


//#include "LedEfect.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const uint32_t connectTimeoutMs = 20000;

String host = "http://192.168.1.200";

const uint16_t port = 80;

IPAddress ip(192, 168, 1, 35);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

// Variable to store the HTTP request
String header;
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

//const char* ssid = "MOVISTAR_CAE0";
//const char* password = "aTchezHwfCpu8MLk6HNn";
const char* ssid = "MainRemote";
const char* password = "goodlife";

void setup() {

  // r = 137;
  // g = 0;
  // b = 255;

  // iniciarVec();

  // Debug console
  Serial.begin(115200);

 Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.softAPConfig(ip, gateway, subnet);

  while (!WiFi.softAP(ssid, password)) {
    Serial.println(".");
    delay(100);
  }

  Serial.println("");
  Serial.print("Iniciado AP:\t");
  Serial.println(ssid);
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());
  lastTime = millis();
}

int botonStatus = HIGH;
int botonBeforeStatus = HIGH;

void loop() {
      if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      String serverPath = "192.168.1.200:80";

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());

      // Send HTTP GET request
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println("from hosting estados: " + payload);

        // 0123456
        // 0,0,0,0


      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}