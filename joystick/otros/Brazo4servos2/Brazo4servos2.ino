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

#define SERVO_PIN1 5
#define SERVO_PIN2 4
#define SERVO_PIN3 3
#define SERVO_PIN4 2

int estado_servo_1 = 90;
int estado_servo_2 = 90;
int estado_servo_3 = 90;
int estado_servo_4 = 90;

int goto_servo_1 = 0;
int goto_servo_2 = 0;
int goto_servo_3 = 0;
int goto_servo_4 = 0;

#define LED_R_PIN 0
#define LED_G_PIN 14
#define LED_B_PIN 12
#define LED_W_PIN 13
#define LED_Y_PIN 15

int LED_R_current_value = 0;
int LED_G_current_value = 0;
int LED_B_current_value = 0;
int LED_W_current_value = 0;
int LED_Y_current_value = 0;

int LED_R_value = 0;
int LED_G_value = 0;
int LED_B_value = 0;
int LED_W_value = 0;
int LED_Y_value = 0;

int velocidadServo = 20;

void setup() {
  Serial.begin(115200);

  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(LED_W_PIN, OUTPUT);
  pinMode(LED_Y_PIN, OUTPUT);

  servo_1.attach(SERVO_PIN1, 500, 2500);
  servo_2.attach(SERVO_PIN2, 500, 2500);
  servo_3.attach(SERVO_PIN3, 500, 2500);
  servo_4.attach(SERVO_PIN4, 500, 2500);

  Serial.println("go to 90...");
  servo_1.write(90);
  servo_2.write(90);
  servo_3.write(90);
  servo_4.write(90);


(LED_R_PIN, LED_R_current_value);
for (int i=0; i<1000;i++){
 
  if (i<255) analogWrite(LED_R_PIN, i);
  if (i > 255 && i<510)  analogWrite(LED_R_PIN, 510 -i);

  if (i > 51 && i<255) analogWrite(LED_G_PIN, i);
  if (i > 255 && i<510)  analogWrite(LED_G_PIN, 510 -i);  

  if (i > 102 && i<255) analogWrite(LED_B_PIN, i);
  if (i > 255 && i<510)  analogWrite(LED_B_PIN, 510 -i);  

    if (i > 150 && i<255) analogWrite(LED_W_PIN, i);
  if (i > 255 && i<510)  analogWrite(LED_W_PIN, 510 -i);  

    if (i > 200 && i<255) analogWrite(LED_Y_PIN, i);
  if (i > 255 && i<510)  analogWrite(LED_Y_PIN, 510 -i);    

delay(10);
}
delay(1000);
  analogWrite(LED_R_PIN, 0);
  analogWrite(LED_G_PIN, 0);
  analogWrite(LED_B_PIN, 0);
  analogWrite(LED_W_PIN, 0);
  analogWrite(LED_Y_PIN, 0);


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
      incrementos = 1;
    } else if (incrementos != 0 && nvalor == "B") {
      incrementos = -1;
    }

    //  Serial.print(" Obj: " + obj + " - " + obj.substring(0, 1));
    //  Serial.println(" - valor: " + valor + " - v: " + nvalor + "  - in: " + String(incrementos));

    if (obj == "S1") { goto_servo_1 = incrementos; }
    if (obj == "S2") { goto_servo_2 = incrementos; }
    if (obj == "S3") { goto_servo_3 = incrementos; }
    if (obj == "S4") { goto_servo_4 = incrementos; }


  } else if (obj == "LR" || obj == "LG" || obj == "LB" || obj == "LW" || obj == "LY") {
    // if (obj == "LR") { LED_R_value = valor; }
    // if (obj == "LG") { LED_G_value = valor; }
    // if (obj == "LB") { LED_B_value = valor; }
    // if (obj == "LW") { LED_W_value = valor; }
    // if (obj == "LY") { LED_Y_value = valor; }

   // setLedColor();
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
        webSocket.sendTXT("#ID_NAME|BRAZO_SERVO_02");
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

void setLedColor() {
  if (LED_R_current_value != LED_R_value) {
    if (LED_R_current_value < LED_R_value) LED_R_current_value++;
    if (LED_R_current_value > LED_R_value) LED_R_current_value--;
    analogWrite(LED_R_PIN, LED_R_current_value);
  }

  if (LED_G_current_value != LED_G_value) {
    if (LED_G_current_value < LED_G_value) LED_G_current_value++;
    if (LED_G_current_value > LED_G_value) LED_G_current_value--;
    analogWrite(LED_G_PIN, LED_G_current_value);
  }

  if (LED_B_current_value != LED_B_value) {
    if (LED_B_current_value < LED_B_value) LED_B_current_value++;
    if (LED_B_current_value > LED_B_value) LED_B_current_value--;
    analogWrite(LED_B_PIN, LED_B_current_value);
  }

  if (LED_W_current_value != LED_W_value) {
    if (LED_W_current_value < LED_W_value) LED_W_current_value++;
    if (LED_W_current_value > LED_W_value) LED_W_current_value--;
    analogWrite(LED_W_PIN, LED_W_current_value);
  }

  if (LED_Y_current_value != LED_Y_value) {
    if (LED_Y_current_value < LED_Y_value) LED_Y_current_value++;
    if (LED_Y_current_value > LED_Y_value) LED_Y_current_value--;
    analogWrite(LED_Y_PIN, LED_Y_current_value);
  }
}

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
    Serial.println(" servo_1 go to: " + String(estado_servo_1));
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
    delay(velocidadServo);
  }
}