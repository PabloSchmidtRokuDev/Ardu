/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

//#include <ESP8266WiFi.h>
//#include <WiFi.h>

#include <ESP8266HTTPClient.h>
//#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;
#include <Servo.h>

#define USE_SERIAL Serial

#ifndef STASSID
const char* ssid     = "MOVISTAR_CAE0";
const char* password = "aTchezHwfCpu8MLk6HNn";
#endif


#define PIN_Entrada 5
#define PIN_Medio 4
#define PIN_PuertaA 0
#define PIN_PuertaB 2

#define PIN_BotonAbrirPuertaA 13
#define PIN_TRIGGER 12
#define PIN_ECO 14
#define PIN_LUCES 15

WiFiServer server(80);

String serverEstados = "http://devpablo.com/Home/Estados.php";

const uint16_t port = 80;

// Variable to store the HTTP request
String header;
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 122);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

Servo servo1;

unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 2000;
const uint32_t connectTimeoutMs = 5000;

void setup()
{
  WiFi.persistent(true);
  USE_SERIAL.begin(115200);

  pinMode(PIN_BotonAbrirPuertaA, INPUT);
  pinMode(PIN_Entrada, OUTPUT);
  pinMode(PIN_Medio, OUTPUT);
  pinMode(PIN_PuertaA, OUTPUT);
  pinMode(PIN_PuertaB, OUTPUT);

  pinMode(PIN_ECO, INPUT);
  pinMode(PIN_TRIGGER, OUTPUT);
  digitalWrite(PIN_TRIGGER, LOW);
  
  //pinMode(PIN_LUCES, OUTPUT);
  //digitalWrite(PIN_LUCES, HIGH);

  servo1.attach(PIN_Medio);
  servo1.write(0);
  servo1.detach();

  digitalWrite(PIN_PuertaA, LOW);
  digitalWrite(PIN_PuertaB, LOW);
  digitalWrite(PIN_Entrada, LOW);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--)
  {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  // Configures static IP address
 // if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  //{
   // Serial.println("STA Failed to configure");
 // }

  // Connect to Wi-Fi network with SSID and password


  // Register multi WiFi networks

wifiMulti.addAP("movil4G", "goodlife");  

  wifiMulti.addAP("PabloDev", "goodlife");  
  wifiMulti.addAP("PabloDev_2_4", "goodlife");
  wifiMulti.addAP("MOVISTAR_CAE0", "aTchezHwfCpu8MLk6HNn");
  
  // WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Print local IP address and start web server
  // Serial.println("");
  // Serial.println("WiFi connected.");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());
  // server.begin();
  
  lastTime = millis();
}

int botonAbrirPuertaA = LOW;
int botonTimbre = LOW;

void loop()
{
  if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
    Serial.print("WiFi connected: ");
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.localIP());

    if ((millis() - lastTime) > timerDelay) {
      USE_SERIAL.print("[HTTP] begin...\n");

      Serial.print(serverEstados);
      Serial.print(':');
      Serial.println(port);
  
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      String serverPath = serverEstados;

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

        USE_SERIAL.println(payload.substring(0, 1));
        USE_SERIAL.println(payload.substring(2, 3));
        USE_SERIAL.println(payload.substring(4, 5));
        USE_SERIAL.println(payload.substring(6, 7));

        if (payload.length() == 7)
        {
          if (payload.substring(0, 1) == "1")
            openDoorNum(0);

          if (payload.substring(2, 3) == "1")
            openDoorNum(1);

          if (payload.substring(4, 5) == "1")
            openDoorNum(2);

          if (payload.substring(6, 7) == "1")
            openDoorNum(3);
        }

        USE_SERIAL.println(payload);
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
    lastTime = millis();
  }
  } else {
    Serial.println("WiFi not connected!");
  }

  // botonAbrirPuertaA = digitalRead(PIN_BotonAbrirPuertaA);
  // if (botonAbrirPuertaA == HIGH)
  // {
  //   USE_SERIAL.println("Abriendo por el Boton...");
  //   digitalWrite(PIN_PuertaA, HIGH);
  //   delay(7000);
  //   digitalWrite(PIN_PuertaA, LOW);
  // }
 
  

}

long leerDistancia() {
  long dis;
  long tiem;

  digitalWrite(PIN_TRIGGER, LOW);  // recibimiento del pulso.
  delayMicroseconds(2);
  digitalWrite(PIN_TRIGGER, HIGH);  // envió del pulso.
  delayMicroseconds(10);
  digitalWrite(PIN_TRIGGER, LOW);

  tiem = pulseIn(PIN_ECO, HIGH);  // fórmula para medir el pulso entrante.

  //dis = tiem * 10 / 292 / 2;  // fórmula para calcular la distancia del sensor ultrasónico.
 dis = tiem * 0.034 /2;
  return dis;
}

void openDoorNum(int doorNum)
{
  if (doorNum == 0)
  {
    Serial.println("Opeb door Entrada");
    digitalWrite(PIN_Entrada, HIGH);
    delay(1000);
    digitalWrite(PIN_Entrada, LOW);
  }

  if (doorNum == 1)
  { 
    Serial.println("Opeb door medio");
    servo1.attach(PIN_Medio);
    delay(100);
    servo1.write(180);
    delay(5000);
    servo1.write(0);
    delay(100);
    servo1.detach();
  }

  if (doorNum == 2)
  {
    Serial.println("Opeb door puerta A");
    digitalWrite(PIN_PuertaA, HIGH);
    delay(3000);
    digitalWrite(PIN_PuertaA, LOW);
  }

  if (doorNum == 3)
  {
    Serial.println("Opeb door B");
    digitalWrite(PIN_PuertaB, HIGH);
    delay(1000);
    digitalWrite(PIN_PuertaB, LOW);
  }
}
