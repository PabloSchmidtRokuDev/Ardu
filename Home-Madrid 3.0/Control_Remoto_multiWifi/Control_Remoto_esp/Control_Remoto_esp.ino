#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <WebSocketsServer.h>

#include <AccelStepper.h>
#include <Servo.h>

WiFiMulti WiFiMulti;

#include <SPI.h>
#include <Ethernet.h>


#include "arduinoFFT.h"
arduinoFFT FFT = arduinoFFT();

#define RXD2 16
#define TXD2 17

TaskHandle_t Task1;
TaskHandle_t Task2;

#define DEVICE_ID_LIVING "living"
#define DEVICE_ID_HABITACION "flor"
#define DEVICE_ID_FAROS "remoto"
#define DEVICE_ID_DOBLE_BARRA "doble_barra"

#define PIN_Entrada 13

#define PIN_Medio_Pulso 25
// #define PIN_Medio_Dir 26
// #define PIN_Medio_ena 33

#define PIN_PuertaA 27
#define PIN_PuertaB 12

#define PIN_BTN_Puerta_Medio 14
// #define PIN_BTN_2 33
// #define PIN_BTN_3 32

// #define PIN_Mic 2
// #define PIN_BotonPuertaA 34

#define DATA_PIN 15
#define DATA_PIN2 4

#define motorInterfaceType 1
AccelStepper stepper1;

bool listenaAudio = false;

Servo servo1;

#include "fsnVarios.hpp"
#include "conexiones.hpp"

#include "EthernetConexion.hpp"

bool conected = false;

