const char *ssid = "ControlMaestro";
const char *password = "goodlife";

IPAddress ip(192, 168, 1, 156);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


unsigned long messageInterval = 20000;
WebSocketsClient webSocket;

String strPayload;
String primerCaracter;
String strCommando;
bool connected = false;
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[%u] Disconnected!\n");
      connected = false;
      break;
    case WStype_CONNECTED:
      {
        Serial.println("[WSc] Connected to url: %s\n");
        connected = true;

        // send message to server when Connected
        Serial.println("[WSc] SENT: Connected");
        webSocket.sendTXT("#ID_NAME|remoto");
      }
      break;
    case WStype_TEXT:
      strPayload = (char *)payload;
      primerCaracter = strPayload.substring(0, 1);

      Serial.println("texto: " + strPayload + " - 1er: " + primerCaracter);
      if (primerCaracter == "#") {

        strPayload = strPayload.substring(1);

        strCommando = strPayload.substring(0, strPayload.indexOf('|'));
        strPayload = strPayload.substring(strPayload.indexOf('|') + 1);

        Serial.println("strCommando: " + strCommando + " - msj: " + strPayload);

        if (strCommando == "MSJ_RECIBIDO") {
          String msj = "#MSJ_RECIBIDO| " + strPayload;
          webSocket.sendTXT(msj);
        }

      } else {
        Serial.println("Simple mensaje recibido: " + strPayload);
      }

      break;
    case WStype_BIN:
      Serial.println("[%u] get binary length: %u\n");
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
  WiFi.disconnect(false);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(500);
  }
  WiFi.begin("ControlMaestro", "goodlife");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  // server address, port and URL
  webSocket.begin("192.168.1.150", 81, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
}