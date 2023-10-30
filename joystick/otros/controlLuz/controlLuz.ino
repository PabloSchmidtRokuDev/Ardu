#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <Servo.h>

const char* ssid = "ControlRemotoJoystick";
const char* password = "goodlife";
const char* hostname = "ESP8266_1";

IPAddress ip(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


#define SERVO_PIN1 2
#define SERVO_PIN2 0
#define PHOTO_PIN 4

#define SERVO_PIN1_DEF_POS 90
#define SERVO_PIN2_DEF_POS 90

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

int distance1 = 0;
int servo1PosActual = SERVO_PIN1_DEF_POS;
Servo servo1;

int distance2 = 0;
int servo2PosActual = SERVO_PIN1_DEF_POS;
Servo servo2;

int incrementos = 2;
int luzEstado = 0;

void delayfor(long milliseconds);
boolean reconnect();

void setup() {
  Serial.begin(115200);

  pinMode(PHOTO_PIN, OUTPUT);

  servo1.attach(SERVO_PIN1);
  servo1.write(SERVO_PIN1_DEF_POS);

  servo2.attach(SERVO_PIN2);
  servo2.write(SERVO_PIN2_DEF_POS);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.disconnect(true);
  delay(1000);

  WiFi.mode(WIFI_AP);
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
  server.begin();
}

String dataAvailable = "";
long lastReconnectAttempt = 0;

void loop() {
  setServoPosition();

  if (WiFi.isConnected()) {
    WiFiClient client = server.available();  // Listen for incoming clients

    if (client) {  // If a new client connects,
      //Serial.println("New Client.");          // print a message out in the serial port
      String currentLine = "";  // make a String to hold incoming data from the client
      currentTime = millis();
      previousTime = currentTime;
      while (client.connected()) {  // && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
        currentTime = millis();
        if (client.available()) {  // if there's bytes to read from the client,
          char c = client.read();  // read a byte, then
          Serial.write(c);         // print it out the serial monitor

          header += c;
          if (c == '\n') {  // if the byte is a newline character
            client.println("HTTP/1.1 200 OK");
            //Serial.print("header: ");
            //Serial.print(header);
            header.trim();
            dataAvailable = header;
            checkData();
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

boolean reconnect() {
  if (!WiFi.isConnected()) {
    Serial.print("Reconnecting WiFi ");
    WiFi.disconnect(false);
    Serial.println("Connecting to WiFi...");
    //WiFi.mode(WIFI_AP);
    //WiFi.begin(ssid, password);
    
  while (!WiFi.softAP(ssid, password)) {
    Serial.println(".");
   delay(100);
  }    
   // if (WiFi.status() != WL_CONNECTED) {
   //   return false;
   // }

    Serial.println("Connected");
  }
}

void delayfor(long milliseconds) {
  long d;
  d = millis();

  while (millis() - d < milliseconds) {

    yield();
  }
}

void checkData() {
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
      checkData();
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
    if (incrementos > 2) incrementos = 2;
    valor = valor.substring(0, 1);
    //Serial.println(" \t new valor " + valor);

    if (obj == "X" && valor == "R") {
      distance2 = -incrementos;
    }

    if (obj == "X" && valor == "L") {
      distance2 = incrementos;
    }

    if (obj == "Y" && valor == "U") {
      distance1 = incrementos;
    }

    if (obj == "Y" && valor == "D") {
      distance1 = -incrementos;
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

    Serial.println("Mueve servos \t S1: " + String(servo1PosActual) + " \t s2: " + String(servo2PosActual) + " \t d1: " + String(distance1) + " \t d2: " + String(distance2));
    servo1.write(servo1PosActual);
    servo2.write(servo2PosActual);
  }
}