void setup() {
  Serial.begin(115200);

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  Serial.println("star...");

  pinMode(PIN_Entrada, OUTPUT);
  //  pinMode(PIN_Medio, OUTPUT);
  pinMode(PIN_PuertaA, OUTPUT);
  pinMode(PIN_PuertaB, OUTPUT);

  //pinMode(PIN_Mic, INPUT);
 // pinMode(PIN_BTN_Puerta_Medio, INPUT);
pinMode(PIN_BTN_Puerta_Medio, OUTPUT);
  //servo1.attach(PIN_Medio_Pulso);


	servo1.attach(PIN_BTN_Puerta_Medio); // attaches the servo on pin 18 to the servo object

  servo1.write(0);
  // delay(500);
  // servo1.detach();

  // stepper1 = AccelStepper(motorInterfaceType, PIN_Medio_Pulso, PIN_Medio_Dir);

  // // stepper1.setEnablePin(PIN_Medio_ena);
  // pinMode(PIN_Medio_ena, OUTPUT);
  // digitalWrite(PIN_Medio_ena, HIGH);


  // stepper1.setMaxSpeed(2000);
  // stepper1.setAcceleration(2000);

  // stepper1.runToNewPosition(1000);

  // digitalWrite(PIN_Medio_ena, LOW);

  digitalWrite(PIN_PuertaA, LOW);
  digitalWrite(PIN_PuertaB, LOW);
  digitalWrite(PIN_Entrada, LOW);

  // pinMode(PIN_BTN_1, INPUT);
  // pinMode(PIN_BTN_2, INPUT);
  // pinMode(PIN_BTN_3, INPUT);

  iniciarWifiServer();
  // xTaskCreatePinnedToCore(
  //   Task1code,
  //   "Task1",
  //   10000,
  //   NULL,
  //   1,
  //   &Task1,
  //   0);

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

const uint16_t samples = 16;
double vReal[samples];
double vImag[samples];
String audData = "";
void loop() {
  webSocket.loop();

  if (ShowSerialData() == true) {
    Serial.print("commando nuevo ");
    sendMsjToDevice();
  }
}

void procesarCodigo(String comando) {
  Serial.println("estados: " + comando);

  // 0123456
  // 0,0,0,0

  comando.trim();

  if (comando.length() >= 7) {
    if (comando.substring(0, 1) == "1")
      openDoorNum(0);

    if (comando.substring(2, 3) == "1")
      openDoorNum(1);

    if (comando.substring(4, 5) == "1")
      openDoorNum(2);

    if (comando.substring(6, 7) == "1")
      openDoorNum(3);

    if (comando.length() >= 7 && comando.substring(8) != "") {

      executeFunction(comando.substring(8));
    }
  }
}

String dataForClient;
String Data;
void sendMsjToDeviceAudio() {

  for (int i = 0; i < listDeciveAndCommand.getConectionCount(); i++) {
    dataForClient = listDeciveAndCommand.getCommand(i);
    if (dataForClient != "") {

      Data = "#MSJ_RECIBIDO|" + dataForClient;
      webSocket.sendTXT(listDeciveAndCommand.getConextionNum(i), Data);
      Serial.print("-");

    } else {
      Serial.print(":");
    }
  }
}

void sendMsjToDevice() {

  for (int i = 0; i < listDeciveAndCommand.getConectionCount(); i++) {
    dataForClient = listDeciveAndCommand.getCommand(i);
    if (dataForClient != "") {

      Data = "#MSJ_RECIBIDO|" + dataForClient;
      webSocket.sendTXT(listDeciveAndCommand.getConextionNum(i), Data);
      Serial.println("Comando '" + Data + "' enviado a: " + listDeciveAndCommand.getDeviceName(i));

    } else {
      Serial.println("Dispocitivo: " + listDeciveAndCommand.getDeviceName(i) + " desconectado o sin comando");
    }
  }
}

void Task1code(void* pvParameters) {
  Serial.println("run task  1...");



  for (;;) {
    webSocket.loop();
    /*
    if (listenaAudio) {
      for (int j = 0; j < samples; j++) {
        vReal[j] = analogRead(PIN_Mic);
        Serial.print("-");
        Serial.print(vReal[j]);
        vImag[j] = 0;
        delayMicroseconds(100);
      }

      // calculo FFT
      Serial.print("-------");
      FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
      FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, samples);

      for (int f = 0; f < samples; f++) {
        Serial.print("-");
        Serial.print(vReal[f]);
      }

      Serial.println("---<<<<----");
    }
*/
    // if (ShowSerialData() == true) {
    //   Serial.print("commando nuevo ");
    //   sendMsjToDevice();
    // }
  }
}

void Task2code(void* pvParameters) {
  Serial.println("run task  2...");

  bool conected = iniciarConexcion();

  for (;;) {

    if (conected) {
      if ((millis() - lastTime) > timerDelay) {
        String comando = VerificarEstadoCliente();

        if (comando != "") {
          procesarCodigo(comando);
        }

        if (millis() - lastConnectionTime > postingInterval) {
          httpRequest();
        }
      }
    } else {
      delay(10000);
      conected = iniciarConexcion();
    }
  }
}

void openDoorNum(int doorNum) {
  if (doorNum == 0) {
    Serial.println("Open door Entrada");
    digitalWrite(PIN_Entrada, HIGH);
    delay(1000);
    digitalWrite(PIN_Entrada, LOW);
  }

  if (doorNum == 1) {
    Serial.println("Open door medio");
    // digitalWrite(PIN_Medio_ena, HIGH);
    // stepper1.runToNewPosition(800);
    //servo1.attach(PIN_Medio_Pulso);
    servo1.write(180);

    delay(5000);
    servo1.write(0);

    delay(500);
    //servo1.detach();
    // stepper1.runToNewPosition(1000);
    // digitalWrite(PIN_Medio_ena, LOW);
    Serial.println("close door medio");
  }

  if (doorNum == 2) {
    Serial.println("Open door puerta A");
    digitalWrite(PIN_PuertaA, HIGH);
    delay(3000);
    digitalWrite(PIN_PuertaA, LOW);
  }

  if (doorNum == 3) {
    Serial.println("Open door B");
    digitalWrite(PIN_PuertaB, HIGH);
    delay(1000);
    digitalWrite(PIN_PuertaB, LOW);
  }
}
