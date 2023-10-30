
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <WebSocketsClient.h>


WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

const char *ssid = "ControlRemotoJoystick";
const char *password = "goodlife";

#define DEVICE_ID_NAME "BRAZO_01"
//#define DEVICE_ID_NAME "BRAZO_02"

unsigned long messageInterval = 5000;
bool connected = false;


String dataAvailable = "";
void executeInstruccion(String obj, String valor) {

  if (obj.substring(0, 1) == "S" || obj.substring(0, 1) == "P") {

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

    if (obj == "S1") {
      goto_servo_1 = incrementos;
      goto_servo_2 = 0;

    } else if (obj == "S2") {
      goto_servo_2 = incrementos;
      goto_servo_1 = 0;

    } else if (obj == "P") {
      brillo_goto = incrementos;
    }

  } else if (obj == "COLOR") {
    String sR = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String sG = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String sB = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String nombre = valor;

    bool flag = true;
    if (sR.toInt() > 255) flag = false;
    if (sR.toInt() < 0) flag = false;
    if (sG.toInt() > 255) flag = false;
    if (sG.toInt() < 0) flag = false;
    if (sB.toInt() > 255) flag = false;
    if (sB.toInt() < 0) flag = false;

    if (flag) {
      LED_R_value = sR.toInt();
      LED_G_value = sG.toInt();
      LED_B_value = sB.toInt();

      LED_R_Brillo_value = LED_R_value * brillo / 100;
      LED_G_Brillo_value = LED_G_value * brillo / 100;
      LED_B_Brillo_value = LED_B_value * brillo / 100;

      LED_Nombre_value = nombre;

    }
  }
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

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
  const uint8_t *src = (const uint8_t *)mem;
  DEBUG_SERIAL.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for (uint32_t i = 0; i < len; i++) {
    if (i % cols == 0) {
      DEBUG_SERIAL.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    DEBUG_SERIAL.printf("%02X ", *src);
    src++;
  }
  DEBUG_SERIAL.printf("\n");
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
        if (DEVICE_ID_NAME == "BRAZO_01") {
          webSocket.sendTXT("#ID_NAME|BRAZO_SERVO_01");
        } else {
          webSocket.sendTXT("#ID_NAME|BRAZO_SERVO_02");
        }
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


void setupWifi() {



  WiFi.disconnect(true);

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
