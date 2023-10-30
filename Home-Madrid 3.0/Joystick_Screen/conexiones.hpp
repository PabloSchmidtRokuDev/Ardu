#include <WiFi.h>
#include <WebSocketsServer.h>

const char *ssid = "ControlRemotoJoystick";
const char *password = "goodlife";

IPAddress ip(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);

const uint8_t wsPort = 81;

unsigned long messageInterval = 20000;


class ConexionDispocitivo {
public:
  //ConexionDispocitivo();
  //ConexionDispocitivo(String _dispocitivoID);
  bool conectado;
  String dispocitivoID;
  int conexionNum;

  void setName(String _dispocitivoID);
  void setEstado(bool _conectado, int _conexionNum);
};

void ConexionDispocitivo::setName(String _dispocitivoID) {
  conectado = false;
  dispocitivoID = _dispocitivoID;
  conexionNum = -1;
}


void ConexionDispocitivo::setEstado(bool _conectado, int _conexionNum) {
  conectado = _conectado;
  conexionNum = _conexionNum;
};


const int maxDevice = 5;
ConexionDispocitivo deviceConectionList[maxDevice];  // An array of MyClass objects using default constructor


WebSocketsServer webSocket = WebSocketsServer(wsPort);

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

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      DEBUG_SERIAL.printf("[%u] Disconnected!\n", num);
      for (int i = 0; i < maxDevice; i++) {
        if (deviceConectionList[i].conexionNum == num) {
          deviceConectionList[i].setEstado(false, 0);
          break;
        }
      }
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        DEBUG_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // send message to client
        webSocket.sendTXT(num, "#CONNECTED|Connected");
      }
      break;
    case WStype_TEXT:
      // DEBUG_SERIAL.printf("[%u] RECEIVE TXT: %s\n", num, payload);

      strPayload = (char *)payload;
      primerCaracter = strPayload.substring(0, 1);

      DEBUG_SERIAL.println("texto: " + strPayload + " - 1er: " + primerCaracter);
      if (primerCaracter == "#") {

        strPayload = strPayload.substring(1);

        strCommando = strPayload.substring(0, strPayload.indexOf('|'));
        strPayload = strPayload.substring(strPayload.indexOf('|') + 1);

        DEBUG_SERIAL.println("strCommando: " + strCommando + " - msj: " + strPayload);

        if (strCommando == "ID_NAME") {
          for (int i = 0; i < maxDevice; i++) {
            if (deviceConectionList[i].dispocitivoID == strPayload) {
              deviceConectionList[i].setEstado(true, num);
              break;
            }
          }

        } else if (strCommando == "MSJ_RECIBIDO") {
          for (int i = 0; i < maxDevice; i++) {
            if (deviceConectionList[i].conexionNum == num) {
              DEBUG_SERIAL.print("msj from: " + deviceConectionList[i].dispocitivoID);
              DEBUG_SERIAL.println(" y dice: " + strPayload);
              break;
            }
          }
        }
      }

      break;
    case WStype_BIN:
      DEBUG_SERIAL.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);

      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_PING:
    case WStype_PONG:
      break;
  }
}

void iniciarWifiServer() {

  deviceConectionList[0].setName("LUZ_01");
  deviceConectionList[1].setName("SOPORTE_CAM_SERVO");
  deviceConectionList[2].setName("BRAZO_SERVO_01");
  deviceConectionList[3].setName("BRAZO_SERVO_02");
  deviceConectionList[4].setName("SOPORTE_MOVIL");

  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    DEBUG_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    DEBUG_SERIAL.flush();
    delay(500);
  }

  WiFi.disconnect(true);
  delay(100);

  //WiFi.mode(WIFI_AP);
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

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void SendDataLuz_01(String Data) {
  if (deviceConectionList[0].conectado == true) {
    Data = "#MSJ_RECIBIDO|" + Data; 
    webSocket.sendTXT(deviceConectionList[0].conexionNum, Data);
    DEBUG_SERIAL.println("Dato enviado a: " + deviceConectionList[0].dispocitivoID);

  } else {
    DEBUG_SERIAL.println("Dispocitivo: " + deviceConectionList[0].dispocitivoID + " desconectado");
  }
}

void SendDataSoporteMovil(String Data) {
  if (deviceConectionList[4].conectado == true) {
    Data = "#MSJ_RECIBIDO|" + Data; 
    webSocket.sendTXT(deviceConectionList[4].conexionNum, Data);
    DEBUG_SERIAL.println("Dato enviado a: " + deviceConectionList[4].dispocitivoID);

  } else {
    DEBUG_SERIAL.println("Dispocitivo: " + deviceConectionList[4].dispocitivoID + " desconectado");
  }
}

void SendDataSopote(String Data) {
  if (deviceConectionList[1].conectado == true) {
    Data = "#MSJ_RECIBIDO|" + Data;    
    webSocket.sendTXT(deviceConectionList[1].conexionNum, Data);
    DEBUG_SERIAL.println("Dato enviado a: " + deviceConectionList[1].dispocitivoID);

  } else {
    DEBUG_SERIAL.println("Dispocitivo: " + deviceConectionList[1].dispocitivoID + " desconectado");
  }
}

void SendDataBrazo01(String Data) {
  int deviceIndex =  2;
if  (opcionSelect == 4) deviceIndex = 3;
  if (deviceConectionList[deviceIndex].conectado == true) {
    Data = "#MSJ_RECIBIDO|" + Data;    
    webSocket.sendTXT(deviceConectionList[deviceIndex].conexionNum, Data);
    DEBUG_SERIAL.println("Dato enviado a: " + deviceConectionList[deviceIndex].dispocitivoID);

  } else {
    DEBUG_SERIAL.print("Dispocitivo: " + deviceConectionList[deviceIndex].dispocitivoID + " desconectado");
        DEBUG_SERIAL.println(" -- deviceIndex: " + String(deviceIndex) + "  - opcionSelect: " +  String(opcionSelect));
    
  }
}