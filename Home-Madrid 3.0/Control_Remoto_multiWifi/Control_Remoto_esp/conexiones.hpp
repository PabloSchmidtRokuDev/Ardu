const char *ssid = "ControlMaestro";
const char *password = "goodlife";

IPAddress ip(192, 168, 1, 150);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);



const uint8_t wsPort = 81;

unsigned long messageInterval = 20000;
/*
class ConexionDispocitivo {
public:
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
*/

WebSocketsServer webSocket = WebSocketsServer(wsPort);

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
  const uint8_t *src = (const uint8_t *)mem;
  //Serial.println("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for (uint32_t i = 0; i < len; i++) {
    if (i % cols == 0) {
     // Serial.println("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
  //  Serial.println("%02X ", *src);
    src++;
  }
 // Serial.println("\n");
}


String strPayload;
String primerCaracter;
String strCommando;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[%u] Disconnected!\n" + String(num));
      listDeciveAndCommand.setDesconexcion(num);
      
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
       // Serial.println("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // send message to client
        webSocket.sendTXT(num, "#CONNECTED|Connected");
      }
      break;
    case WStype_TEXT:
//       Serial.println("[%u] RECEIVE TXT: %s\n", num, payload);

      strPayload = (char *)payload;
      primerCaracter = strPayload.substring(0, 1);

      Serial.println("texto: " + strPayload + " - 1er: " + primerCaracter);
      if (primerCaracter == "#") {

        strPayload = strPayload.substring(1);

        strCommando = strPayload.substring(0, strPayload.indexOf('|'));
        strPayload = strPayload.substring(strPayload.indexOf('|') + 1);

        Serial.println("strCommando: " + strCommando + " - msj: " + strPayload);

        if (strCommando == "ID_NAME") {
          listDeciveAndCommand.agregarDispocitivo(strPayload, num);

        } else if (strCommando == "MSJ_RECIBIDO") {
           Serial.println(" respuesta: " + strPayload);
          // for (int i = 0; i < maxDevice; i++) {
          //   if (deviceConectionList[i].conexionNum == num) {
          //     Serial.print("msj from: " + deviceConectionList[i].dispocitivoID);
          //     Serial.println(" y dice: " + strPayload);
          //     break;
          //   }
          // }
        } else if (strCommando == "audioData") {
          Serial.println(" y dice: " + strPayload);

        }
      }

      break;
    case WStype_BIN:
     // Serial.println("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);

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

bool iniciarWifiServer() {
/*
  deviceConectionList[0].setName(DEVICE_ID_LIVING);
  deviceConectionList[1].setName(DEVICE_ID_HABITACION);
  deviceConectionList[2].setName(DEVICE_ID_FAROS);
  deviceConectionList[3].setName("");
deviceConectionList[4].setName("");
*/
  WiFi.disconnect(false);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  delay(100);

Serial.println(" y dice:2 ");

  while (!WiFi.softAP(ssid, password)) {
    Serial.println(".");
    delay(100);
  }

Serial.println(" y dice:3 ");
  Serial.println("");
  Serial.print("Iniciado AP:\t");
  Serial.println(ssid);
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  return true;
}