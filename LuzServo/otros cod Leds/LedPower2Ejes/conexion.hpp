/*#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <WebSocketsClient.h>
WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
*/
#include <RGBWConverter.h>

RGBWConverter converter(240, 215, 200, true);

#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

const char *ssid = "ControlRemotoJoystick";
const char *password = "goodlife";

// const char* ssid = "ControlMaestro";
// const char* password = "goodlife";

const uint8_t wsPort = 81;

unsigned long messageInterval = 5000;
bool connected = false;


String dataAvailable = "";
void executeInstruccion(String obj, String valor) {

  // String msj;
  if (obj.substring(0, 1) == "S" || obj.substring(0, 1) == "P") {

    String incStr = valor.substring(1);
    //Serial.print(" \t incStr " + incStr);
    int incrementos = incStr.toInt();

    String nvalor = valor.substring(0, 1);

    if (incrementos != 0 && nvalor == "A") {
      LED_R_Brillo_value = LED_R_value * brillo / 100;
      LED_G_Brillo_value = LED_G_value * brillo / 100;
      LED_B_Brillo_value = LED_B_value * brillo / 100;
      LED_W_Brillo_value = LED_W_value * brillo / 100;
    } else if (incrementos != 0 && nvalor == "B") {
      LED_R_Brillo_value = 0;
      LED_G_Brillo_value = 0;
      LED_B_Brillo_value = 0;
      LED_W_Brillo_value = 0;
    }

  } else if (obj == "X" || obj == "Y") {

    //Serial.print("valor " + valor);

    String incStr = valor.substring(1);
    //Serial.print(" \t incStr " + incStr);
    int incrementos = incStr.toInt();
    if (incrementos > 2) incrementos = 2;
    valor = valor.substring(0, 1);
    Serial.println(" \t new valor " + obj + " -- " + valor + " - - " + String(incrementos));

    if (obj == "X" && valor == "R") {
      goto_servo_1 = -incrementos;
    }

    if (obj == "X" && valor == "L") {
      goto_servo_1 = incrementos;
    }

    if (obj == "Y" && valor == "U") {
      goto_servo_2 = incrementos;
    }

    if (obj == "Y" && valor == "D") {
      goto_servo_2 = -incrementos;
    }


  } else if (obj == "COLOR") {
    String sR = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String sG = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String sB = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String nombre = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String sBrillo = valor.substring(0, valor.indexOf(";"));
    valor = valor.substring(valor.indexOf(";") + 1);

    String sBlanco = valor;

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

      brillo = sBrillo.toInt();

      LED_W_value = sBlanco.toInt();
      LED_W_value = map(LED_W_value, 0, 100, 0, 255);

      LED_R_Brillo_value = LED_R_value * brillo / 100;
      LED_G_Brillo_value = LED_G_value * brillo / 100;
      LED_B_Brillo_value = LED_B_value * brillo / 100;
      LED_W_Brillo_value = LED_W_value * brillo / 100;
      setLedColor();
    }
  } else if (obj == "BRILLO") {

      brillo = valor.toInt();

      LED_R_Brillo_value = LED_R_value * brillo / 100;
      LED_G_Brillo_value = LED_G_value * brillo / 100;
      LED_B_Brillo_value = LED_B_value * brillo / 100;
      LED_W_Brillo_value = LED_W_value * brillo / 100;

    //setLedColor();
  }  else if (obj == "BLANCO") {
    LED_W_value = valor.toInt();
    LED_W_value = map(LED_W_value, 0, 100, 0, 255);
    // LED_R_Brillo_value = LED_R_value * brillo / 100;
    // LED_G_Brillo_value = LED_G_value * brillo / 100;
    // LED_B_Brillo_value = LED_B_value * brillo / 100;
    LED_W_Brillo_value = LED_W_value * brillo / 100;

    //setLedColor();
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
        webSocket.sendTXT("#ID_NAME|POWER_LED2EJES");
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
          //webSocket.sendTXT(msj);
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
