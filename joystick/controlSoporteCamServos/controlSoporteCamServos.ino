#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

const char *ssid = "ControlRemotoJoystick";
const char *password = "goodlife";

unsigned long messageInterval = 5000;
bool connected = false;

#define DEBUG_SERIAL Serial

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;
Servo servo_6;

const int pin_servo_1 = 2;
const int pin_servo_2 = 0;
const int pin_servo_3 = 4;
const int pin_servo_4 = 5;
const int pin_servo_5 = 14;
const int pin_servo_6 = 12;

int estado_servo_1 = 90;
int estado_servo_2 = 90;
int estado_servo_3 = 90;
int estado_servo_4 = 90;
int estado_servo_5 = 90;
int estado_servo_6 = 90;

int goto_servo_1 = 0;
int goto_servo_2 = 0;
int goto_servo_3 = 0;
int goto_servo_4 = 0;
int goto_servo_5 = 0;
int goto_servo_6 = 0;

int velocidadServo = 10;

void setup() {
  Serial.begin(115200);

  servo_1.attach(pin_servo_1);
  servo_2.attach(pin_servo_2);
  servo_3.attach(pin_servo_3);
  servo_4.attach(pin_servo_4);
  servo_5.attach(pin_servo_5);
  servo_6.attach(pin_servo_6);

  servo_1.write(0);
  servo_2.write(0);
  servo_3.write(0);
  servo_4.write(0);
  servo_5.write(0);
  servo_6.write(0);
  delay(1000);
  Serial.println("go to 90...");
  servo_1.write(90);
  servo_2.write(90);
  servo_3.write(90);
  servo_4.write(90);
  servo_5.write(90);
  servo_6.write(90);
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

bool btn1Pressed = false;

void executeInstruccion(String obj, String valor) {

  if (obj.substring(0, 1) == "S") {

    String incStr = valor.substring(1);
    //Serial.print(" \t incStr " + incStr);
    int incrementos = incStr.toInt();

    String nvalor = valor.substring(0, 1);

    if (incrementos != 0 && nvalor == "A") {
      incrementos = 2;
    } else if (incrementos != 0 && nvalor == "B") {
      incrementos = -2;
    }

 Serial.print(" Obj: " +  obj  + " - " + obj.substring(0, 1));
 Serial.println(" - valor: " + valor + " - v: " + nvalor + "  - in: " + String(incrementos));

    if (obj == "S1") { goto_servo_1 = incrementos; }
    if (obj == "S2") { goto_servo_2 = incrementos; }
    if (obj == "S3") { goto_servo_3 = incrementos; }
    if (obj == "S4") { goto_servo_4 = incrementos; }
    if (obj == "S5") { goto_servo_5 = incrementos; }
    if (obj == "S6") { goto_servo_6 = incrementos; }
  }
  mover_servos();
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
        webSocket.sendTXT("#ID_NAME|SOPORTE_CAM_SERVO");
      }
      break;
    case WStype_TEXT:
      strPayload = (char *)payload;
      primerCaracter = strPayload.substring(0, 1);

      DEBUG_SERIAL.println("texto: " + strPayload + " - 1er: " + primerCaracter);
      if (primerCaracter == "#") {

        strPayload = strPayload.substring(1);

        strCommando = strPayload.substring(0, strPayload.indexOf('|'));
        strPayload = strPayload.substring(strPayload.indexOf('|') +1);

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


/*

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

*/

void mover_servos() {

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
    Serial.println(" servo_1 go to: " +  String(estado_servo_1));
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
     Serial.println(" servo_2 go to: " +  String(estado_servo_2));
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
     Serial.println(" servo_3 go to: " +  String(estado_servo_3));
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
     Serial.println(" servo_4 go to: " +  String(estado_servo_4));
     delay(velocidadServo);
  }

  if (estado_servo_5 != estado_servo_5 + goto_servo_5) {
    estado_servo_5 += goto_servo_5;

    if (estado_servo_5 < 0) {
      estado_servo_5 = 0;
      goto_servo_5 = 0;
    }
    if (estado_servo_5 > 180) {
      estado_servo_5 = 180;
      goto_servo_5 = 0;
    }

    servo_5.write(estado_servo_5);
     Serial.println(" servo_5 go to: " +  String(estado_servo_5));
     delay(velocidadServo);
  }

  if (estado_servo_6 != estado_servo_6 + goto_servo_6) {
    estado_servo_6 += goto_servo_6;

    if (estado_servo_6 < 0) {
      estado_servo_6 = 0;
      goto_servo_6 = 0;
    }
    if (estado_servo_6 > 180) {
      estado_servo_6 = 180;
      goto_servo_6 = 0;
    }

    servo_6.write(estado_servo_6);
     Serial.println(" servo_6 go to: " +  String(estado_servo_6));
     delay(velocidadServo);
  }
}
/*
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
}*/