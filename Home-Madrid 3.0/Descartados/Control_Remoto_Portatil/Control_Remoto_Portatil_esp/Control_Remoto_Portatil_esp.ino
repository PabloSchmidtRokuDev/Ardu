#include <Arduino.h>


#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

#define DEBUG_SERIAL Serial

#include "conexiones.hpp"

String ShowSerialData() {
  String deviceName = "";
  String deviceCommand = "";
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
      Serial.println(deviceName + "-" + deviceCommand);

      if (deviceName != "" && deviceCommand != "") {
        return deviceName + deviceCommand;
      }
    }


  }
  return "";
}

  void setup() {
    Serial.begin(115200);

    iniciarWifiServer();
  }

  void loop() {
    //Serial.print("-");
    webSocket.loop();
    String Data = ShowSerialData();
    if (Data != "") {
      Data = "#MSJ_RECIBIDO|" + Data;
      webSocket.sendTXT(Data);
      DEBUG_SERIAL.println("Dato enviado: " + Data);
    }
    delay(100);
  }
